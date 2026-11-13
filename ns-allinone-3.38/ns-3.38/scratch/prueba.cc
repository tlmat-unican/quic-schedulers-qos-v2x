/*
This scenario just connect the containers through a simple point-2-point
link.

For testing delay has been added to CSMA and point-to-point links.
*/

#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TapCsmaDocker");

int main(int argc, char *argv[])
{
  Time::SetResolution(Time::FS);
  // LogComponentEnable("TapBridge", LOG_LEVEL_ALL); 
  double rate_mbps = 27;
  double delay_ms = 0;
  double error_rate = 0.0;
  uint32_t seed = 12345;
  uint32_t run = 1;
  CommandLine cmd(__FILE__);
  cmd.AddValue("data-rate", "Data rate of the link in Mbps", rate_mbps);
  cmd.AddValue("delay", "One way delay of the link in ms", delay_ms);
  cmd.AddValue("error-rate", "Packet error rate of the link.", error_rate);
  cmd.AddValue("seed", "RNG seed", seed);
  cmd.AddValue("run", "RNG run", run);
  cmd.Parse(argc, argv);
  SeedManager::SetSeed(seed);
  SeedManager::SetRun(run);
  // LogComponentEnable("TapBridge", LOG_LEVEL_INFO);
  std::string mode = "ConfigureLocal";
  std::string tapName = "ns3tap";

  GlobalValue::Bind("SimulatorImplementationType", StringValue("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind("ChecksumEnabled", BooleanValue(true));

  NodeContainer nodes;
  nodes.Create(4);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(std::to_string(rate_mbps) + "Mbps")); // BW
  pointToPoint.SetChannelAttribute("Delay", StringValue(std::to_string(delay_ms) + "ms"));      // delay
  pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("0p"));
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("10Gbps"));

  NetDeviceContainer csmaDevIn = csma.Install(NodeContainer{nodes.Get(0), nodes.Get(1)});
  NetDeviceContainer p2pDev = pointToPoint.Install(NodeContainer{nodes.Get(1), nodes.Get(2)});
  NetDeviceContainer csmaDevOut = csma.Install(NodeContainer{nodes.Get(3), nodes.Get(2)});

  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
  em->SetAttribute("ErrorRate", DoubleValue(error_rate));
  em->SetAttribute("ErrorUnit", EnumValue(2)); // 0 = ERROR_UNIT_BIT, 1 = ERROR_UNIT_BYTE, 2 = ERROR_UNIT_PACKET (Entire packet will be dropped)
  p2pDev.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));


  InternetStackHelper stack;
  stack.Install(NodeContainer{nodes.Get(1), nodes.Get(2)});

  Ipv4AddressHelper addressesCsmaIn;
  addressesCsmaIn.SetBase("10.1.0.0", "255.255.255.0", "0.0.0.3");
  Ipv4AddressHelper addressesP2p;
  addressesP2p.SetBase("11.0.0.0", "255.255.255.0", "0.0.0.1");
  Ipv4AddressHelper addressesCsmaOut;
  addressesCsmaOut.SetBase("10.2.0.0", "255.255.255.0", "0.0.0.3");

  Ipv4InterfaceContainer ifacesCsmaIn = addressesCsmaIn.Assign({csmaDevIn.Get(1)});
  Ipv4InterfaceContainer ifacesP2p = addressesP2p.Assign(p2pDev);
  Ipv4InterfaceContainer ifacesCsmaOut = addressesCsmaOut.Assign({csmaDevOut.Get(1)});
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  {
    TapBridgeHelper tapBridge1;
    tapBridge1.SetAttribute("Mode", StringValue("UseBridge"));
    tapBridge1.SetAttribute("DeviceName", StringValue("tap-left"));
    tapBridge1.Install(nodes.Get(0), csmaDevIn.Get(0));
  }
  {
    TapBridgeHelper tapBridge2;
    tapBridge2.SetAttribute("Mode", StringValue("UseBridge"));
    tapBridge2.SetAttribute("DeviceName", StringValue("tap-right"));
    tapBridge2.Install(nodes.Get(3), csmaDevOut.Get(0));
  }
 

  std::cout << "NS-3 simulation starting..." << std::endl;
  Simulator::Run();
  std::cout << "NS-3 simulation finished." << std::endl;
  Simulator::Destroy();


}