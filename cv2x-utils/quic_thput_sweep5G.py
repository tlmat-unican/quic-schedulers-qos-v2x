import paramiko
import os
import glob
import json
import time
import threading
import sys
import copy
import subprocess
from clock_sync import *
from funcs_to_automatize_5gsetup import *
# ==========================================
# PARÁMETROS DE CONEXIÓN REMOTA
# ==========================================
HOST_IP = "192.168.112.61"
HOST_USER = "administrator"
HOST_PASS = "password"
LOG_FILE_PATH = "./log_experimentos.txt"
REMOTE_BASE_DIR = "v2x_scenarios/Dockers/quic-apps/go_client"
CORE_IP = "10.45.0.1"
EXECUTION_TIMEOUT = 125
REMOTE_BASE_DIR = "v2x_scenarios/Dockers/quic-apps/go_client"
LOCAL_BASE_DIR = "v2x_scenarios/Dockers/quic-apps/go_server"
# ==========================================
# PARÁMETROS FIJOS DE SIMULACIÓN
# ==========================================
streamnum = 1
sched = "RR"
simtime = 120 
sleeptime = 1
nstart = 4
N_Experiments = 5
BASE_FOLDERNAME = f"QUIC_Performace{sched}_1streams"

# ==========================================
# CONFIGURACIÓN DE ETAS
# ==========================================
capacities = [46]

# ==========================================
# FUNCIONES AUXILIARES
# ==========================================

def configure_streams(json_path, streamnum, cap, total_filesize=60, ip_listener=None):
    with open(json_path, "r") as f:
        data = json.load(f)

    data["TotalStream"] = streamnum
    base_stream = data["InfoStream"][0]
    data["Addr"] = f"{ip_listener}:20001"
    rate_per_stream = cap / streamnum
    filesize_per_stream = total_filesize / streamnum

    streams = []
    for _ in range(streamnum):
        s = copy.deepcopy(base_stream)
        s["FileSize"] = filesize_per_stream
        s["Distribution"]["Rate"] = rate_per_stream
        streams.append(s)

    data["InfoStream"] = streams

    with open(json_path, "w") as f:
        json.dump(data, f, indent=4)

def stream_process_output(proc, prefix="[SERVER]"):
    for line in iter(proc.stdout.readline, ''):
        if not line:
            break
        print(f"{prefix} {line}", end='')

# ==========================================
# PATHS JSON
# ==========================================
local_server_json = "./Dockers/quic-apps/go_server/docker-v2x-perf5G.json"
local_client_json = "./Dockers/quic-apps/go_client/docker-v2x-perf5G.json"
local_sched_json = "./Dockers/quic-apps/go_client/conf_Scheduler.json"
remote_client_json = f"{REMOTE_BASE_DIR}/docker-v2x-perf5G.json"
remote_sched_json = f"{REMOTE_BASE_DIR}/conf_Scheduler.json"

