# Docker-Based Experiment Workflow

This folder contains the Docker orchestration code used to deploy the V2X experiment environment, run the QUIC client and server applications, and connect them with the ns-3 simulation workflow.

The main purpose of this directory is to provide a reproducible execution pipeline for performance experiments, including scheduler comparison, throughput sweeps, and network impairment scenarios.

## What This Folder Does

This directory ties together three pieces of the experiment stack:

- Docker containers that run the QUIC client and server.
- Linux tap devices and Docker bridges that connect the containers to the host network.
- ns-3 simulations used to emulate or control network conditions during the experiments.

In practice, the workflow is:

1. Deploy the Docker environment.
2. Attach tap interfaces to the Docker bridges.
3. Launch ns-3 scenarios when needed.
4. Run the QUIC application inside the containers.
5. Collect logs and results for post-processing.

## Main Files

### `dockergo.py`

Main entry point for starting or stopping the Docker-based environment.

Typical usage:

```bash
python3 dockergo.py deploy
python3 dockergo.py stop
```

- `deploy` starts the containerized environment.
- `stop` tears down the environment.

### `runscen.py`

Starts the Docker infrastructure and prepares the network interfaces.

It currently:

- adjusts system network settings,
- starts the Docker Compose services,
- creates and attaches tap interfaces to the Docker bridges.

### `stopscen.py`

Stops the environment and cleans up the network setup.

This is used when you want to shut down the containers, detach taps, and return the host to a clean state.

### `SetupFunctions.py`

Contains the helper functions used by the orchestration scripts.

Important functions include:

- `RunDockers()` - launches Docker Compose services.
- `CreateAndAttachTaps()` - creates `tap-left` and `tap-right` and attaches them to the bridges.
- `RunNs3()` - launches an ns-3 scenario from the `ns-allinone-3.38/ns-3.38/` tree.
- `ClearDockers()` - stops containers and removes Docker networks.
- `DetachAndRemoveTaps()` - removes tap interfaces and detaches them from the bridges.
- `StopNs3()` - kills a running ns-3 scenario process.

### `docker-compose.yml`

Defines the containerized execution environment.

It launches two containers:

- `sender`
- `receiver`

Each container:

- runs in privileged mode,
- joins its own Docker bridge network,
- mounts the repository into `/home/`,
- applies route changes through `AdaptRouteTable.py`.

The networks are:

- `leftnet` with bridge name `br-left`
- `rightnet` with bridge name `br-right`

The bridge and tap mapping is what allows traffic to flow between the Docker containers and the host-side network setup.

### `MLDW_params_sweep_in_weight_final_iters.py`

Main performance sweep script.

This script automates:

- scheduler selection,
- saturation level sweeps,
- delay and error variation,
- ns-3 execution,
- configuration updates,
- result collection and compression.

It is the script you run after the Docker environment is deployed when you want to generate performance results.

### `results.ipynb`

Notebook used to inspect and analyze the collected outputs.

## How the Pieces Connect

### 1. Docker deployment

When you run:

```bash
python3 dockergo.py deploy
```

the script calls `runscen()`.

`runscen()` does three things:

- gives Docker socket permissions,
- applies kernel buffer settings with `sysctl`,
- starts the Docker containers and attaches the tap devices.

This creates the base network environment used by the experiments.

### 2. Tap and bridge connection

`CreateAndAttachTaps()` creates:

- `tap-left`
- `tap-right`

Then it attaches them to:

- `br-left`
- `br-right`

This is the link between the host network and the Docker networks defined in `docker-compose.yml`.

In other words:

- `sender` lives on `leftnet`
- `receiver` lives on `rightnet`
- the tap devices connect those networks to the host
- the host can then interconnect the Docker environment with the rest of the setup

### 3. ns-3 integration

The function `RunNs3()` in `SetupFunctions.py` launches ns-3 with a command of the form:

