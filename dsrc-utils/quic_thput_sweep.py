import paramiko
import os
import glob
import json
import time
import threading
import sys
import copy

# ==========================================
# PARÁMETROS DE CONEXIÓN REMOTA
# ==========================================
HOST_IP = "192.168.88.5"
HOST_USER = "fatima"
HOST_PASS = "password"
REMOTE_BASE_DIR = "/quic-apps/go_server"


HOST_IP = "192.168.112.61"
HOST_USER = "administrator"
HOST_PASS = "password"
# ==========================================
# PARÁMETROS FIJOS DE SIMULACIÓN
# ==========================================
traffidist = "poisson"
streamnum = 1           # <<<<<< CAMBIA AQUÍ Nº DE STREAMS
T_slot_ms = 50
sched = "RR"            # MLWDF
sleeptime = 80
nstart = 0
N_Experiments = 1
BASE_FOLDERNAME = f"QUIC_Performace{sched}"

# ==========================================
# CONFIGURACIÓN DE ETAS
# ==========================================
capacities = [10, 15, 25, 30, 35]

# ==========================================
# FUNCIONES AUXILIARES
# ==========================================
def run_client_silent(cmd):
    os.system(f"{cmd} > /dev/null 2>&1")

def create_ssh_client():
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(HOST_IP, username=HOST_USER, password=HOST_PASS)
    return client

def clean_local_dirs():
    try:
        files = glob.glob("./quic-apps/go_client/LOGS_congestionWindow/*") + \
                glob.glob("./quic-apps/go_client/tmp/*")
        for f in files:
            os.remove(f)
    except Exception:
        pass

def clean_remote_dirs(ssh_client):
    ssh_client.exec_command(f"rm -f {REMOTE_BASE_DIR}/tmp/*.log")
    ssh_client.exec_command(f"rm -f {REMOTE_BASE_DIR}/LOGS_congestionWindow/*.log")

def configure_streams(json_path, streamnum, cap, total_filesize=60):
    with open(json_path, "r") as f:
        data = json.load(f)

    data["TotalStream"] = streamnum

    base_stream = data["InfoStream"][0]
    rate_per_stream = cap / streamnum
    filesize_per_stream = total_filesize / streamnum

    streams = []
    for i in range(streamnum):
        s = copy.deepcopy(base_stream)
        s["FileSize"] = filesize_per_stream
        s["Distribution"]["Rate"] = rate_per_stream
        streams.append(s)

    data["InfoStream"] = streams

    with open(json_path, "w") as f:
        json.dump(data, f, indent=4)

local_json = "./quic-apps/go_server/docker-v2x-perf.json"
remote_json = f"{REMOTE_BASE_DIR}/docker-v2x-perf.json"
def upload_json_to_server(sftp, local_json, remote_json):
    sftp.put(local_json, remote_json)

# ==========================================
# MAIN
# ==========================================
def main():

    print(f"[HYBRID] - Iniciando Barrido Cliente Local / Servidor Remoto")

    ssh = create_ssh_client()
    sftp = ssh.open_sftp()
    print(f">>> Conectado a {HOST_IP}")

    os.system("sudo sysctl -w net.core.rmem_default=2500000")
    os.system("sudo sysctl -w net.core.rmem_max=2500000")
    os.system("sudo sysctl -w net.core.wmem_default=2500000")
    os.system("sudo sysctl -w net.core.wmem_max=2500000")

    try:
        for n in range(nstart, N_Experiments):
            for cap in capacities:

                print(f"\n[RUNNING] CAP:{cap} | ITER:{n}/{N_Experiments}")

                os.system("sudo ntpdate 212.227.145.233")
                ssh.exec_command("sudo ntpdate 212.227.145.233")

                clean_local_dirs()
                clean_remote_dirs(ssh)
                ssh.exec_command("killall server")

                parent_sat_path = f"./results/{BASE_FOLDERNAME}/CAP{cap}"
                iter_path = f"{parent_sat_path}/ITER{n}"

                os.makedirs(f"{iter_path}/go_client", exist_ok=True)
                os.makedirs(f"{iter_path}/go_server", exist_ok=True)
                os.makedirs(f"{iter_path}/LOGS_congestionWindow", exist_ok=True)

                print("[Config-files] scheduler configuration")
                # Scheduler
                with open("./quic-apps/go_client/conf_Scheduler_tmp.json", "r") as f:
                    sched_data = json.load(f)
                sched_data["Scheduler_name"] = sched
                with open("./quic-apps/go_client/conf_Scheduler.json", "w") as f:
                    json.dump(sched_data, f, indent=4)

                # Streams (Client)
                configure_streams(
                    "./quic-apps/go_client/docker-v2x-perf.json",
                    streamnum=streamnum,
                    cap=cap,
                    total_filesize=60
                )
                configure_streams(
                    "./quic-apps/go_server/docker-v2x-perf.json",
                    streamnum=streamnum,
                    cap=cap,
                    total_filesize=60
                )
                upload_json_to_server(sftp, local_json, remote_json)
                print(">>> Iniciando Servidor Remoto...")
                _, stdout, _ = ssh.exec_command(
                    f"cd {REMOTE_BASE_DIR} && ./server ./docker-v2x-perf.json",
                    get_pty=True
                )

                time.sleep(1)

                print(">>> Lanzando Cliente (Background)...")
                client_cmd = "cd ./quic-apps/go_client && ./client_OBU ./docker-v2x-perf.json"
                client_thread = threading.Thread(target=run_client_silent, args=(client_cmd,))
                client_thread.start()

                print("--- VISUALIZANDO LOGS DEL SERVIDOR ---")
                while not stdout.channel.exit_status_ready():
                    if stdout.channel.recv_ready():
                        output = stdout.channel.recv(1024).decode("utf-8", errors="ignore")
                        sys.stdout.write(output)
                        sys.stdout.flush()
                    time.sleep(0.01)

                print(">>> Recolectando Logs...")
                remote_tmp_dir = f"{REMOTE_BASE_DIR}/tmp"
                try:
                    for f_name in sftp.listdir(remote_tmp_dir):
                        if f_name.endswith(".log"):
                            sftp.get(
                                f"{remote_tmp_dir}/{f_name}",
                                f"{iter_path}/go_server/{f_name}"
                            )
                except FileNotFoundError:
                    print("[WARN] No se encontraron logs en el servidor")
                time.sleep(3)

                ssh.exec_command(
                    f"rm {remote_tmp_dir}/*", 
                    get_pty=True
                )
                ssh.exec_command(
                    f"rm {REMOTE_BASE_DIR}/LOGS_congestionWindow/*", 
                    get_pty=True
                )
                os.system(f"mv ./quic-apps/go_client/tmp/*.log {iter_path}/go_client/ 2>/dev/null")
                os.system(f"mv ./quic-apps/go_client/LOGS_congestionWindow/*.log {iter_path}/LOGS_congestionWindow/ 2>/dev/null")

                print(f"[INFO] Espero {sleeptime}s ...")
                time.sleep(sleeptime)

    except KeyboardInterrupt:
        print("\n[STOP] Interrupción detectada.")
    except Exception as e:
        print(f"\n[ERROR] {e}")
    finally:
        sftp.close()
        ssh.close()
        print("Conexión cerrada.")

# ==========================================
if __name__ == "__main__":
    main()
