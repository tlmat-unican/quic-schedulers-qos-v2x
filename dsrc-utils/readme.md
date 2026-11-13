# DSRC Utilities and Experiment Support

This folder contains the scripts, configuration files, and reference materials used to run DSRC-oriented experiments in this project.

The main goal of this directory is to support reproducible performance evaluations for QUIC and transport-layer experiments over a DSRC testbed.

## What is inside

- `exec_code_v1.py` - automation script for local and remote experiment execution.
- `quic_thput_sweep.py` - throughput sweep script for QUIC performance evaluation.
- `quic-apps/` - Go client/server applications and configuration files used for transport experiments.
- `conffiles_obus/` - DSRC device and uCoupler configuration files.
- `apps_confiles/` - application-level configuration files.
- `docs/` - reference material for the embedded/vehicular platform and related tooling.
- `udptrazas/` - UDP trace helpers and lightweight sender/receiver examples.

## Purpose of this folder

The scripts in this folder are used to:

1. Configure experiment parameters such as traffic load, scheduler policy, and stream count.
2. Run QUIC client/server workloads on a DSRC-related setup.
3. Collect congestion-window logs, buffer traces, and per-run outputs.
4. Compare the effect of different schedulers and traffic settings.
5. Keep the experiment flow reproducible across runs and platforms.

## Experiment workflow

The typical workflow is:

1. Prepare the DSRC environment and verify the radio-side configuration.
2. Update the QUIC and scheduler JSON files in `quic-apps/`.
3. Run the throughput or performance script.
4. Execute the client and server, either locally or over SSH depending on the script.
5. Collect the generated `.log` files and congestion-window traces.
6. Post-process the results in notebooks or external analysis scripts.

## Main scripts

### `quic_thput_sweep.py`

This script runs a throughput sweep over a set of experiment parameters.

It typically:

- updates scheduler configuration files,
- adjusts traffic rates and stream sizes,
- launches the QUIC applications,
- stores logs in structured result folders,
- supports repeatable performance comparisons.

### `exec_code_v1.py`

This script automates hybrid experiments where the client runs locally and the server runs on a remote machine.

It performs tasks such as:

- opening an SSH connection to the remote host,
- synchronizing clocks with `ntpdate`,
- cleaning temporary logs,
- updating configuration files,
- launching the server remotely and the client locally,
- collecting logs from both sides.

This is the most useful script when the experiment spans two machines.

## DSRC and hardware configuration

The DSRC-related configuration files are stored in:

- `conffiles_obus/`
- `apps_confiles/`

These folders include device and service configuration files such as uCoupler settings and channel assignments.

The `docs/` folder contains supporting material for the hardware and software platform used in the DSRC setup.

## QUIC applications

The `quic-apps/` directory contains the client and server code used by the DSRC experiments.

See:

- `quic-apps/readme.md`

That document explains how to build the Go applications and which transport modes are supported.

The applications support both QUIC and TCP, and the selected transport is controlled through the JSON configuration files.

## UDP trace helpers

The `udptrazas/` directory contains simple UDP sender and receiver examples.

These helpers are useful when you want to validate the channel, capture basic traces, or test packet forwarding without running the full QUIC stack.

Example usage:

```bash
./udp_sender 192.168.1.30 20001 4444 <filename> <rate_mbps> <pkt_l> -f 1
./udp_receiver 4444 -f <filesize>
```

## Configuration files

The scripts commonly update or consume these files:

- `quic-apps/go_client/conf_Scheduler.json`
- `quic-apps/go_client/conf_Scheduler_tmp.json`
- `quic-apps/go_server/conf_Scheduler.json`
- `quic-apps/go_server/conf_Scheduler_tmp.json`
- `quic-apps/go_client/docker-v2x-*.json`
- `quic-apps/go_server/docker-v2x-*.json`

These files define:

- scheduler name,
- stream parameters,
- transport mode,
- packet size,
- session count,
- traffic distribution,
- rate or load targets.

## Outputs and logs

Depending on the script and experiment, the generated outputs may include:

- congestion window logs,
- per-stream throughput logs,
- temporary execution logs,
- remote server logs,
- result folders organized by scheduler or capacity.

Common output locations are inside:

- `quic-apps/go_client/tmp/`
- `quic-apps/go_client/LOGS_congestionWindow/`
- `quic-apps/go_server/tmp/`

## How this relates to the rest of the project

This folder is the DSRC-oriented counterpart of the containerized workflows used elsewhere in the repository.

- `Dockers/` is used for Docker-based deployments.
- `cv2x-utils/` is used for 3GPP and C-V2X validation.
- `dsrc-utils/` is used for DSRC-focused experiments and trace generation.

Together, these folders allow the project to compare the same QUIC scheduler logic across different environments.


## Related documentation

- [Root README](../README.md)
- [Docker workflow](../Dockers/readme.md)
- [QUIC application README](quic-apps/readme.md)
- [UDP trace helpers](udptrazas/readme.md)

## Contact

Fatima Khan Blanco  
fatima.khan@unican.es
