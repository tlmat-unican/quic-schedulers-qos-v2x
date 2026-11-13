# Plotting and Results Analysis

This folder contains the Jupyter notebooks used to process, analyze, and visualize the experimental results. The notebooks are organized according to the type of evaluation.

### Testbed Experiments

Notebooks dedicated to processing and analyzing results obtained from the experimental testbeds:

- `MLWDF_Analysis_Testbeds.ipynb`: Analysis and visualization of MLWDF scheduler performance in the testbeds.
- `quic_thput_sweep_Testbeds.ipynb`: Throughput sweep analysis for the testbed experiments.

**Generating the results:** To reproduce the testbed experiments and generate the corresponding results, refer to the following directories:

- [`cv2x-utils/`](../cv2x-utils/): Scripts and utilities for the C-V2X testbed experiments.
    - Throughput sweep orchestration script, named `quic_thput_swwep5g.py`, used to automate experiment runs.
    - Scheduler comparison script named `exec_code_v15G.py`, used to evaluate and compare scheduler behavior across scenarios.
- [`dsrc-utils/`](../dsrc-utils/): Scripts and utilities for the DSRC testbed experiments.
    - Throughput sweep orchestration script, named `quic_thput_swwep.py`, used to automate experiment runs.
    - Scheduler comparison script named `exec_code_v1.py`, used to evaluate and compare scheduler behavior across scenarios.

### Simulation Experiments

Notebooks dedicated to processing and analyzing simulation results:

- `MLWDF_Analysis_Sims.ipynb`: Analysis and visualization of MLWDF scheduler performance in simulations.
- `sensitivity_eta_analysis_Sims.ipynb`: Sensitivity analysis of the MLWDF scheduling weights ($\Gamma_W$ and $\Gamma_Q$).

**Generating the results:** To reproduce the simulations and generate the corresponding results, refer to the [`Docker/`](../Dockers/) directory, which contains the Docker environments and scripts required to run the simulations.