```bash
cd ../ns-allinone-3.38/ns-3.38/ && ./ns3 run "scratch/prueba.cc <params>"
```

or, if no extra parameters are provided:

```bash
cd ../ns-allinone-3.38/ns-3.38/ && ./ns3 run scratch/prueba.cc
```

This means ns-3 is used as an external simulator that can be launched from the Docker orchestration workflow.

In this project, ns-3 is used to:

- emulate channel conditions,
- inject delay and error rate,
- create repeatable network scenarios,
- support comparative performance experiments.

### 4. Experiment sweep execution

`MLDW_params_sweep_in_weight_final_iters.py` performs the main experiment loop.

For each scheduler and experiment setting, it:

- selects the scheduler policy,
- updates the JSON configuration files,
- computes traffic load based on the target saturation,
- launches ns-3 with the desired delay and error settings,
- starts the QUIC client and server inside the Docker containers,
- moves logs and results into a structured output directory,
- kills the running processes after each run,
- compresses the results for later analysis.

This script is the bridge between:

- QUIC scheduling logic,
- Dockerized transport execution,
- and ns-3-based network conditions.

## Running the Environment

### Step 1: Deploy the Docker environment

From this folder:

```bash
python3 dockergo.py deploy
```

This starts the Docker services and attaches the tap interfaces.

### Step 2: Run the sweep or experiment script

After deployment, run:

```bash
python3 MLDW_params_sweep_in_weight_final_iters.py
```

This executes the configured scheduler sweep and stores outputs under the `results/` directory.

### Step 3: Stop the environment

When finished:

```bash
python3 dockergo.py stop
```

This removes the Docker setup and detaches the taps.

## Configuration Files

The main runtime configuration files are located inside `quic-apps/`:

### Client

- `quic-apps/go_client/docker-v2x-cli.json`
- `quic-apps/go_client/conf_Scheduler.json`
- `quic-apps/go_client/conf_Scheduler_tmp.json`

### Server

- `quic-apps/go_server/docker-v2x-serv.json`
- `quic-apps/go_server/conf_Scheduler.json`
- `quic-apps/go_server/conf_Scheduler_tmp.json`

These files define:

- client and server addresses,
- packet sizes,
- number of sessions,
- number of streams,
- TCP or QUIC mode,
- traffic distributions,
- scheduler parameters.

The sweep script updates these JSON files automatically during the experiment loop.

## Outputs and Logs

During execution, the system writes results to:

- `quic-apps/go_client/tmp/`
- `quic-apps/go_server/tmp/`
- `quic-apps/go_client/LOGS_congestionWindow/`
- `results/` under the sweep folder structure

Typical outputs include:

- congestion window logs,
- per-stream traces,
- throughput and buffer-related metrics,
- compressed result archives for each parameter set.

## ns-3 Path in This Project

The ns-3 installation used by the scripts is expected at:

```text
../ns-allinone-3.38/ns-3.38/
```

That path is relative to the `Dockers/` folder.

The script expects an ns-3 scenario available in `scratch/prueba.cWc`, which is invoked through the `ns3` command.

## Performance Evaluation Scripts

This folder also contains the scripts used to evaluate the system under varying conditions:

- throughput and scheduler sweep scripts,
- Docker deployment helpers,
- ns-3 execution wrappers,
- result post-processing notebooks.

These scripts are meant to be adapted to the specific experimental design you want to reproduce.

## Practical Notes

- Run the Docker deployment before launching any experiment.
- Keep the JSON files under version control or store copies per experiment run.
- If you change scheduler parameters, record the exact values used.
- If you use ns-3 error or delay parameters, save those alongside the results.
- For reproducibility, always note the Git commit, system settings, and output directory.

## Related Documentation

- [Root README](../README.md)
- [QUIC implementation](../Quic_GO/README.md)
- [3GPP/C-V2X notes](../cv2x-utils/README.md)

## Contact

Fatima Khan Blanco
fatima.khan@unican.es