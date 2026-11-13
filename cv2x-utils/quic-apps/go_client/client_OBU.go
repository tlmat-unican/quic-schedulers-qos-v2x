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
	"github.com/fkwhite/Quic_GO/logging"

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

// QuicMetrics para los contadores
type QuicMetrics struct {
	Sent    int64
	Lost    int64
	MaxCwnd int64
	AvgRTT  time.Duration
	mu      sync.Mutex
}

// MiTracer implementa logging.Tracer
type MiTracer struct {
	metrics *QuicMetrics
}

func (t *MiTracer) TracerForConnection(ctx context.Context, p logging.Perspective, odcid logging.ConnectionID) logging.ConnectionTracer {
	return &MiConnTracer{metrics: t.metrics}
}
func (t *MiTracer) SentPacket(net.Addr, *logging.Header, logging.ByteCount, []logging.Frame) {}
func (t *MiTracer) DroppedPacket(net.Addr, logging.PacketType, logging.ByteCount, logging.PacketDropReason) {
}

// MiConnTracer implementa logging.ConnectionTracer con todos los métodos requeridos
type MiConnTracer struct {
	metrics *QuicMetrics
}

// --- MÉTODOS DE MEDICIÓN ---

func (ct *MiConnTracer) SentPacket(hdr *logging.ExtendedHeader, size logging.ByteCount, ack *logging.AckFrame, frames []logging.Frame) {
	ct.metrics.mu.Lock()
	ct.metrics.Sent++
	ct.metrics.mu.Unlock()
}

func (ct *MiConnTracer) LostPacket(encLevel logging.EncryptionLevel, pn logging.PacketNumber, reason logging.PacketLossReason) {
	ct.metrics.mu.Lock()
	ct.metrics.Lost++
	ct.metrics.mu.Unlock()
}

func (ct *MiConnTracer) UpdatedMetrics(rttStats *logging.RTTStats, cwnd, bytesInFlight logging.ByteCount, packetsInFlight int) {
	ct.metrics.mu.Lock()
	ct.metrics.AvgRTT = rttStats.SmoothedRTT()
	if int64(cwnd) > ct.metrics.MaxCwnd {
		ct.metrics.MaxCwnd = int64(cwnd)
	}
	ct.metrics.mu.Unlock()
}

//  MÉTODOS OBLIGATORIOS

func (ct *MiConnTracer) StartedConnection(local, remote net.Addr, srcConnID, destConnID logging.ConnectionID) {
}
func (ct *MiConnTracer) NegotiatedVersion(chosen logging.VersionNumber, clientVersions, serverVersions []logging.VersionNumber) {
}
func (ct *MiConnTracer) ClosedConnection(e error)                                    {}
func (ct *MiConnTracer) SentTransportParameters(tp *logging.TransportParameters)     {}
func (ct *MiConnTracer) ReceivedTransportParameters(tp *logging.TransportParameters) {}
func (ct *MiConnTracer) RestoredTransportParameters(tp *logging.TransportParameters) {}
func (ct *MiConnTracer) ReceivedVersionNegotiationPacket(hdr *logging.Header, versions []logging.VersionNumber) {
}
func (ct *MiConnTracer) ReceivedRetry(hdr *logging.Header) {}
func (ct *MiConnTracer) ReceivedPacket(hdr *logging.ExtendedHeader, size logging.ByteCount, frames []logging.Frame) {
}
func (ct *MiConnTracer) BufferedPacket(typ logging.PacketType) {}
func (ct *MiConnTracer) DroppedPacket(typ logging.PacketType, size logging.ByteCount, reason logging.PacketDropReason) {
}
func (ct *MiConnTracer) UpdatedCongestionState(state logging.CongestionState) {}

// func (ct *MiConnTracer) UpdatedMetrics(rttStats *logging.RTTStats, cwnd, bytesInFlight logging.ByteCount, packetsInFlight int) {
// }
func (ct *MiConnTracer) AcknowledgedPacket(encLevel logging.EncryptionLevel, pn logging.PacketNumber) {
}
func (ct *MiConnTracer) UpdatedPTOCount(value uint32) {}
func (ct *MiConnTracer) UpdatedKeyFromTLS(encLevel logging.EncryptionLevel, pers logging.Perspective) {
}
func (ct *MiConnTracer) UpdatedKey(generation logging.KeyPhase, remote bool)     {}
func (ct *MiConnTracer) DroppedEncryptionLevel(encLevel logging.EncryptionLevel) {}
func (ct *MiConnTracer) DroppedKey(generation logging.KeyPhase)                  {}
func (ct *MiConnTracer) SetLossTimer(typ logging.TimerType, encLevel logging.EncryptionLevel, exp time.Time) {
}
func (ct *MiConnTracer) LossTimerExpired(typ logging.TimerType, encLevel logging.EncryptionLevel) {}
func (ct *MiConnTracer) LossTimerCanceled()                                                       {}
func (ct *MiConnTracer) Debug(name, msg string)                                                   {}
func (ct *MiConnTracer) Close()                                                                   {}
func saveFinalMetrics(m *QuicMetrics) {
	m.mu.Lock()
	s, l := m.Sent, m.Lost
	rtt := m.AvgRTT.Milliseconds()
	cwnd := m.MaxCwnd
	m.mu.Unlock()

	per := 0.0
	if s > 0 {
		per = (float64(l) / float64(s)) * 100
	}

	// Formato simple: Sent Lost PER smoothRTT MaxCwnd
	content := fmt.Sprintf("%d\t%d\t%.4f\t%d\t%d\n", s, l, per, rtt, cwnd)

	err := os.WriteFile("tmp/resultado_final.log", []byte(content), 0644)
	if err != nil {
		fmt.Println("\nError al guardar archivo")
	}
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
func startMetricsMonitor(m *QuicMetrics) {
	fmt.Println("Iniciando monitor de metricas...")
	for {
		time.Sleep(1 * time.Second)
		m.mu.Lock()
		s, l := m.Sent, m.Lost
		m.mu.Unlock()

		if s > 0 {
			per := (float64(l) / float64(s)) * 100
			fmt.Printf("\rSTATS - Enviados: %d, Perdidos: %d, PER: %.4f%%\n", s, l, per)
		}
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
		metricas := &QuicMetrics{}
		miTracer := &MiTracer{metrics: metricas} // Ahora sí está definido
		quicConf := &quic.Config{

			MaxIdleTimeout: 20 * time.Minute,

			Tracer: miTracer, // <--- CONECTAMOS EL TRACER AQUÍ

		}

		// 2. Lanzar monitor en segundo plano
		// go startMetricsMonitor(metricas)
		var wg sync.WaitGroup //wait for multiple goroutines to finish

		//el cliente puede abrir varias sesiones
		for i := 1; i <= configuration.TotalSession; i++ {
			session, err := quic.DialAddr(configuration.Addr, tlsConf, quicConf)
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
			wg.Wait()
			saveFinalMetrics(metricas)
		}

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
