package main

import (
	"context"
	"crypto/rand"
	"crypto/rsa"
	"crypto/tls"
	"crypto/x509"
	"encoding/json"
	"encoding/pem"
	"fmt"
	"io"
	"log"
	"math/big"
	"os"
	"sync"
	"time"

	// "os/exec"

	quic "github.com/fkwhite/Quic_GO"
)

var (
	globalFlag [2]int
	mutex      sync.Mutex
)

type ConfigOnOff struct {
	OnTime  float64 `json:"OnTime"`
	OffTime float64 `json:"OffTime"`
	Type    string  `json:"Type"`
	Lambda  float64 `json:"lambda"`
	PktSize int     `json:"PktSize"`
}
type ConfigStream struct {
	TypeofApp string  `json:"TypeofApp"`
	FileSize  float64 //bytes envia el cliente (en un slot temporal) --> multiplo de bytesStream [MBytes]
	OnOff     *ConfigOnOff
}

type Configuration struct {
	Addr         string
	PktSize      int //bytes (estandar Stream)
	TotalSession int
	TotalStream  int
	TCP          bool //TCP or QUIC connection
	// Distribution  string    //distribucion (uniforme o poisson)
	InfoStream []ConfigStream
}

func main() {
	fmt.Println("Server: Start")

	configFile := os.Args[1]

	file, err := os.Open(configFile)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening configuration file")
		panic(err)
	}
	defer file.Close()
	decoder := json.NewDecoder(file)
	configuration := Configuration{}
	err = decoder.Decode(&configuration)
	if err != nil {
		fmt.Println("error:", err)
	}

	for _, infoStream := range configuration.InfoStream {
		if infoStream.FileSize != 0 {
			mutex.Lock()
			globalFlag[0] += 1
			mutex.Unlock()
		}
	}

	// Connection TCP or QUIC
	tcp := configuration.TCP

	if tcp {
		//TLS + TCP
		readDataTCP(configuration)

	} else /* QUIC */ {
		fmt.Println(configuration.Addr)
		quicConfig := &quic.Config{
			// AQUÍ defines los minutos de espera antes de cortar por inactividad
			MaxIdleTimeout: 20 * time.Minute, 
			
			// Recomendado: Envía pings periódicos para que no se corte si el cálculo es largo
			// KeepAlive:      true,            
		}
		listener, err := quic.ListenAddr(configuration.Addr, generateTLSConfig(), quicConfig)
		if err != nil {
			fmt.Println("An error has ocurred -- Listening")
			panic(err)
		}
		fmt.Println("Server: Listening")

		var wg sync.WaitGroup //wait for multiple goroutines to finish

		// createSession(listener, 0, configuration)

		//no abrir mas sesiones de las necesarias
		for i := 1; i <= configuration.TotalSession; i++ {
			wg.Add(1)
			go func(numSession int) {
				defer wg.Done()
				createSession(listener, numSession, configuration)
			}(i)
		}

		wg.Wait()
	}
	// out, err := exec.Command("pwd").Output() //sudo killall /go_client/client"
	// if err != nil {
	// 		log.Fatal(err)
	// }
	// fmt.Println(string(out))
	fmt.Println("Server: End")
}

func createSession(listener quic.Listener, numSession int, configuration Configuration) {
	var wg sync.WaitGroup //wait for multiple goroutines to finish

	sess, err := listener.Accept(context.Background())
	if err != nil {
		panic(err)
	}
	fmt.Printf("Server: Accepted session\n")

	//no aceptar mas streams de las necesarios
	for i := 1; i <= configuration.TotalStream; i++ {

		wg.Add(1)
		go func() {
			defer wg.Done()
			acceptStream(sess, numSession, configuration)
		}()

	}

	wg.Wait()

	fmt.Println("End Session")
}

