from SetupFunctions import *
from runscen import runscen
from stopscen import stopscen
import os
import glob
import json
import numpy as np
import time
import subprocess
import shutil

# ==========================================
# PARÁMETROS FIJOS DE SIMULACIÓN
# ==========================================
delays = [0]
pers = [0,2,4,6,8,10]



traffidist = "poisson"
ChannelCapacity = 27  # Mbps
T_slot_ms = 50
simTime = 120

Satutil = [50, 70, 90]
schedulers = ["MLWDF", "STBP", "RR"]  # Cambiado a plural para evitar colisiones de nombres
streamsload = [0.66, 0.34]

BASE_FOLDERNAME = "Poisson_2_1_conMDlogs_ALL"
NUM_RUNS = 1

# ==========================================
# CONFIGURACIÓN EXPERIMENTAL (Solo para MLWDF)
# ==========================================
eta_configs = [
    {"W": 1, "Q": 1, "label": "W1_Q1"},
    {"W": 1, "Q": 0, "label": "W1_Q0"},
    {"W": 0, "Q": 1, "label": "W0_Q1"}
]

Gamma_stream0 = 1.0
gammas_stream1 = [1, 2, 4, 6, 8, 10]


# ==========================================
def run_simulation_block(sched, sat, sat_folder, parent_sat_path, full_sat_path, W_val=1, Q_val=1, g_stream1=1.0):
    """
    Función auxiliar para no duplicar el código interno de la simulación.
    Se encarga de iterar sobre delays, pers y lanzar los runs.
    """
    seed_base = 12345

    for delay in delays:
        for per in pers:
            for run_id in range(NUM_RUNS):
                seed = seed_base
                run = run_id

                print(f"[RUN] {sched} | SAT={sat}% | PER={per}% | RUN={run}")

                # ==========================================
                # LIMPIEZA
                # ==========================================
                try:
                    for f in glob.glob("./quic-apps/go_client/LOGS_congestionWindow/*"): os.remove(f)
                    for f in glob.glob("./quic-apps/go_client/tmp/*"): os.remove(f)
                    for f in glob.glob("./quic-apps/go_server/tmp/*"): os.remove(f)
                except:
                    pass

                # ==========================================
                # PATH (Creación limpia con os.makedirs)
                # ==========================================
                base_path = f"{parent_sat_path}/{sat_folder}/DELAY{delay}_PER{per}/RUN{run}"
                os.makedirs(f"{base_path}/go_client", exist_ok=True)
                os.makedirs(f"{base_path}/go_server", exist_ok=True)
                os.makedirs(f"{base_path}/LOGS_congestionWindow", exist_ok=True)

                # ==========================================
                # NS-3 EXEC
                # ==========================================
                RunNs3(
                    "../ns-allinone-3.38/ns-3.38/",
                    "prueba.cc",
                    f"--delay={delay} --error-rate={per/100 if per else 0} --seed={seed} --run={run}"
                )

                # ==========================================
                # SCHEDULER CONFIG
                # ==========================================
                with open("./quic-apps/go_client/conf_Scheduler_tmp.json") as f:
                    data = json.load(f)

                data["Scheduler_name"] = sched
                data["GrowingFactor"] = 1
                data["MLWDF_Params"] = [
                    {"Gamma": Gamma_stream0, "gammaW": W_val, "gammaQ": Q_val},
                    {"Gamma": g_stream1, "gammaW": W_val, "gammaQ": Q_val}
                ]

                with open("./quic-apps/go_client/conf_Scheduler.json", "w") as f:
                    json.dump(data, f, indent=4)

                # ==========================================
                # LOAD MODEL
                # ==========================================
                effective_capacity = ChannelCapacity * (1 - per/100)
                target_load = effective_capacity * (sat / 100)

                rate0 = target_load * streamsload[0]
                rate1 = target_load * streamsload[1]
                filesize_total = (simTime * target_load) / 8

                # CLIENT JSON
                with open("./quic-apps/go_client/docker-v2x-cli_tmp.json") as f:
                    cli = json.load(f)
                cli["TimeSlot"] = T_slot_ms
                cli["InfoStream"][0].update({"FileSize": float(filesize_total * streamsload[0]), "Distribution": {"Type": traffidist, "Rate": rate0}})
                cli["InfoStream"][1].update({"FileSize": float(filesize_total * streamsload[1]), "Distribution": {"Type": traffidist, "Rate": rate1}})
                with open("./quic-apps/go_client/docker-v2x-cli.json", "w") as f:
                    json.dump(cli, f, indent=4)

                # SERVER JSON
                with open("./quic-apps/go_server/docker-v2x-serv_tmp.json") as f:
                    serv = json.load(f)
                serv["InfoStream"][0]["Distribution"]["Rate"] = rate0
                serv["InfoStream"][1]["Distribution"]["Rate"] = rate1
                serv["InfoStream"][0]["FileSize"] = float(filesize_total * streamsload[0])
                serv["InfoStream"][1]["FileSize"] = float(filesize_total * streamsload[1])
                with open("./quic-apps/go_server/docker-v2x-serv.json", "w") as f:
                    json.dump(serv, f, indent=4)

                # ==========================================
                # EXECUTION (Subprocess)
                # ==========================================
                subprocess.run("docker exec -d -w /home/quic-apps/go_client sender ./client_OBU docker-v2x-cli.json", shell=True)
                subprocess.run("docker exec -w /home/quic-apps/go_server receiver ./server docker-v2x-serv.json", shell=True)

                time.sleep(3)

                # ==========================================
                # SAVE RESULTS (Subprocess)
                # ==========================================
                subprocess.run(f"mv ./quic-apps/go_server/tmp/* {base_path}/go_server/ 2>/dev/null", shell=True)
                subprocess.run(f"mv ./quic-apps/go_client/tmp/* {base_path}/go_client/ 2>/dev/null", shell=True)
                subprocess.run(f"mv ./quic-apps/go_client/LOGS_congestionWindow/* {base_path}/LOGS_congestionWindow/ 2>/dev/null", shell=True)

                # ==========================================
                # CLEAN (Subprocess)
                # ==========================================
                subprocess.run("docker exec sender pkill client_OBU", shell=True)
                subprocess.run("docker exec receiver pkill server", shell=True)
                subprocess.run("pkill ns3", shell=True)
                
    # ==========================================
    # COMPRESS SAT BLOCK (Al terminar todas las pers/delays)
    # ==========================================
    print(f"[COMPRESSING] {full_sat_path}")
    tar_cmd = f"tar -czf {full_sat_path}.tar.gz -C {parent_sat_path} {sat_folder}"
    subprocess.run(tar_cmd, shell=True, check=True)
    shutil.rmtree(full_sat_path)


