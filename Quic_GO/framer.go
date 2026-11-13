package quic

import (
	"encoding/json"
	"errors"
	"fmt"
	"math"
	"os"
	"sort"
	"sync"
	"time"

	"github.com/fkwhite/Quic_GO/internal/ackhandler"
	"github.com/fkwhite/Quic_GO/internal/protocol"
	"github.com/fkwhite/Quic_GO/internal/wire"
	"github.com/fkwhite/Quic_GO/quicvarint"
)

type MLWDFParam struct {
	Gamma  float64
	GammaW float64
	GammaQ float64
}
type Configuration struct {
	Scheduler_name string
	Weight         []float64
	Penalty        []float64
	GrowingFactor  float64
	MLWDF_Params   []MLWDFParam
}

type framer interface {
	HasData() bool

	QueueControlFrame(wire.Frame)
	AppendControlFrames([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)

	AddActiveStream(protocol.StreamID)
	SchedulerProposalQueuing([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) //CRIS
	SchedulerWFQ([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)             //CRIS
	SchedulerFairQueuing([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)     //CRIS
	SchedulerDelayQueuing([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)
	SchedulerMaxDelayQueuing([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)
	SchedulerSTBPQueuing([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)
	SchedulerMLWDF([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)
	AppendStreamFrames([]ackhandler.Frame, protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount)

	Handle0RTTRejection() error

	AdvancedOutgoingStreamManagement
}

type framerI struct {
	mutex sync.Mutex

	streamGetter streamGetter
	version      protocol.VersionNumber

	activeStreams map[protocol.StreamID]struct{}
	streamQueue   []protocol.StreamID

	controlFrameMutex sync.Mutex
	controlFrames     []wire.Frame
}

var _ framer = &framerI{}

func newFramer(
	streamGetter streamGetter,
	v protocol.VersionNumber,
) framer {
	return &framerI{
		streamGetter:  streamGetter,
		activeStreams: make(map[protocol.StreamID]struct{}),
		version:       v,
	}
}

func (f *framerI) HasData() bool {
	f.mutex.Lock()
	hasData := len(f.streamQueue) > 0
	f.mutex.Unlock()
	if hasData {
		return true
	}
	f.controlFrameMutex.Lock()
	hasData = len(f.controlFrames) > 0
	f.controlFrameMutex.Unlock()
	return hasData
}

func (f *framerI) QueueControlFrame(frame wire.Frame) {
	f.controlFrameMutex.Lock()
	f.controlFrames = append(f.controlFrames, frame)
	f.controlFrameMutex.Unlock()
}

func (f *framerI) AppendControlFrames(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	f.controlFrameMutex.Lock()
	for len(f.controlFrames) > 0 {
		frame := f.controlFrames[len(f.controlFrames)-1]
		frameLen := frame.Length(f.version)
		if length+frameLen > maxLen {
			break
		}
		frames = append(frames, ackhandler.Frame{Frame: frame})
		length += frameLen
		f.controlFrames = f.controlFrames[:len(f.controlFrames)-1]
	}
	f.controlFrameMutex.Unlock()
	return frames, length
}

func (f *framerI) AddActiveStream(id protocol.StreamID) {
	f.mutex.Lock()
	if _, ok := f.activeStreams[id]; !ok {
		f.streamQueue = append(f.streamQueue, id)
		f.activeStreams[id] = struct{}{}
	}
	f.mutex.Unlock()
}

// Added --> escribir fichero
// TIMESTAMP | ID STREAM | BUFFER STREAM (total) | LENGTH (frame.Length(f.version))
func writeFile(scheduler string, timestamp int64, ID protocol.StreamID, buffer protocol.ByteCount, length protocol.ByteCount) {
	nameFile := fmt.Sprint("tmp/logScheduler", scheduler, ".log")            //logSchedulerXX.log
	f, err := os.OpenFile(nameFile, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666) //os.O_TRUNC
	if err != nil {
		fmt.Println("An error has ocurred -- Opening file")
		panic(err)
	}
	defer f.Close()

	logMessage := fmt.Sprint(timestamp, "	", ID, "	", buffer, "	", length, "\n")
	//fmt.Println(logMessage) //por pantalla
	_, err = f.WriteString(logMessage) //fichero log
	if err != nil {
		fmt.Println("An error has ocurred -- Writing file")
		panic(err)
	}
}

// Added
func Min(array []protocol.ByteCount) int {
	var min protocol.ByteCount = array[0]
	var id_stream int

	for id, value := range array {
		if min > value {
			min = value
			id_stream = id
		}
	}
	return id_stream
}

// TODO: scheduler --> Proposal Queuing
func (f *framerI) SchedulerProposalQueuing(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame

	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	// Whoever has the highest penalty sends (as much maxlen as possible).
	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
	r := make([]protocol.ByteCount, numActiveStreams)
	var sum_r protocol.ByteCount
	for i := 0; i < numActiveStreams; i++ {
		id := f.streamQueue[i]
		str, _ := f.streamGetter.GetOrOpenSendStream(id)
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
		//fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
		// Obtaining sojourn time from slices of data

		GlobalBuffersSojournTimeLog("Proposal", int(id))
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
		r[i] = totalTX
		sum_r += r[i]
	}

	penalty := make([]protocol.ByteCount, numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		penalty[i] = protocol.ByteCount(configuration.Penalty[i]) - r[i]
	}

	for i := 0; i < numActiveStreams; i++ {
		for j := 0; j < numActiveStreams; j++ {
			if penalty[i] < penalty[j] {
				aux := penalty[j]
				penalty[j] = penalty[i]
				penalty[i] = aux
				aux2 := f.streamQueue[j]
				f.streamQueue[j] = f.streamQueue[i]
				f.streamQueue[i] = aux2
			}
		}
	}

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}
		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		f.streamQueue = append(f.streamQueue, id)
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length
		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).
		remainingLen += quicvarint.Len(uint64(remainingLen))
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTX := str.TotalQueue() + buffBytes
		frame, _ := str.popStreamFrame(remainingLen)
		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("Proposal", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame
	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// TODO: scheduler --> Delay Queuing
func (f *framerI) SchedulerDelayQueuing(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame

	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	// Whoever has the highest penalty sends (as much maxlen as possible).
	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
	r := make([]protocol.ByteCount, numActiveStreams)
	var sum_r protocol.ByteCount
	var sum = make([]int64, numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		id := f.streamQueue[i]
		str, _ := f.streamGetter.GetOrOpenSendStream(id)

		sum[i] = GlobalBuffersTotalDelay(int(id / 4))
		timestamp := time.Now().UnixMicro()

		// Obtaining sojourn time from slices of data
		sumaux, num_data := GlobalBuffersSojournTime(int(id / 4))
		if num_data != 0 {
			sumaux = int64(num_data)*timestamp - sumaux
		} else {
			sumaux = 0
		}
		GlobalBuffersSojournTimeLog("Delay", int(id))
		//fmt.Println("Retardo acumulado del id ", f.streamQueue[i], " es: ", sum[i])

		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
		// fmt.Println("Bytes en el buffer de id ", i, ": ", buffBytes)
		// fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
		r[i] = totalTX
		sum_r += r[i]
	}

	//fmt.Println("Streams Activos: ", numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		for j := 0; j < numActiveStreams; j++ {
			if int64(sum[i]) > int64(sum[j]) {
				aux := sum[j]
				sum[j] = sum[i]
				sum[i] = aux
				aux2 := f.streamQueue[j]
				f.streamQueue[j] = f.streamQueue[i]
				f.streamQueue[i] = aux2
			}
		}
	}

	// for i := 0; i < numActiveStreams; i++ {
	// 	fmt.Printf("%d %i ", sum[i], f.streamQueue[i])
	// }
	// fmt.Printf("\n")

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		f.streamQueue = append(f.streamQueue, id)
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length
		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).

		remainingLen += quicvarint.Len(uint64(remainingLen)) // value que varia en función del pkt
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTX := str.TotalQueue() + buffBytes
		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

		frame, _ := str.popStreamFrame(remainingLen)

		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("Delay", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame

	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// TODO: scheduler --> Delay Queuing
func (f *framerI) SchedulerSTBPQueuing(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame
	var num_data int
	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	// Whoever has the highest penalty sends (as much maxlen as possible).
	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
	r := make([]protocol.ByteCount, numActiveStreams)
	var sum_r protocol.ByteCount
	var sum = make([]int64, numActiveStreams)

	for i := 0; i < numActiveStreams; i++ {
		id := f.streamQueue[i]
		str, _ := f.streamGetter.GetOrOpenSendStream(id)

		// Obtaining sojourn time from slices of data
		sum[i], num_data = GlobalBuffersSojournTime(int(id / 4))
		timestamp := time.Now().UnixMicro()
		if num_data != 0 {
			sum[i] = int64(num_data)*timestamp - sum[i]
		} else {
			sum[i] = 0
		}
		GlobalBuffersSojournTimeLog("STBP", int(id)) // storing arrival times
		//fmt.Println("Retardo acumulado del id ", f.streamQueue[i], " es: ", sum[i])

		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
		// fmt.Println("Bytes en el buffer de id ", i, ": ", buffBytes)
		// fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
		r[i] = totalTX
		sum_r += r[i]
	}

	//fmt.Println("Streams Activos: ", numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		for j := 0; j < numActiveStreams; j++ {
			if int64(sum[i]) > int64(sum[j]) {
				aux := sum[j]
				sum[j] = sum[i]
				sum[i] = aux
				aux2 := f.streamQueue[j]
				f.streamQueue[j] = f.streamQueue[i]
				f.streamQueue[i] = aux2
			}
		}
	}

	// for i := 0; i < numActiveStreams; i++ {
	// 	fmt.Printf("%d %i ", sum[i], f.streamQueue[i])
	// }
	// fmt.Printf("\n")

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		f.streamQueue = append(f.streamQueue, id)
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length
		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).

		remainingLen += quicvarint.Len(uint64(remainingLen)) // value que varia en función del pkt
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTX := str.TotalQueue() + buffBytes
		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

		frame, _ := str.popStreamFrame(remainingLen)

		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("STBP", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame

	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// TODO: scheduler --> Delay Queuing
func (f *framerI) SchedulerMaxDelayQueuing(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame

	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	// Whoever has the highest penalty sends (as much maxlen as possible).
	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
	r := make([]protocol.ByteCount, numActiveStreams)
	var sum_r protocol.ByteCount
	var delay = make([]int64, numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		id := f.streamQueue[i]
		str, _ := f.streamGetter.GetOrOpenSendStream(id)

		firstpktFIFO := GlobalBuffersPktDelay(int(id / 4))
		timestamp := time.Now().UnixMicro()
		if firstpktFIFO != 0 {
			delay[i] = timestamp - firstpktFIFO
		} else {
			delay[i] = 0
		}

		// Obtaining sojourn time from slices of data
		sumaux, num_data := GlobalBuffersSojournTime(int(id / 4))
		if num_data != 0 {
			sumaux = int64(num_data)*timestamp - sumaux
		} else {
			sumaux = 0
		}
		GlobalBuffersSojournTimeLog("MaxDelay", int(id))
		//fmt.Println("Retardo acumulado del id ", f.streamQueue[i], " es: ", sum[i])

		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
		// fmt.Println("Bytes en el buffer de id ", i, ": ", buffBytes)
		// fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
		r[i] = totalTX
		sum_r += r[i]
	}

	// for i := 0; i < numActiveStreams; i++ {
	// 	fmt.Printf("Delay of stream %d is %d\n",f.streamQueue[i], delay[i])
	// }

	//fmt.Println("Streams Activos: ", numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		for j := 0; j < numActiveStreams; j++ {
			if int64(delay[i]) > int64(delay[j]) {
				aux := delay[j]
				delay[j] = delay[i]
				delay[i] = aux
				aux2 := f.streamQueue[j]
				f.streamQueue[j] = f.streamQueue[i]
				f.streamQueue[i] = aux2
			}
		}
	}

	// for i := 0; i < numActiveStreams; i++ {
	// 	fmt.Printf("Delay of stream %d is %d\n",f.streamQueue[i], delay[i])
	// }

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		id := f.streamQueue[0]
		// fmt.Printf("Stream %d TX\n",id)
		f.streamQueue = f.streamQueue[1:]
		f.streamQueue = append(f.streamQueue, id)
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length
		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).

		remainingLen += quicvarint.Len(uint64(remainingLen)) // value que varia en función del pkt
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTX := str.TotalQueue() + buffBytes
		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

		frame, _ := str.popStreamFrame(remainingLen)

		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("MaxDelay", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame

	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// func (f *framerI) SchedulerMLWDF(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
// 	var length protocol.ByteCount
// 	var lastFrame *ackhandler.Frame

// 	configFile := "conf_Scheduler.json"
// 	file, err := os.Open(configFile)
// 	if err != nil {
// 		fmt.Println("An error has ocurred -- Opening configuration file")
// 		panic(err)
// 	}
// 	defer file.Close()
// 	decoder := json.NewDecoder(file)
// 	configuration := Configuration{}
// 	err = decoder.Decode(&configuration)
// 	if err != nil {
// 		fmt.Println("error:", err)
// 	}

// 	f.mutex.Lock()

// 	numActiveStreams := len(f.streamQueue)
// 	// Whoever has the highest penalty sends (as much maxlen as possible).
// 	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
// 	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
// 	r := make([]protocol.ByteCount, numActiveStreams)
// 	var sum_r protocol.ByteCount
// 	var delay = make([]int64, numActiveStreams)
// 	var V = make([]float64, numActiveStreams)
// 	for i := 0; i < numActiveStreams; i++ {
// 		id := f.streamQueue[i]
// 		str, _ := f.streamGetter.GetOrOpenSendStream(id)

// 		mindelay := GlobalBuffersPktDelay(int(id / 4))
// 		if mindelay != 0 {
// 			delay[i] = mindelay
// 		} else {
// 			delay[i] = 0
// 		}

// 		GlobalBuffersSojournTimeLog("MLWDF", int(id/4))
// 		//fmt.Println("Retardo acumulado del id ", f.streamQueue[i], " es: ", sum[i])

// 		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
// 		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
// 		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
// 		// fmt.Println("Bytes en el buffer de id ", i, ": ", buffBytes)
// 		// fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
// 		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
// 		r[i] = totalTX
// 		sum_r += r[i]
// 		V[i] = configuration.MLWDF_Params[i].Gamma * math.Pow(
// 			float64(configuration.MLWDF_Params[i].GammaW)*float64(delay[i])+
// 				float64(configuration.MLWDF_Params[i].GammaQ)*float64(r[i]),
// 			float64(configuration.GrowingFactor))

// 	}

// 	// for i := 0; i < numActiveStreams; i++ {
// 	// 	fmt.Printf("Delay of stream %d is %d\n",f.streamQueue[i], delay[i])
// 	// }

// 	//fmt.Println("Streams Activos: ", numActiveStreams)
// 	for i := 0; i < numActiveStreams; i++ {
// 		for j := 0; j < numActiveStreams; j++ {
// 			if int64(V[i]) > int64(V[j]) {
// 				aux := V[j]
// 				V[j] = V[i]
// 				V[i] = aux
// 				aux2 := f.streamQueue[j]
// 				f.streamQueue[j] = f.streamQueue[i]
// 				f.streamQueue[i] = aux2
// 			}
// 		}
// 	}

// 	// sort.Slice(f.streamQueue, func(i, j int) bool { return V[i] > V[j] })
// 	fmt.Printf("MLWDF Scheduling - Active Streams: %d\n", numActiveStreams)
// 	for i := 0; i < numActiveStreams; i++ {
// 		fmt.Printf("- Stream info %d with V = %.1f, W = %d and Q = %d\n", f.streamQueue[i], V[i], delay[i], r[i])
// 	}

// 	for i := 0; i < numActiveStreams; i++ {
// 		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
// 		if protocol.MinStreamFrameSize+length > maxLen {
// 			break
// 		}

// 		id := f.streamQueue[0]
// 		// fmt.Printf("Stream %d TX\n",id)
// 		f.streamQueue = f.streamQueue[1:]
// 		f.streamQueue = append(f.streamQueue, id)
// 		// This should never return an error. Better check it anyway.
// 		// The stream will only be in the streamQueue, if it enqueued itself there.
// 		str, err := f.streamGetter.GetOrOpenSendStream(id)
// 		// The stream can be nil if it completed after it said it had data.
// 		if str == nil || err != nil {
// 			delete(f.activeStreams, id)
// 			continue
// 		}
// 		remainingLen := maxLen - length
// 		// For the last STREAM frame, we'll remove the DataLen field later.
// 		// Therefore, we can pretend to have more bytes available when popping
// 		// the STREAM frame (which will always have the DataLen set).

// 		remainingLen += quicvarint.Len(uint64(remainingLen)) // value que varia en función del pkt
// 		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
// 		totalTX := str.TotalQueue() + buffBytes
// 		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

// 		frame, _ := str.popStreamFrame(remainingLen)

// 		// The frame can be nil
// 		// * if the receiveStream was canceled after it said it had data
// 		// * the remaining size doesn't allow us to add another STREAM frame
// 		if frame == nil {
// 			continue
// 		}
// 		timestamp := time.Now().UnixMicro()
// 		writeFile("MLWDF", timestamp, id, totalTX, frame.Length(f.version))
// 		frames = append(frames, *frame)
// 		length += frame.Length(f.version)
// 		lastFrame = frame

// 	}

// 	f.mutex.Unlock()
// 	if lastFrame != nil {
// 		lastFrameLen := lastFrame.Length(f.version)
// 		// account for the smaller size of the last STREAM frame
// 		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
// 		length += lastFrame.Length(f.version) - lastFrameLen
// 	}
// 	return frames, length
// }

// / Version decente de MLWDF
func (f *framerI) SchedulerMLWDF(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame

	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)

	type mlwdfEntry struct {
		id    protocol.StreamID
		V     float64
		delay int64
		queue protocol.ByteCount
	}

	entries := make([]mlwdfEntry, 0, numActiveStreams)
	// Whoever has the highest penalty sends (as much maxlen as possible).
	// If there are not enough bytes in the buffer, they are sent from the next stream with the highest priority.
	// The penalty is based on the number of bytes in the queue (higher number of bytes, higher priority) and the penalties of each stream.
	r := make([]protocol.ByteCount, numActiveStreams)
	var sum_r protocol.ByteCount
	var delay = make([]int64, numActiveStreams)
	var V = make([]float64, numActiveStreams)
	for i := 0; i < numActiveStreams; i++ {
		id := f.streamQueue[i]
		str, _ := f.streamGetter.GetOrOpenSendStream(id)

		mindelay := GlobalBuffersPktDelay(int(id / 4))
		if mindelay != 0 {
			delay[i] = mindelay
		} else {
			delay[i] = 0
		}

		GlobalBuffersSojournTimeLog("MLWDF", int(id))
		//fmt.Println("Retardo acumulado del id ", f.streamQueue[i], " es: ", sum[i])

		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("Bytes para enviar ", str.RemainingBytes(), "\n")
		// fmt.Println("Bytes para RTX ", str.BytesToRetransmit(), "\n")
		// fmt.Println("Bytes en el buffer de id ", i, ": ", buffBytes)
		// fmt.Println("Bytes para Next Stream ", str.nextFrame.DataLen(), "\n")
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame   + buffer Real (app)
		r[i] = totalTX
		sum_r += r[i]

		V[i] = float64(configuration.MLWDF_Params[int(id/4)].Gamma) * math.Pow(
			float64(configuration.MLWDF_Params[int(id/4)].GammaW)*float64(delay[i])+
				float64(configuration.MLWDF_Params[int(id/4)].GammaQ)*float64(r[i]),
			float64(configuration.GrowingFactor))
		// fmt.Println("Index ", i, "numstream ", int(id), " V[i] ", V[i], " buffBytes ", totalTX, "\n")
		entries = append(entries, mlwdfEntry{
			id:    id,
			V:     V[i],
			delay: delay[i],
			queue: r[i],
		})

	}

	sort.Slice(entries, func(i, j int) bool {
		return entries[i].V > entries[j].V
	})
	// fmt.Printf("MLWDF Scheduling - Active Streams: %d\n", len(entries))
	// for _, e := range entries {
	// 	str, _ := f.streamGetter.GetOrOpenSendStream(e.id)
	// 	fmt.Printf(
	// 		"- Stream %d | V=%.3f | W=%d us | Q=%d B | StreamQ %d\n",
	// 		e.id, e.V, e.delay, e.queue, str.TotalQueue(),
	// 	)
	// }

	for i := 0; i < len(entries); i++ {
		f.streamQueue[i] = entries[i].id
	}

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		id := f.streamQueue[0]
		// fmt.Printf("Stream %d TX\n",id)
		f.streamQueue = f.streamQueue[1:]
		f.streamQueue = append(f.streamQueue, id)
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length
		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).

		remainingLen += quicvarint.Len(uint64(remainingLen)) // value que varia en función del pkt
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTX := str.TotalQueue() + buffBytes
		//fmt.Println("Bytes para enviar ", str.RemainingBytes(), " ", buffBytes)

		frame, _ := str.popStreamFrame(remainingLen)

		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("MLWDF", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame
		// fmt.Println("	>> Selected stream ", id, " len ", frame.Length(f.version), "/", maxLen, " buffbytes ", buffBytes, " StreamQ ", str.TotalQueue(), "\n")
	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// TODO: scheduler --> WFQ
func (f *framerI) SchedulerWFQ(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame

	configFile := "conf_Scheduler.json"
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

	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	weightArray := make([]float64, numActiveStreams)
	weightArray = configuration.Weight

	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		// Sort streams (lowest to highest queue)
		r := make([]protocol.ByteCount, len(f.streamQueue))
		var N float64
		for j := 0; j < len(f.streamQueue); j++ {
			id := f.streamQueue[j]
			str, _ := f.streamGetter.GetOrOpenSendStream(id)
			// buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
			totalTX := str.TotalQueue() // bufferTx + bufferRtx + bufferNextFrame
			r[j] = totalTX
			N += weightArray[j]
			GlobalBuffersSojournTimeLog("WFQ", int(id))
		}
		aux := make([]protocol.StreamID, len(f.streamQueue))
		auxWeight := make([]float64, len(f.streamQueue))
		for j := 0; j < len(f.streamQueue); j++ {
			id := Min(r)
			aux[j] = f.streamQueue[id]
			auxWeight[j] = weightArray[id]
			r[id] = protocol.ByteCount(100000)
		}
		f.streamQueue = aux
		weightArray = auxWeight

		C := maxLen - length // remaining bytes
		bytesToDistribute := protocol.ByteCount(float64(C) * weightArray[0] / N)

		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		weightArray = weightArray[1:]
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		totalTX := str.TotalQueue() // bufferTx + bufferRtx + bufferNextFrame
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		totalTXReal := totalTX + buffBytes
		remainingLen := bytesToDistribute
		newStreamFrame := true
		var lengthStreamFrameTotal protocol.ByteCount
		var dataLen protocol.ByteCount
		for newStreamFrame {
			if remainingLen < 10 {
				newStreamFrame = false
				continue
			}
			// For the last STREAM frame, we'll remove the DataLen field later.
			// Therefore, we can pretend to have more bytes available when popping
			// the STREAM frame (which will always have the DataLen set).
			dataLen = quicvarint.Len(uint64(remainingLen - 8))
			frame, _ := str.popStreamFrame(remainingLen + dataLen)
			// The frame can be nil
			// * if the receiveStream was canceled after it said it had data
			// * the remaining size doesn't allow us to add another STREAM frame
			if frame == nil {
				newStreamFrame = false
				continue //continue
			}
			frames = append(frames, *frame)
			lengthStreamFrame := frame.Length(f.version)
			lengthStreamFrameTotal += lengthStreamFrame
			length += lengthStreamFrame
			lastFrame = frame
			if lengthStreamFrame >= remainingLen {
				newStreamFrame = false
			}
			remainingLen -= lengthStreamFrame
			if totalTX-lengthStreamFrameTotal <= 0 {
				newStreamFrame = false
			}
		}
		timestamp := time.Now().UnixMicro()
		writeFile("WFQ", timestamp, id, totalTXReal, lengthStreamFrameTotal)
	}
	// Re-activate streams
	for i := 0; i < numActiveStreams; i++ {
		id := protocol.StreamID(i * 4)
		f.streamQueue = append(f.streamQueue, id)
	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// TODO: scheduler --> FairQueuing
func (f *framerI) SchedulerFairQueuing(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame
	f.mutex.Lock()

	numActiveStreams := len(f.streamQueue)
	// fmt.Println("FQ=        Numactives Stream: ", numActiveStreams)
	var excess protocol.ByteCount
	for i := 0; i < numActiveStreams; i++ {
		// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}

		// Sort streams (lowest to highest queue)
		r := make([]protocol.ByteCount, len(f.streamQueue))
		for j := 0; j < len(f.streamQueue); j++ {
			id := f.streamQueue[j]
			str, _ := f.streamGetter.GetOrOpenSendStream(id)
			// buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
			totalTX := str.TotalQueue() // bufferTx + bufferRtx + bufferNextFrame
			r[j] = totalTX

			GlobalBuffersSojournTimeLog("FQ", int(id))
		}

		aux := make([]protocol.StreamID, len(f.streamQueue))
		for j := 0; j < len(f.streamQueue); j++ {
			id := Min(r)
			aux[j] = f.streamQueue[id]
			r[id] = protocol.ByteCount(100000)
		}
		f.streamQueue = aux

		C := maxLen
		N := float64(numActiveStreams)
		bytesAdded := float64(excess) / float64(len(f.streamQueue))

		// Round off
		bytesCN := float64(C) / N
		modulus := float64(C % protocol.ByteCount(N))
		if modulus != 0 {
			if float64(len(f.streamQueue))-1.0 < modulus {
				bytesCN = math.Ceil(bytesCN)
			} else {
				bytesCN = math.Floor(bytesCN)
			}
		}
		bytesToDistribute := protocol.ByteCount(bytesCN + bytesAdded)
		excess -= protocol.ByteCount(bytesAdded)

		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}

		totalTX := str.TotalQueue() // bufferTx + bufferRtx + bufferNextFrame
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))
		// fmt.Println("+++FQ=    Bytes Buffer en el stream ", id, ":  ", buffBytes)
		totalTXReal := totalTX + buffBytes
		if totalTX == 0 {
			//no bytes to send
			excess += bytesToDistribute
			continue
		}
		remainingLen := bytesToDistribute
		// fmt.Println("++ STREAM ID: ", id, "   Bytes max frame: ", maxLen, "   Bytes buffer: ", totalTX, "    Bytes para enviar: ", bytesToDistribute)
		newStreamFrame := true
		var lengthStreamFrameTotal protocol.ByteCount
		var dataLen protocol.ByteCount
		for newStreamFrame {
			if remainingLen < 10 { //10: too small
				newStreamFrame = false
				continue
			}
			// For the last STREAM frame, we'll remove the DataLen field later.
			// Therefore, we can pretend to have more bytes available when popping
			// the STREAM frame (which will always have the DataLen set).

			// typeStream := 1 //always 1 byte
			// streamID := quicvarint.Len(uint64(id))
			// offset := 8 //max bytes offset head (cannot be known beforehand)
			// dataLen = quicvarint.Len(uint64(remainingLen-protocol.ByteCount(typeStream)-protocol.ByteCount(streamID)-protocol.ByteCount(offset))) //length data application (only)
			// frame, _ := str.popStreamFrame(remainingLen+protocol.ByteCount(dataLen))

			dataLen = quicvarint.Len(uint64(remainingLen - 8)) // 8: max bytes head
			frame, _ := str.popStreamFrame(remainingLen + dataLen)
			// The frame can be nil
			// * if the receiveStream was canceled after it said it had data
			// * the remaining size doesn't allow us to add another STREAM frame
			if frame == nil {
				newStreamFrame = false
				continue
			}
			frames = append(frames, *frame)
			lengthStreamFrame := frame.Length(f.version)
			lengthStreamFrameTotal += lengthStreamFrame
			length += lengthStreamFrame
			lastFrame = frame
			if lengthStreamFrame >= remainingLen {
				newStreamFrame = false
			}
			remainingLen -= lengthStreamFrame
			if totalTX-lengthStreamFrameTotal <= 0 {
				newStreamFrame = false
			}
		}
		timestamp := time.Now().UnixMicro()
		writeFile("FQ", timestamp, id, totalTXReal, lengthStreamFrameTotal)
		// fmt.Println("ID: ", id, " Bytes Buffer: ", totalTX, " Bytes Enviados: ", lengthStreamFrameTotal)
		if totalTX <= bytesToDistribute {
			excess += (bytesToDistribute - lengthStreamFrameTotal)
		} else {
			excess -= dataLen
		}
	}
	// Re-activate streams
	for i := 0; i < numActiveStreams; i++ {
		id := protocol.StreamID(i * 4)
		f.streamQueue = append(f.streamQueue, id)
	}

	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

// based on Round-Robin
func (f *framerI) AppendStreamFrames(frames []ackhandler.Frame, maxLen protocol.ByteCount) ([]ackhandler.Frame, protocol.ByteCount) {
	var length protocol.ByteCount
	var lastFrame *ackhandler.Frame
	f.mutex.Lock()
	// pop STREAM frames, until less than MinStreamFrameSize bytes are left in the packet
	numActiveStreams := len(f.streamQueue)

	for i := 0; i < numActiveStreams; i++ {
		if protocol.MinStreamFrameSize+length > maxLen {
			break
		}
		id := f.streamQueue[0]
		f.streamQueue = f.streamQueue[1:]
		// This should never return an error. Better check it anyway.
		// The stream will only be in the streamQueue, if it enqueued itself there.
		str, err := f.streamGetter.GetOrOpenSendStream(id)
		// The stream can be nil if it completed after it said it had data.
		if str == nil || err != nil {
			delete(f.activeStreams, id)
			continue
		}
		remainingLen := maxLen - length

		// For the last STREAM frame, we'll remove the DataLen field later.
		// Therefore, we can pretend to have more bytes available when popping
		// the STREAM frame (which will always have the DataLen set).
		remainingLen += quicvarint.Len(uint64(remainingLen))
		buffBytes := protocol.ByteCount(GlobalBuffersRead(int(id / 4)))

		GlobalBuffersSojournTimeLog("RR", int(id))
		totalTX := str.TotalQueue() + buffBytes // bufferTx + bufferRtx + bufferNextFrame
		// fmt.Println("+++RR=    Bytes Buffer en el stream ", id, ":  ", buffBytes)
		frame, hasMoreData := str.popStreamFrame(remainingLen)
		if hasMoreData { // put the stream back in the queue (at the end)
			f.streamQueue = append(f.streamQueue, id)
		} else { // no more data to send. Stream is not active any more
			delete(f.activeStreams, id)
		}
		// The frame can be nil
		// * if the receiveStream was canceled after it said it had data
		// * the remaining size doesn't allow us to add another STREAM frame
		if frame == nil {
			continue
		}
		timestamp := time.Now().UnixMicro()
		writeFile("RR", timestamp, id, totalTX, frame.Length(f.version))
		frames = append(frames, *frame)
		length += frame.Length(f.version)
		lastFrame = frame
	}
	f.mutex.Unlock()
	if lastFrame != nil {
		lastFrameLen := lastFrame.Length(f.version)
		// account for the smaller size of the last STREAM frame
		lastFrame.Frame.(*wire.StreamFrame).DataLenPresent = false
		length += lastFrame.Length(f.version) - lastFrameLen
	}
	return frames, length
}

func (f *framerI) Handle0RTTRejection() error {
	f.mutex.Lock()
	defer f.mutex.Unlock()

	f.controlFrameMutex.Lock()
	f.streamQueue = f.streamQueue[:0]
	for id := range f.activeStreams {
		delete(f.activeStreams, id)
	}
	var j int
	for i, frame := range f.controlFrames {
		switch frame.(type) {
		case *wire.MaxDataFrame, *wire.MaxStreamDataFrame, *wire.MaxStreamsFrame:
			return errors.New("didn't expect MAX_DATA / MAX_STREAM_DATA / MAX_STREAMS frame to be sent in 0-RTT")
		case *wire.DataBlockedFrame, *wire.StreamDataBlockedFrame, *wire.StreamsBlockedFrame:
			continue
		default:
			f.controlFrames[j] = f.controlFrames[i]
			j++
		}
	}
	f.controlFrames = f.controlFrames[:j]
	f.controlFrameMutex.Unlock()
	return nil
}

// HasAppData checks if there is more application data to be transmitted
func (f *framerI) HasAppData() bool {
	return f.streamGetter.HasAppData()
}

// SentBytes returns the number of sent bytes without counting the retransmissions
func (f *framerI) SentBytes() protocol.ByteCount {
	return f.streamGetter.SentBytes()
}

// RemainingBytes returns current length of the application data not transmitted yet
func (f *framerI) RemainingBytes() protocol.ByteCount {
	return f.streamGetter.RemainingBytes()
}

// HasRetransmission checks if any data frame has been enqueued for retransmission
func (f *framerI) HasRetransmission() bool {
	return f.streamGetter.HasRetransmission()
}

// TotalQueue returns current length of the application data not transmitted + retransmitted
// If there are bytes in the next frame, it adds it up and returns it
func (f *framerI) TotalQueue() protocol.ByteCount {
	return f.streamGetter.TotalQueue()
}

// FramesToRetransmit returns the number of frames enqueued for retransmission
func (f *framerI) FramesToRetransmit() int {
	return f.streamGetter.FramesToRetransmit()
}

// BytesToRetransmit returns the number of data bytes excluding frame headers
func (f *framerI) BytesToRetransmit() protocol.ByteCount {
	return f.streamGetter.BytesToRetransmit()
}
