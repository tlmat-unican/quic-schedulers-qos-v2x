#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wave-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/bridge-module.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/csma-module.h"

using namespace ns3;


void PhyRxTrace (std::string context,
                 Ptr<const Packet> packet,
                 uint16_t channelFreqMhz,
                 WifiTxVector txVector,
                 MpduInfo aMpdu,
                 SignalNoiseDbm signalNoise,
                 uint16_t staId)
{
    std::cout << Simulator::Now ().GetSeconds ()
              << "s RX: freq=" << channelFreqMhz << " MHz"
              << " Mode=" << txVector.GetMode ()
              << " TxPower=" << txVector.GetTxPowerLevel ()
              << " SNR=" << (signalNoise.signal - signalNoise.noise) << " dB"
              << " PacketSize=" << packet->GetSize () << " bytes"
              << std::endl;
}

void TxOkTrace (std::string context, Ptr<const Packet> packet)
{
    std::cout << Simulator::Now ().GetSeconds () 
              << "s TX-OK: PacketSize=" << packet->GetSize () << " bytes" 
              << std::endl;
}

void TxErrTrace (std::string context, Ptr<const Packet> packet)
{
    std::cout << Simulator::Now ().GetSeconds () 
              << "s TX-ERR: Packet lost" << std::endl;
}


int main(int argc, char *argv[])
{
    
    bool trafficmode = false; // false: synthetic, true: real
    CommandLine cmd;
    cmd.Parse(argc, argv);
    cmd.AddValue("traffic-mode", "Traffic mode: real or synthetic", trafficmode);


    if (trafficmode) {
        GlobalValue::Bind("SimulatorImplementationType", StringValue("ns3::RealtimeSimulatorImpl"));
        GlobalValue::Bind("ChecksumEnabled", BooleanValue(true));
    }
    
    Time::SetResolution(Time::NS);
    std::string phyMode("OfdmRate27MbpsBW10MHz");
    
    
    // LogComponentEnable("YansErrorRateModel", LOG_LEVEL_ALL);
    LogComponentEnable("WaveHelper", LOG_LEVEL_DEBUG);
    // LogComponentEnable("YansWifiPhy", LOG_LEVEL_INFO);
    // LogComponentEnable("YansErrorRateModel", LOG_LEVEL_INFO);

    // Crear nodos
    NodeContainer vehicles;
    vehicles.Create(2);

    NodeContainer DockerToNs3;
    DockerToNs3.Create(2);
    
    // Configurar movilidad
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(1.0, 0.0, 0.0));
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(vehicles);

    // Configurar PHY y MAC
    YansWavePhyHelper wifiPhy;
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    Ptr<YansWifiChannel> channel = wifiChannel.Create();
    wifiPhy.SetChannel(channel);
    wifiPhy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11);
    wifiPhy.Set("Antennas", UintegerValue(2));


    // Ajustar la potencia de transmisión
    wifiPhy.Set("TxPowerStart", DoubleValue(20.0)); // Potencia mínima en dBm
    wifiPhy.Set("TxPowerEnd", DoubleValue(20.0));   // Potencia máxima en dBm

    NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default();
    Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default();
    wifi80211p.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                       "DataMode", StringValue(phyMode),
                                       "ControlMode", StringValue(phyMode));
    NetDeviceContainer devices = wifi80211p.Install(wifiPhy, wifi80211pMac, vehicles);
   


   



    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Gbps"));
    NetDeviceContainer csmaDevIn = csma.Install(NodeContainer{DockerToNs3.Get(0), vehicles.Get(0)});  // primero nodo externo - Docker
    NetDeviceContainer csmaDevOut = csma.Install(NodeContainer{DockerToNs3.Get(1), vehicles.Get(1)}); // primero nodo externo   

    // std::cout << csmaDevIn.GetN() << std::endl;

    // Instalar stack IP
    InternetStackHelper internet;
    internet.Install(vehicles);
        

    Ipv4AddressHelper addressesCsmaIn;
    addressesCsmaIn.SetBase("10.1.0.0", "255.255.255.0", "0.0.0.3");
    Ipv4InterfaceContainer ifacesCsmaIn = addressesCsmaIn.Assign({csmaDevIn.Get(1)});
    // Ipv4InterfaceContainer ifacesCsmaIn = addressesCsmaIn.Assign({csmaDevIn});

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("11.3.0.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    Ipv4AddressHelper addressesCsmaOut;
    addressesCsmaOut.SetBase("10.2.0.0", "255.255.255.0","0.0.0.3");
    Ipv4InterfaceContainer ifacesCsmaOut = addressesCsmaOut.Assign({csmaDevOut.Get(1)});
  
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

   

    // // Configurar TapBridge
    TapBridgeHelper tapBridge;
    tapBridge.SetAttribute("Mode", StringValue("UseBridge"));  

    tapBridge.SetAttribute("DeviceName", StringValue("tap-left"));
    tapBridge.Install(DockerToNs3.Get(0), csmaDevIn.Get(0));

    tapBridge.SetAttribute("DeviceName", StringValue("tap-right"));
    tapBridge.Install(DockerToNs3.Get(1), csmaDevOut.Get(0));

    
    // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferRx",
    //              MakeCallback (&PhyRxTrace));


    // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTxOk", 
    //                 MakeCallback (&TxOkTrace));

    // Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTxDrop", 
    //                 MakeCallback (&TxErrTrace));

    std::cout << "NS-3 simulation starting..." << std::endl;
    Simulator::Run();
    std::cout << "NS-3 simulation finished." << std::endl;
    Simulator::Destroy();
}