func acceptStream(sess quic.Session, numSession int, configuration Configuration) {
	stream, err := sess.AcceptStream(context.Background())
	numStream := int(stream.StreamID() / 4)

	if err != nil {
		fmt.Println("An error has ocurred -- Stream")
		panic(err)
	}
	fmt.Printf("Server: Accepted stream %d\n", numStream)
	// sess.GetCongestionWindow()

	nameFile := fmt.Sprint("tmp/logServer_", numSession, "_", numStream+1, ".log") //logServer_numSession_numStream.log
	f, err := os.OpenFile(nameFile, os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND, 0666)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening file")
		panic(err)
	}
	defer f.Close()

	//leer todo el rato de 1 stream (de configuration.PktSize en configuration.PktSize)
	buf := make([]byte, configuration.PktSize)
	if configuration.InfoStream[numStream].TypeofApp != "Slotted" {
		buf = make([]byte, configuration.InfoStream[numStream].OnOff.PktSize)
	}

	if configuration.InfoStream[numStream].TypeofApp == "OnOff" && configuration.InfoStream[numStream].FileSize == 0 {

		timestampBeg := time.Now().UnixMicro()
		first := true
		localBytesReceived := 0

		for globalFlag[0] != globalFlag[1] {
			numBytes, err := io.ReadFull(stream, buf)
			timestamp := time.Now().UnixMicro()
			if first {
				timestampBeg = timestamp
				first = false
			}
			localBytesReceived += numBytes
			if localBytesReceived%1e7 == 0 {
				fmt.Printf("Stream %d received %d bytes \n", numStream, localBytesReceived)
			}

			if numBytes != 0 {
				logMessage := fmt.Sprint(timestamp, "	", numBytes, "\n")
				_, err := f.WriteString(logMessage) //fichero log
				if err != nil {
					fmt.Println("An error has ocurred -- Writing file")
					panic(err)
				}

			}

				if err != nil {
                if err != io.EOF {
                    // Si es un error real (no EOF), lo imprimimos
                    fmt.Printf("Stream %d error de lectura: %v\n", numStream, err)
                }
                break // ROMPEMOS EL BUCLE AQUÍ
		}
		}

	
		timestampEnd := time.Now().UnixMicro()
		elapsedTime := (timestampEnd - timestampBeg)
		rate := float64(localBytesReceived) * 8 / float64(elapsedTime)

		fmt.Printf("Stream ID: %d, received %d MB at %f Mbps\n", numStream, localBytesReceived/1e6, rate)
		fmt.Printf("Stream ID: %d, Total Time: %d s\n", numStream, elapsedTime/1e6)

	} else {
	// 	fileSize := configuration.InfoStream[numStream].FileSize * 1e6

	// 	timestampBeg := time.Now().UnixMicro()
	// 	first := true
	// 	localBytesReceived := 0

	// 	for localBytesReceived < int(fileSize) {
	// 		numBytes, err := io.ReadFull(stream, buf)
	// 		timestamp := time.Now().UnixMicro()
	// 		if first {
	// 			timestampBeg = timestamp
	// 			first = false
	// 		}
	// 		localBytesReceived += numBytes
	// 		if localBytesReceived%1e7 == 0 {
	// 			fmt.Printf("Stream %d received %d/%d bytes   \n", numStream, localBytesReceived, int(fileSize))
	// 		}
	// 		if numBytes != 0 {
	// 			logMessage := fmt.Sprint(timestamp, "	", numBytes, "\n")
	// 			_, err := f.WriteString(logMessage) //fichero log
	// 			if err != nil {
	// 				fmt.Println("An error has ocurred -- Writing file")
	// 				panic(err)
	// 			}
	// 		}
	// 		if err != nil {
    //             if err != io.EOF {
    //                 fmt.Printf("Stream %d error de lectura: %v\n", numStream, err)
    //             }
    //             break // dejamos de leer
	// 		}
	// 	}
	// 	timestampEnd := time.Now().UnixMicro()
	// 	elapsedTime := (timestampEnd - timestampBeg)
	// 	rate := float64(localBytesReceived) * 8 / float64(elapsedTime)

	// 	fmt.Printf("Stream ID: %d, received %d MB at %f Mbps\n", numStream, localBytesReceived/1e6, rate)
	// 	fmt.Printf("Stream ID: %d, Total Time: %d s\n", numStream, elapsedTime/1e6)
	// 	// os.Exit(0)
	// 	mutex.Lock()
	// 	globalFlag[1] += 1
	// 	mutex.Unlock()
	
	// Definimos el tamaño total esperado
	targetSize := int64(configuration.InfoStream[numStream].FileSize * 1e6)
	localBytesReceived := int64(0)

	timestampBeg := time.Now().UnixMicro()
	first := true

	// Buffer de lectura (ej. 32KB es estándar, ajusta según necesidad)
	// buf := make([]byte, 32*1024) 

	for localBytesReceived < targetSize {
		// 1. IMPORTANTE: Refrescar el Deadline antes de cada lectura.
		// Esto dice: "Si no recibo nada en 10 segs, es un error. Si recibo algo, resetea el reloj."
		// stream.SetReadDeadline(time.Now().Add(10 * time.Second))

		// 2. Usar stream.Read en lugar de ReadFull
		// Esto evita bloquearse esperando llenar el buffer al final del stream
		numBytes, err := io.ReadFull(stream, buf)
		
		// Capturamos timestamp justo al recibir
		timestamp := time.Now().UnixMicro()
		if first {
			timestampBeg = timestamp
			first = false
		}

		// 3. Procesar los bytes recibidos (incluso si err != nil)
		if numBytes > 0 {
			// Asegurarnos de no contar bytes extra si el emisor se pasó
			remaining := targetSize - localBytesReceived
			if int64(numBytes) > remaining {
				numBytes = int(remaining) // Truncar si excedemos lo esperado
			}

			localBytesReceived += int64(numBytes)

			// Logging a fichero (Nota: esto puede ser lento, considerar bufio)
			logMessage := fmt.Sprintf("%d   %d\n", timestamp, numBytes)
			if _, errLog := f.WriteString(logMessage); errLog != nil {
				fmt.Printf("Error escribiendo log stream %d: %v\n", numStream, errLog)
			}

			// Logging a consola cada 10MB
			if localBytesReceived % 1000000 < int64(numBytes) { // Truco para detectar el cruce de umbral
				fmt.Printf("Stream %d received %d/%d bytes\n", numStream, localBytesReceived, targetSize)
			}
		}

		// 4. Gestión de errores
		if err != nil {
			if err == io.EOF {
				// Fin de transmisión normal
				break 
			}
			// Error real (timeout, conexión caída, etc.)
			fmt.Printf("Stream %d error de lectura crítico: %v\n", numStream, err)
			break
		}
}

// Cálculos finales
timestampEnd := time.Now().UnixMicro()
elapsedTime := timestampEnd - timestampBeg
if elapsedTime == 0 { elapsedTime = 1 } // Evitar división por cero

// Mbps = (Bytes * 8 bits) / (Microsegundos) 
// Nota: (B * 8 / us) es equivalente a Mbps directo
rate := float64(localBytesReceived) * 8.0 / float64(elapsedTime)

fmt.Printf("Stream ID: %d, FINAL: %d MB at %.2f Mbps\n", numStream, localBytesReceived/1e6, rate)
fmt.Printf("Stream ID: %d, Total Time: %.2f s\n", numStream, float64(elapsedTime)/1e6)

mutex.Lock()
globalFlag[1] += 1
mutex.Unlock()

}

}

