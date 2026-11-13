#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wave-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/error-model.h"

using namespace ns3;

uint32_t lastCwnd = 0;
std::ofstream cwndFile;
std::ofstream txFile;
std::ofstream rxFile;

// Callback para trazar paquetes enviados
void TxTracer(Ptr<const Packet> packet)
{
    txFile  << Simulator::Now().GetNanoSeconds() << "\t" << packet->GetSize() << std::endl;
}

// Callback para trazar paquetes recibidos
void RxTracer(Ptr<const Packet> packet, const Address &address)
{
    rxFile << Simulator::Now().GetNanoSeconds() << "\t" << packet->GetSize() << std::endl;
}

// Callback para el trace de CWND
void CwndTracer(uint32_t oldCwnd, uint32_t newCwnd)
{
    lastCwnd = newCwnd;
    cwndFile << Simulator::Now().GetSeconds() << "\t" << newCwnd << std::endl;
}

// Función periódica para imprimir CWND
void PrintCwndPeriodically()
{
    Simulator::Schedule(Seconds(0.1), &PrintCwndPeriodically);
}

int main(int argc, char *argv[])
{
    Time::SetResolution(Time::NS);
    std::string phyMode("OfdmRate27MbpsBW10MHz");

    cwndFile.open("cwnd_trace.txt");
    cwndFile << "Time_s\tCWND_bytes" << std::endl;

    txFile.open("tx_trace.csv");
    rxFile.open("rx_trace.csv");

    LogComponentEnable("YansWifiChannel", LOG_LEVEL_INFO);
    LogComponentEnable("YansWifiPhy", LOG_LEVEL_INFO);
    LogComponentEnable("YansErrorRateModel", LOG_LEVEL_INFO);

    // TCP configuration
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));

    // Crear nodos
    NodeContainer vehicles;
    vehicles.Create(2);

    // Configurar movilidad
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(1.0, 0.0, 0.0)); // distancia aumentada a 50 m
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(vehicles);

    // Configurar PHY y MAC
    YansWifiPhyHelper wifiPhy;
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel"); // fading aleatorio
    wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel"); // pérdida por distancia
    Ptr<YansWifiChannel> channel = wifiChannel.Create();
    wifiPhy.SetChannel(channel);
    wifiPhy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);
    wifiPhy.SetErrorRateModel("ns3::YansErrorRateModel");
    // wifiPhy.Set("RxNoiseFigure", DoubleValue(10.0)); // aumenta ruido para degradar SNR

    NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default();
    Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default();
    wifi80211p.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                       "DataMode", StringValue(phyMode),
                                       "ControlMode", StringValue(phyMode));
    NetDeviceContainer devices = wifi80211p.Install(wifiPhy, wifi80211pMac, vehicles);



    // Instalar stack IP
    InternetStackHelper internet;
    internet.Install(vehicles);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.2.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    uint16_t port = 50000;

    // Aplicación OnOff TCP
    OnOffHelper onoff("ns3::TcpSocketFactory", InetSocketAddress(interfaces.GetAddress(1), port));
    onoff.SetAttribute("DataRate", StringValue("30Mbps"));
    onoff.SetAttribute("PacketSize", UintegerValue(1000));

    ApplicationContainer apps = onoff.Install(vehicles.Get(0));
    apps.Start(Seconds(1.0));

    // Aplicación PacketSink TCP
    PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer sinkApps = sink.Install(vehicles.Get(1));
    sinkApps.Start(Seconds(0.0));

    // Conectar TxTracer y RxTracer
    Ptr<OnOffApplication> onOffApp = DynamicCast<OnOffApplication>(vehicles.Get(0)->GetApplication(0));
    onOffApp->TraceConnectWithoutContext("Tx", MakeCallback(&TxTracer));

    Ptr<PacketSink> packetSink = DynamicCast<PacketSink>(vehicles.Get(1)->GetApplication(0));
    packetSink->TraceConnectWithoutContext("Rx", MakeCallback(&RxTracer));

    // Conectar trace de CWND
    Simulator::Schedule(Seconds(1.01), [=]() {
        Ptr<OnOffApplication> app = DynamicCast<OnOffApplication>(vehicles.Get(0)->GetApplication(0));
        Ptr<Socket> tcpSocket = app->GetSocket();
        if (tcpSocket)
        {
            tcpSocket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndTracer));
            std::cout << "Tracer conectado correctamente!" << std::endl;
            PrintCwndPeriodically();
        }
        else
        {
            std::cout << "No se pudo obtener el socket" << std::endl;
        }
    });

    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
