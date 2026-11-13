package main

import (
	"fmt"
	"math"
	"net"
	"os"
	"time"

	xrand "golang.org/x/exp/rand"
	"gonum.org/v1/gonum/stat/distuv"

	quic "github.com/fkwhite/Quic_GO"
	"github.com/mikioh/tcpinfo"
	// "github.com/mikioh/tcpinfo"
)

func SetupDistribution(configuration Configuration, numStream int) float64 {

	var numPktperSlot float64

	source := xrand.NewSource(uint64(time.Now().UnixMicro()))

	switch configuration.InfoStream[numStream].Distribution.Type {
	case "poisson":
		rateEvents := (((configuration.InfoStream[numStream].Distribution.Rate * 1e6) / 8) / (configuration.PktSize)) * (float64(configuration.TimeSlot) * 1e-3)
		poisson := distuv.Poisson{
			Lambda: rateEvents,
			Src:    source,
		}
		numPktperSlot = poisson.Rand()
		// fmt.Println("rateEvents:", rateEvents)
	case "fixed":
		numPktperSlot = (((configuration.InfoStream[numStream].Distribution.Rate * 1e6) * (float64(configuration.TimeSlot) * 1e-3)) / 8) / configuration.PktSize
	case "LogNormal":
		rateEvents := (((configuration.InfoStream[numStream].Distribution.Rate * 1e6) / 8) / (configuration.PktSize)) * (float64(configuration.TimeSlot) * 1e-3)
		LogNormal := distuv.LogNormal{
			Mu:    rateEvents,
			Sigma: configuration.InfoStream[numStream].Distribution.Variance,
			Src:   source,
		}
		numPktperSlot = LogNormal.Rand()

	case "uniform":
		Uniform := distuv.Uniform{
			Min: (((configuration.InfoStream[numStream].Distribution.Min * 1e6) / 8) / configuration.PktSize),
			Max: (((configuration.InfoStream[numStream].Distribution.Max * 1e6) / 8) / configuration.PktSize),
			Src: source,
		}

		numPktperSlot = Uniform.Rand()

	default:
		fmt.Println("Error: Distribution")
	}

	return float64(numPktperSlot)
}

func OnOffrate(configuration Configuration, numStream int) float64 {

	var numPktperSlot float64

	source := xrand.NewSource(uint64(time.Now().UnixMicro()))

	switch configuration.InfoStream[numStream].OnOff.Type {
	case "poisson":
		rateEvents := configuration.InfoStream[numStream].OnOff.Lambda * configuration.InfoStream[numStream].OnOff.OnTime
		poisson := distuv.Poisson{
			Lambda: rateEvents,
			Src:    source,
		}
		numPktperSlot = poisson.Rand()
	case "fixed":
		numPktperSlot = configuration.InfoStream[numStream].OnOff.Lambda * configuration.InfoStream[numStream].OnOff.OnTime

	default:
		fmt.Println("Error: Distribution")
	}

	return float64(numPktperSlot)
}