def main():
    print(f"[SIMULATION] Starting sweep with {NUM_RUNS} runs per case")

    for sched in schedulers:
        print(f"\n>>> SWITCHING TO SCHEDULER: {sched} <<<")

        if sched == "MLWDF":
            # Si es MLWDF, hacemos todo el barrido complejo de eta y gammas
            for eta in eta_configs:
                W_val = eta["W"]
                Q_val = eta["Q"]
                eta_label = eta["label"]

                for g_stream1 in gammas_stream1:
                    for sat in Satutil:
                        parent_sat_path = f"./results/{BASE_FOLDERNAME}/{sched}/{eta_label}/GAMMA_CRIT_{g_stream1:.1f}"
                        sat_folder = f"SAT{sat}"
                        full_sat_path = f"{parent_sat_path}/{sat_folder}"

                        run_simulation_block(sched, sat, sat_folder, parent_sat_path, full_sat_path, W_val, Q_val, g_stream1)
        else:
            # Si es STBP o RR, ignoramos eta y gammas. Van directo a una carpeta limpia.
            for sat in Satutil:
                parent_sat_path = f"./results/{BASE_FOLDERNAME}/{sched}"
                sat_folder = f"SAT{sat}"
                full_sat_path = f"{parent_sat_path}/{sat_folder}"

                # Ejecutamos con parámetros por defecto (no afectarán a RR/STBP)
                run_simulation_block(sched, sat, sat_folder, parent_sat_path, full_sat_path)

    print("\n[FINISHED] All experiments completed successfully.")


if __name__ == "__main__":
    main()