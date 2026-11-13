package main

import (
	"context"
	"crypto/tls"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
	"sync"
	"time"

	quic "github.com/fkwhite/Quic_GO"
	"github.com/jursonmo/go-tcpinfo"
	// "github.com/mikioh/tcpinfo"
)

type ConfigDistribution struct {
	Type     string  `json:"Type"`
	Rate     float64 `json:"Rate"`
	Variance float64 `json:"Variance"`
	Min      float64 `json:"Min"`
	Max      float64 `json:"Max"`
}

type ConfigOnOff struct {
	OnTime  float64 `json:"OnTime"`
	OffTime float64 `json:"OffTime"`
	Type    string  `json:"Type"`
	Lambda  float64 `json:"lambda"`
	PktSize float64 `json:"PktSize"`
}

type ConfigStream struct {
	TypeofApp    string              `json:"TypeofApp"`
	FileSize     float64             `json:"FileSize"`
	Distribution *ConfigDistribution `json:"Distribution,omitempty"`
	OnOff        *ConfigOnOff        `json:"OnOff,omitempty"`
}

type Configuration struct {
	Addr           string         `json:"Addr"`
	Src            string         `json:"Src"`
	PktSize        float64        `json:"PktSize"`
	TotalSession   int            `json:"TotalSession"`
	TotalStream    int            `json:"TotalStream"`
	TimeSlot       int            `json:"TimeSlot"`
	TCP            bool           `json:"TCP"`
	UseDynamicPort bool           `json:"UseDynamicPort"`
	InfoStream     []ConfigStream `json:"InfoStream"`
}

func main() {
	var
	// time.Sleep(1 * time.Second) //necesario para que le de tiempo al servidor a ejecutar primero
	configFile = os.Args[1]

	file, err := os.Open(configFile)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening configuration file")
		panic(err)
	}
	defer file.Close()

	var configuration Configuration

	// Create a JSON decoder
	decoder := json.NewDecoder(file)

	// Decode the JSON data into the Configuration variable
	if err := decoder.Decode(&configuration); err != nil {
		fmt.Println("Error decoding JSON:", err)
		return
	}

	quic.GlobalBuffersInit(configuration.TotalStream) //inicialización de los buffers

	for {
		err = clientMain(configuration)
		if err == nil {
			break //evitar volver a mandar datos
		}
		fmt.Println("Retry - Client")
	}
}

func clientMain(configuration Configuration) error {
	// Connection TCP or QUIC
	tcp := configuration.TCP

	// wireshark
	var keyLog io.Writer
	f, err := os.Create("key.log")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	keyLog = f

	if tcp {
		// TLS + TCP
		fmt.Printf("Connecting to TCP server\n")
		sendDataTCP(configuration)

	} else /* QUIC */ {
		tlsConf := &tls.Config{
			InsecureSkipVerify: true,
			NextProtos:         []string{"quic-echo-example"},
			KeyLogWriter:       keyLog, //wireshark
		}

		var wg sync.WaitGroup //wait for multiple goroutines to finish

		//el cliente puede abrir varias sesiones
		for i := 1; i <= configuration.TotalSession; i++ {
			session, err := quic.DialAddr(configuration.Addr, tlsConf, nil)
			if err != nil {
				return err
			}
			log.Printf("Client : Session %d \n", i)

			// el cliente envia varios streams en cada sesion
			for j := 1; j <= configuration.TotalStream; j++ {
				wg.Add(1)
				go func(i int) {
					defer wg.Done()
					sendStream(session, i, configuration)
				}(i)
				//time.Sleep(10 * time.Millisecond)
			}
		}
		wg.Wait()
	}

	return nil
}

func sendStream(session quic.Session, numSession int, configuration Configuration) {
	stream, err := session.OpenStreamSync(context.Background())

	numStream := int(stream.StreamID() / 4)
	fmt.Printf("Client: open stream %d\n", numStream)
	if err != nil {
		panic(err)
	}

	nameFile := fmt.Sprint("tmp/logClient_", numSession, "_", numStream+1, ".log") //logClient_numSession_numStream.log
	f, err := os.OpenFile(nameFile, os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND, 0666)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening file")
		panic(err)
	}
	defer f.Close()

	if configuration.InfoStream[numStream].TypeofApp == "Slotted" {
		fmt.Println(" + Running a Slotted application")
		slot_application(stream, f, configuration, numStream)
	} else if configuration.InfoStream[numStream].TypeofApp == "OnOff" {
		fmt.Println(" + Running an OnOff application")
		OnOff_application(stream, f, configuration, numStream)

	}

}

func monitorTCPInfo(tcpConn *net.TCPConn) {

	logFile := "tmp/cwndTCP.log"
	f, err := os.OpenFile(logFile, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		fmt.Println("Error opening log file:", err)
		return
	}
	_, err = fmt.Fprintf(f, "Time\tCWND\tMSS\tBytes\tRTT\tRtx\n")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	// Cada vez que queramos la info TCP:
	for {
		info, err := tcpinfo.GetTCPInfo(tcpConn)
		if err != nil {
			fmt.Fprintln(f, "error leyendo TCPInfo:", err)
			break
		}

		line := fmt.Sprint(time.Now().UnixMicro(), "\t",
			info.Snd_cwnd, "\t",
			info.Snd_mss, "\t",
			int(info.Snd_cwnd)*int(info.Snd_mss), "\t",
			info.Rtt, "\t",
			info.Total_retrans, "\n",
		)

		f.WriteString(line)

		time.Sleep(1 * time.Nanosecond)
	}
}

func sendDataTCP(configuration Configuration) {
	// Resolver direcciones

	localAddr := configuration.Src
	if configuration.UseDynamicPort { // flag en el JSON para puerto dinámico
		localAddr = strings.Split(configuration.Src, ":")[0] + ":0" // misma IP, puerto 0
	}

	ldr, err := net.ResolveTCPAddr("tcp", localAddr)
	if err != nil {
		fmt.Println("Error parsing TCP address:", err)
		return
	}
	fmt.Println("+ Local addr: ", configuration.Src)

	rdr, err := net.ResolveTCPAddr("tcp", configuration.Addr)
	if err != nil {
		fmt.Println("Error parsing TCP address:", err)
		return
	}
	fmt.Println("+ Remote addr: ", configuration.Addr)

	// tcpConn, err := net.DialTCP("tcp", ldr, rdr)
	tcpConn, err := net.DialTCP("tcp", ldr, rdr)
	if err != nil {
		fmt.Println("Error connecting to server:", err)
		return
	}
	// tcpConn.SetLinger(0)
	defer tcpConn.Close()

	// TLS
	cert, err := tls.LoadX509KeyPair("certs/client.pem", "certs/client.key")
	if err != nil {
		panic(err)
	}
	tlsConfig := tls.Config{Certificates: []tls.Certificate{cert}, InsecureSkipVerify: true}
	conn := tls.Client(tcpConn, &tlsConfig)
	if err := conn.Handshake(); err != nil {
		fmt.Println("Error performing TLS handshake:", err)
		return
	}
	defer conn.Close()

	// Log del cliente TCP
	logFile := "tmp/logClientTCP.log"
	f, err := os.OpenFile(logFile, os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND, 0666)
	if err != nil {
		fmt.Println("Error opening log file:", err)
		return
	}
	defer f.Close()

	// Lanzar monitor de cwnd en segundo plano
	go monitorTCPInfo(tcpConn)
	// go congestionWindowTCP()
	// Enviar datos usando tu función existente
	doSendDataTcp(conn, f, configuration, 0)
}