func OnOff_application(stream quic.Stream, f *os.File, configuration Configuration, numStream int) {
	fileSizeStream := configuration.InfoStream[numStream].FileSize * 1e6
	rem := 0.0 // in
	ctr := 0.0
	if fileSizeStream != 0 { // Set an amount of file
		pktLen := configuration.InfoStream[numStream].OnOff.PktSize
		fmt.Println("Sending stream...", numStream)
		quic.GlobalBuffersWrite(numStream, 0.0)
		rem = fileSizeStream
		// go DoSendData(stream, &rem, pktLen, f, configuration, numStream) // !!
		timestampBeg := time.Now().UnixMicro() // To track the start instant
		for rem != 0 {
			numPktperSlot := math.Round(OnOffrate(configuration, numStream))
			// fmt.Printf("Number of packets to send %d\n", int(numPktperSlot))
			newBytes := math.Min(numPktperSlot*pktLen, rem)

			tip := (configuration.InfoStream[numStream].OnOff.OnTime / numPktperSlot)
			rem -= newBytes
			// fmt.Printf("Sending %d bytes, pending %f\n", int(newBytes), rem)
			aux := newBytes
			for aux > 0 {
				timestamp := time.Now().UnixMicro()
				sent := math.Min(pktLen, aux)
				quic.GlobalBuffersIncr(numStream, sent) // !!
				// message := make([]byte, int(sent)) // !!
				// stream.Write(message) // !!
				logMessage := fmt.Sprint(timestamp, "	", sent, "\n")
				f.WriteString(logMessage) //fichero log
				aux -= sent
				time.Sleep(time.Duration(tip*1000) * time.Millisecond)
			}

			// if rem == 0 {
			// 	mutex.Lock()
			// 	globalFlag[1] += 1
			// 	mutex.Unlock()
			// 	break
			// }
			time.Sleep(time.Duration(configuration.InfoStream[numStream].OnOff.OffTime*1000) * time.Millisecond)
		}

		timestampEnd := time.Now().UnixMicro()
		elapsedTime := (timestampEnd - timestampBeg)
		rate := float64(fileSizeStream) * 8 / float64(elapsedTime)
		fmt.Printf("----- Stream %d finished: %d MB sent at %f Mbps\n", numStream, int(fileSizeStream*1e-6), rate)
		for {
			buffBytes := quic.GlobalBuffersRead(numStream)
			fmt.Printf("Stream %d: waiting with queue %d ... \n", numStream, int(buffBytes))
			time.Sleep(1000 * time.Millisecond)
			if buffBytes == 0.0 {
				break
			}
		}
		quic.GlobalBuffersLog(numStream)

	} else { // Without setting an amount of file
		fmt.Println(" + Runs until simulation finishes")
		pktLen := configuration.InfoStream[numStream].OnOff.PktSize
		fmt.Println("Sending stream...", numStream)
		quic.GlobalBuffersWrite(numStream, 0.0)
		// go DoSendData(stream, &rem, pktLen, f, configuration, numStream) // !!
		timestampBeg := time.Now().UnixMicro()
		for {
			numPktperSlot := math.Round(OnOffrate(configuration, numStream))
			// fmt.Printf("Number of packets to send %d\n", int(numPktperSlot))
			newBytes := numPktperSlot * pktLen

			tip := (configuration.InfoStream[numStream].OnOff.OnTime / numPktperSlot)
			aux := newBytes
			ctr += newBytes
			configuration.InfoStream[numStream].FileSize = ctr
			for aux > 0 {
				timestamp := time.Now().UnixMicro()
				sent := math.Min(pktLen, aux)
				quic.GlobalBuffersIncr(numStream, sent) // !!
				// message := make([]byte, int(sent)) // !!
				// stream.Write(message) // !!
				logMessage := fmt.Sprint(timestamp, "	", sent, "\n")
				f.WriteString(logMessage) //fichero log
				aux -= sent
				time.Sleep(time.Duration(tip*1000) * time.Millisecond)
			}

			time.Sleep(time.Duration(configuration.InfoStream[numStream].OnOff.OffTime*1000) * time.Millisecond)
			// fmt.Printf(" - Stream %d must be sent through DoSendData %f\n", numStream, ctr)
			// mutex.Lock()
			// if globalFlag[0] == globalFlag[1] {
			// 	fmt.Printf(" - Break Stream %d must be sent through DoSendData %f\n", numStream, ctr)
			// 	break
			// }
			// mutex.Unlock()

		}

		timestampEnd := time.Now().UnixMicro()
		elapsedTime := (timestampEnd - timestampBeg)
		rate := float64(ctr) * 8 / float64(elapsedTime)
		fmt.Printf("----- Stream %d finished: %d MB sent at %f Mbps\n", numStream, int(fileSizeStream*1e-6), rate)
		for {
			buffBytes := quic.GlobalBuffersRead(numStream)
			fmt.Printf("Stream %d: waiting with queue %d ... \n", numStream, int(buffBytes))
			time.Sleep(1000 * time.Millisecond)
			if buffBytes == 0.0 {
				break
			}
		}
		quic.GlobalBuffersLog(numStream)
		// time.Sleep(60000 * time.Millisecond)
	}

}

// /////////////////////////////////////////
func slot_application(stream quic.Stream, f *os.File, configuration Configuration, numStream int) {
	fileSizeStream := configuration.InfoStream[numStream].FileSize * 1e6
	pktLen := configuration.PktSize
	fmt.Println("Sending stream...", numStream)

	quic.GlobalBuffersWrite(numStream, 0.0)
	rem := fileSizeStream
	go DoSendData(stream, &rem, pktLen, f, configuration, numStream) // !!

	ticker := time.NewTicker(time.Duration(configuration.TimeSlot) * time.Millisecond)
	timestampBeg := time.Now().UnixMicro()
	for range ticker.C {
		numPktperSlot := math.Round(SetupDistribution(configuration, numStream))
		// if numPktperSlot == 0 {
		// 	fmt.Println("Cero")
		// }
		// fmt.Printf("Number of packets to send %d\n", int(numPktperSlot))
		newBytes := math.Min(numPktperSlot*pktLen, rem)
		// fmt.Printf("Sending %d bytes \n", int(newBytes))
		quic.GlobalBuffersIncr(numStream, newBytes) // !!
		rem -= newBytes
		timestamp := time.Now().UnixMicro()
		aux := newBytes
		for aux > 0 {
			sent := math.Min(pktLen, aux)
			// message := make([]byte, int(sent)) // !!
			// stream.Write(message) // !!
			// time.Sleep(time.Duration(configuration.TimeSlot) * time.Millisecond / time.Duration(numPktperSlot))
			logMessage := fmt.Sprint(timestamp, "	", sent, "\n")
			f.WriteString(logMessage) //fichero log
			aux -= sent
		}

		if rem == 0 {
			// globalFlag[1] += 1
			break
		}
	}

	timestampEnd := time.Now().UnixMicro()
	ticker.Stop()
	elapsedTime := (timestampEnd - timestampBeg)
	rate := float64(fileSizeStream) * 8 / float64(elapsedTime)
	fmt.Printf("----- Stream %d finished: %d MB sent at %f Mbps\n", numStream, int(fileSizeStream*1e-6), rate)
	for {
		buffBytes := quic.GlobalBuffersRead(numStream)
		fmt.Printf("Stream %d: waiting with queue %d ... \n", numStream, int(buffBytes))
		time.Sleep(1000 * time.Millisecond)
		if buffBytes == 0.0 {
			break
		}
	}
	quic.GlobalBuffersLog(numStream)
	// time.Sleep(60000 * time.Millisecond)
}

