# 3GPP Evaluation

This folder collects the material used to evaluate the 3GPP/C-V2X setup in this project.

## Testbed
The implementation was validated with:

- OAI as the RAN/gNB stack.
- Open5GS as the 5G Core.


## Hardware Used
- 3 × Intel NUCs for the gNB, core, and UE side.
- Quectel RM520N-GL modem.
- USRP B210 radio platform.
- RCDAT-6000-110 programmable attenuator.

## Notes

This environment is intended as a reference for reproducing the experimental setup used in the project. If you adapt it to a different deployment, document the hardware, software versions, and network parameters you used. In this particular case, the client was running at the top of the open5gs and the server at the quectel UE side.


# UE Connection Setup

This guide describes the basic steps required to connect a User Equipment (UE) to an OpenAirInterface (OAI) network using Open5GS.

## Prerequisites

- Open5GS installed and configured.
- OpenAirInterface (OAI) installed and configured.
- A programmable USIM card and card reader.
- A compatible UE.

## Setup Procedure

### 1. Deploy the Open5GS Core Network

Install and configure the Open5GS core network.

### 2. Deploy the OAI RAN

Install and configure the OpenAirInterface Radio Access Network (gNB):

- The [repository to uinstall](https://github.com/OPENAIRINTERFACE/openairinterface5g)
- The [configuration file](00_106PRB_gNB_USRPB210.conf) should be in the [directory](https://github.com/openairinterface/openairinterface5g/tree/develop/targets/PROJECTS/GENERIC-NR-5GC/CONF). 


### 3. Configure the UE Subscription

Create a subscriber profile in the Open5GS WebUI, including the required parameters (e.g., IMSI, authentication key, OP/OPc, and DNN/APN).

Follow the Open5GS [Quickstart Guide](https://open5gs.org/open5gs/docs/guide/01-quickstart/) and refer to the **"Register Subscriber Information"** section for detailed instructions.


### 4. Program the SIM Card

Flash the USIM card with the same credentials configured in the Open5GS subscriber profile.

Follow this [programming guide](https://open-cells.com/index.php/uiccsim-programing/). The required source code (`uicc-v3.3.tgz`) is already provided in this directory.

### 5. Connect the UE

Insert the programmed SIM card into the UE and connect it to the OAI network.

### 6. Verify the Connection

Once the UE has successfully registered with the network, verify that:
- The UE is attached to the 5G network.
- A PDU session has been established.
- The UE has been assigned an IP address.
- Check if end-to-end connectivity is available (e.g., using `ping` or `iperf3`).

## Next Steps

After confirming successful connectivity, you can proceed with your experiments or deploy additional network applications and services.




## Performance Evaluation Artifacts

The following scripts are included to support the performance assessment of the testbed:

- Attenuator controller Python script, used to adjust the channel attenuation during experiments.
- Throughput sweep orchestration script, named `quic_thput_swwep5g.py`, used to automate experiment runs.
- Scheduler comparison script named `exec_code_v15G.py`, used to evaluate and compare scheduler behavior across scenarios.