// TLS + TCP --> receive data
func readDataTCP(configuration Configuration) {
	fmt.Println("Server: TCP")
	//TLS
	cert, err := tls.LoadX509KeyPair("./certs/server.pem", "./certs/server.key")
	if err != nil {
		log.Fatalf("server: loadkeys: %s", err)
	}
	config := tls.Config{Certificates: []tls.Certificate{cert}}
	config.Rand = rand.Reader

	//TCP
	fmt.Println("Server listening at addr: ", configuration.Addr)
	listener, err := tls.Listen("tcp", configuration.Addr, &config)
	if err != nil {
		fmt.Println("error:", err)
	}

	conn, err := listener.Accept()
	if err != nil {
		fmt.Println("error:", err)
	}
	fmt.Printf("Connection accepted\n")

	//TCP
	nameFile := fmt.Sprint("tmp/logServerTCP", ".log") //logServerTCP.log
	f, err := os.OpenFile(nameFile, os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND, 0666)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening file")
		panic(err)
	}
	defer f.Close()

	// Read data
	timestampBeg := time.Now().UnixMicro()
	buf := make([]byte, configuration.PktSize)
	numBytesTotalTCP := 0
	fileSize := configuration.InfoStream[0].FileSize * 1e6

	for numBytesTotalTCP < int(fileSize) {
		numBytes, _ := conn.Read(buf)

		timestamp := time.Now().UnixMicro()

		numBytesTotalTCP += numBytes
		if numBytesTotalTCP%1e6 == 0 {
			fmt.Printf("TCP received %d/%d bytes\n", int(numBytesTotalTCP), int(fileSize))
		}

		if numBytes == 0 {
			//si no hay mas datos, evitar escribir --> timeout
			continue
		}
		logMessage := fmt.Sprint(timestamp, "	", numBytes, "\n")
		// fmt.Println(logMessage) //por pantalla
		_, err := f.WriteString(logMessage) //fichero log
		if err != nil {
			fmt.Println("An error has ocurred -- Writing file")
			panic(err)
		}
	}
	timestampEnd := time.Now().UnixMicro()
	elapsedTime := (timestampEnd - timestampBeg)
	rate := float64(numBytesTotalTCP) * 8 / float64(elapsedTime)

	fmt.Printf("Conn ID: -, received %d MB at %f Mbps\n", numBytesTotalTCP/1e6, rate)
	fmt.Printf("Conn ID: -, Total Time: %d s\n", elapsedTime/1e6)

}

// Setup a bare-bones TLS config for the server
func generateTLSConfig() *tls.Config {
	key, err := rsa.GenerateKey(rand.Reader, 1024)
	if err != nil {
		panic(err)
	}
	template := x509.Certificate{SerialNumber: big.NewInt(1)}
	certDER, err := x509.CreateCertificate(rand.Reader, &template, &template, &key.PublicKey, key)
	if err != nil {
		panic(err)
	}
	keyPEM := pem.EncodeToMemory(&pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(key)})
	certPEM := pem.EncodeToMemory(&pem.Block{Type: "CERTIFICATE", Bytes: certDER})

	tlsCert, err := tls.X509KeyPair(certPEM, keyPEM)
	if err != nil {
		panic(err)
	}
	return &tls.Config{
		Certificates: []tls.Certificate{tlsCert},
		NextProtos:   []string{"quic-echo-example"},
	}
}