# ==========================================
# MAIN
# ==========================================
def main():

    print("[HYBRID] Servidor LOCAL / Cliente REMOTO")  
    with open(LOG_FILE_PATH, "a") as f:
        f.write(f"\n=== NUEVA SESIÓN DE SIMULACIÓN ({time.strftime('%Y-%m-%d %H:%M:%S')}) ===\n")

    # Conexión SSH al Cliente/Core únicamente
    ssh = create_ssh_client(HOST_IP, HOST_USER, HOST_PASS)
    sftp = ssh.open_sftp()
    print(f">>> Conectado a Cliente/Core: {HOST_IP}")

    # Aplicar sysctls de red en ambos extremos
    verificaciones = [
        "sudo sysctl -w net.core.rmem_default=2500000",
        "sudo sysctl -w net.core.rmem_max=2500000",
        "sudo sysctl -w net.core.wmem_default=2500000",
        "sudo sysctl -w net.core.wmem_max=2500000",
    ]
    for cmd in verificaciones:
        stdin, stdout, stderr = ssh.exec_command(cmd, get_pty=True)
        stdin.write(HOST_PASS + '\n')
        stdin.flush()
        stdout.read()
        os.system(cmd)
    try:
        for n in range(nstart, N_Experiments):
            for cap in capacities:
                filesize = int((cap*simtime)/8)
                print(f"\n[RUNNING] CAP:{cap} | ITER:{n}/{N_Experiments}")

                print("\nSynchronize clocks ...")
                synchronize_and_verify(ssh)
                # ------------------------------------------------------------
                # VERIFICACIÓN DE LA INFRAESTRUCTURA 5G (SIN REINICIAR gNB)
                # ------------------------------------------------------------
                print(f"\n--- [ITERACION {n}] VERIFICANDO ENLACE 5G MANTENIENDO gNB ACTIVA ---")

                # Comprobar estado del enlace del módem Quectel hacia la celda
                ip = wait_for_ip(timeout=10)
                if ip is not None:
                    # Si tiene IP, verificar que no sea una interfaz zombi (plano de datos congelado)
                    if not verify_data_plane(CORE_IP):
                        print("[UE] Interfaz zombi detectada.")
                        ip = None

                if ip is None:
                    print("[UE] Forzando reconexión del módem Quectel...")
                    ip = force_ue_reconnection()
                    
                    # Doble verificación estricta de seguridad
                    if not verify_data_plane(CORE_IP):
                        print("[CRÍTICO] El enlace sigue sin responder. Reintentando ciclo limpio extra...")
                        ip = force_ue_reconnection()

                print(f"[MAIN] Enlace 5G completamente verificado. Listo con IP: {ip}")





                clean_local_dirs(LOCAL_BASE_DIR)
                clean_remote_dirs(ssh, REMOTE_BASE_DIR)

                parent_path = f"./results/{BASE_FOLDERNAME}/CAP{cap}"
                iter_path = f"{parent_path}/ITER{n}"

                os.makedirs(f"{iter_path}/go_server", exist_ok=True)
                os.makedirs(f"{iter_path}/go_client", exist_ok=True)
                os.makedirs(f"{iter_path}/LOGS_congestionWindow", exist_ok=True)

                # Scheduler cliente
                with open("./Dockers/quic-apps/go_client/conf_Scheduler_tmp.json") as f:
                    sched_data = json.load(f)
                sched_data["Scheduler_name"] = sched
                with open("./Dockers/quic-apps/go_client/conf_Scheduler.json", "w") as f:
                    json.dump(sched_data, f, indent=4)

                print(f"[INFO] Configurando streams para {streamnum} streams, CAP: {cap} Mbps, FileSize: {filesize} MB, IP: {ip}")
                # Config streams
                configure_streams(local_client_json, streamnum, cap, filesize, ip)
                configure_streams(local_server_json, streamnum, cap, filesize, ip)
                sftp.put(local_sched_json, remote_sched_json)
                # Subir JSON cliente
                sftp.put(local_client_json, remote_client_json)





                # =============================
                # LANZAR CLIENTE REMOTO
                # =============================
                print(">>> Lanzando CLIENTE REMOTO...")
                ssh.exec_command(
                    f"cd {REMOTE_BASE_DIR} && ./client_OBU ./docker-v2x-perf5G.json",
                    get_pty=True
                )
                # =============================
                # LANZAR SERVIDOR LOCAL
                # =============================
                print(">>> Lanzando SERVIDOR LOCAL...")
                server_cmd = "cd ./Dockers/quic-apps/go_server && ./server ./docker-v2x-perf5G.json"

                server_proc = subprocess.Popen(
                    server_cmd,
                    shell=True,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1
                )

               

                start_time = time.time()
                timed_out = False

                server_proc.stdout.flush()
                os.set_blocking(server_proc.stdout.fileno(), False)
               
                print(f"[MONITOR] Monitoreando experimento (Max: {EXECUTION_TIMEOUT}s)...")
                while True:
                    elapsed_time = time.time() - start_time

                    # 1. Leer de forma continua TODO lo que haya en el buffer actual del stdout
                    while True:
                        try:
                            line = server_proc.stdout.readline()
                            if not line:
                                break
                            print(f"[SERVER] {line}", end='')
                        except Exception:
                            break

                    # Romper bucle si el servidor local ya terminó de procesar
                    if server_proc.poll() is not None:
                        break

                    # Romper bucle si nos excedemos de la ventana de ejecución permitida
                    if elapsed_time > EXECUTION_TIMEOUT:
                        timed_out = True
                        break

                    time.sleep(0.5)

                # 2. VACIADO FINAL (Draining): Lee las líneas finales tras la muerte del proceso
                while True:
                    try:
                        line = server_proc.stdout.readline()
                        if not line:
                            break
                        print(f"[SERVER] {line}", end='')
                    except Exception:
                        break

                server_proc.stdout.close()
                server_proc.wait()  # Espera a que el servidor termine completamente
                print(">>> Servidor local finalizado")
                time.sleep(5)

                if timed_out:
                    print(f"\n[ALERTA] Bad ejecución del experimento en cuestión: {sched} | CAP: {cap} | Streams: {streamnum} | Iter: {n} - status: BAD")
                    
                    # Guardar estado en logfile estructurado
                    log_experiment_status(sched, cap, streamnum, n, "BAD", LOG_FILE_PATH)
                    
                    print("[INFO] Saltando recolección de logs debido a iteración colgada.")
                    continue  # Salta directo al siguiente ciclo

                else:
                    print(f">>> Servidor local finalizado correctamente ({sched} | CAP: {cap} | Streams: {streamnum}  | Iter: {n}) - status: GOOD")
                    
                    # Guardar estado exitoso en logfile estructurado
                    log_experiment_status(sched, cap, streamnum, n, "GOOD", LOG_FILE_PATH)

                    server_proc.stdout.close()
                
                    time.sleep(5)

                    # Recolección y guardado de logs locales válidos
                    os.system(f"mv ./Dockers/quic-apps/go_server/tmp/*.log {iter_path}/go_server/ 2>/dev/null")

                    # Recolección de logs remotos vía SFTP
                    for folder in ["tmp", "LOGS_congestionWindow"]:
                        remote_dir = f"{REMOTE_BASE_DIR}/{folder}"
                        local_dir = f"{iter_path}/go_client" if folder == "tmp" else f"{iter_path}/LOGS_congestionWindow"
                        try:
                            for f in sftp.listdir(remote_dir):
                                if f.endswith(".log"):
                                    sftp.get(f"{remote_dir}/{f}", f"{local_dir}/{f}")
                        except FileNotFoundError:
                            pass

                clean_remote_dirs(ssh, REMOTE_BASE_DIR)
                print(f"[INFO] Espero tiempo de guarda entre experimentos ({sleeptime}s) ...")
                time.sleep(sleeptime)
    except KeyboardInterrupt:
        print("\n[STOP] Interrupción detectada.")
    finally:
        sftp.close()
        ssh.close()
        print("Conexión cerrada.")

# ==========================================
if __name__ == "__main__":
    main()