func DoSendData(stream quic.Stream, rem *float64, pktLen float64, f *os.File, configuration Configuration, numStream int) {
	ctr := 0.0

	fileSize := configuration.InfoStream[numStream].FileSize * 1e6
	for quic.GlobalBuffersRead(numStream) > 0.0 || *rem > 0.0 || ctr != fileSize {
		toSend := math.Min(quic.GlobalBuffersRead(numStream), pktLen)
		// toSend := (quic.GlobalBuffersRead(numStream))
		ctr += toSend
		quic.GlobalBuffersIncr(numStream, -toSend)
		///////////////////////////////////////////
		// timestamp := time.Now().UnixMicro()
		// for toSend > 0 {
		// 	sent := math.Min(pktLen, toSend)
		// 	message := make([]byte, int(sent))
		// 	stream.Write(message)
		// 	logMessage := fmt.Sprint(timestamp, "	", sent, "\n")
		// 	f.WriteString(logMessage) //fichero log
		// 	toSend -= sent
		// }
		/////////////////////////////////////////
		if int(toSend) > 0 {

			message := make([]byte, int(toSend))
			stream.Write(message)
		}

	}

	fmt.Printf("+++++++++++++++++++++++ Stream %d DoSendData is done with %f bytes!!\n", numStream, ctr)
}

func doSendDataTcp(conn net.Conn, f *os.File, configuration Configuration, numStream int) {
	fileSizeStream := configuration.InfoStream[numStream].FileSize * 1e6
	pktLen := configuration.PktSize
	rem := fileSizeStream
	ticker := time.NewTicker(time.Duration(configuration.TimeSlot) * time.Millisecond)
	timestampBeg := time.Now().UnixMicro()
	for range ticker.C {
		numPktperSlot := math.Round(SetupDistribution(configuration, numStream))
		newBytes := math.Min(numPktperSlot*pktLen, rem)
		rem -= newBytes
		timestamp := time.Now().UnixMicro()
		aux := newBytes
		for aux > 0 {
			sent := math.Min(pktLen, aux)
			message := make([]byte, int(sent))
			logMessage := fmt.Sprint(timestamp, "	", sent, "\n")
			f.WriteString(logMessage)
			conn.Write(message)

			aux -= sent
		}

		if rem == 0 {
			break
		}
	}

	timestampEnd := time.Now().UnixMicro()
	ticker.Stop()
	elapsedTime := (timestampEnd - timestampBeg)
	rate := float64(fileSizeStream) * 8 / float64(elapsedTime)
	fmt.Printf("----- Conn ID %d finished: %d MB sent at %f Mbps\n", numStream, int(fileSizeStream*1e-6), rate)
}

func congestionWindowTCP() {
	fmt.Println("Enter congestionWindowTCP")
	var cwndTCP tcpinfo.CongestionControl
	cwnd := -1
	cwndFile := fmt.Sprint("LOGS_congestionWindow/congestionTCP.log") //logClientTCP.log
	fTCP, err := os.OpenFile(cwndFile, os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND, 0666)
	if err != nil {
		fmt.Println("An error has ocurred -- Opening file")
		panic(err)
	}
	defer fTCP.Close()

	for {
		congestionWindow := int(cwndTCP.SenderWindowBytes)
		if cwnd != congestionWindow {
			fmt.Println("Enter IF")
			cwnd = congestionWindow
			_, err := fTCP.WriteString(fmt.Sprint(congestionWindow, "\n")) //fichero log
			if err != nil {
				fmt.Println("An error has ocurred -- Writing file")
				panic(err)
			}
		}
		time.Sleep(1 * time.Nanosecond)
	}

}
