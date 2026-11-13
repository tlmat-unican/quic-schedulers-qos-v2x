import paramiko
import os
import glob
import json
import time
import threading
import sys

# ==========================================
# PARÁMETROS DE CONEXIÓN REMOTA
# ==========================================
HOST_IP = "192.168.88.5"
HOST_USER = "fatima"
HOST_PASS = "password"
REMOTE_BASE_DIR = "./quic-apps/go_server" # Ruta en el servidor

# ==========================================
# PARÁMETROS FIJOS DE SIMULACIÓN
# ==========================================
delays = [0]
pers = [0] 
traffidist = "poisson"  
ChannelCapacity = 27 # Mbps
streamnum = 2
T_slot_ms = 50
simTime = 2     
Satutil = [0] 
sched = "MLWDF" #MLWDF
sleeptime = 30
nstart = 1
N_Experiments = 2
BASE_FOLDERNAME = f"{sched}" # Cambio el nombre para no mezclar

# ==========================================
# CONFIGURACIÓN DE ETAS
# ==========================================
eta_configs = [
    # {"W": 1, "Q": 0, "label": "W1_Q0"},
    # {"W": 1, "Q": 1, "label": "W1_Q1"},
    {"W": 0, "Q": 1, "label": "W0_Q1"}
]

Gamma_stream0 = 1.0
gammas_stream1 = [1,3,5,10,15,20,30,50,100]
gammas_stream1 =[100]

# ==========================================
# FUNCIONES AUXILIARES
# ==========================================
def run_client_silent(cmd):
    """
    Ejecuta el cliente localmente pero oculta su salida.
    """
    # > /dev/null 2>&1  <-- Esto manda toda la salida del cliente al vacío
    os.system(f"{cmd} > /dev/null 2>&1")

def clean_remote(ssh):
    ssh.exec_command(f"rm -f {REMOTE_BASE_DIR}/tmp/*.log")


def create_ssh_client():
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(HOST_IP, username=HOST_USER, password=HOST_PASS)
    return client

def clean_local_dirs():
    """Limpia temporales locales"""
    try:
        # Ajusta estas rutas a donde compile/guarde tu cliente local
        files = glob.glob("./quic-apps/go_client/LOGS_congestionWindow/*") + \
                glob.glob("./quic-apps/go_client/tmp/*")
        for f in files:
            os.remove(f)
    except Exception:
        pass

def clean_remote_dirs(ssh_client):
    """Limpia temporales en el servidor remoto"""
    # Borramos logs antiguos en tmp
    cmd = f"rm -f {REMOTE_BASE_DIR}/tmp/*.log"
    ssh_client.exec_command(cmd)




def main():
    print(f"[HYBRID] - Iniciando Barrido Cliente Local / Servidor Remoto")
    
    # 1. Establecer conexión SSH única
    ssh = create_ssh_client()
    sftp = ssh.open_sftp()
    print(f">>> Conectado a {HOST_IP}")
    os.system("sudo sysctl -w net.core.rmem_default=2500000")
    os.system("sudo sysctl -w net.core.rmem_max=2500000")
    os.system("sudo sysctl -w net.core.wmem_default=2500000")
    os.system("sudo sysctl -w net.core.wmem_max=2500000")

    # os.system("sudo chronyc burst 4/4")
    # time.sleep(2) # Esperar a que lleguen las respuestas
    # os.system("sudo chronyc makestep")
    
    # # 2. Verificar el estado en el tracking local (opcional, para tu log de Python)
    # print("--- Estado del Reloj Local (Cliente) ---")
    # os.system("chronyc tracking")
    # time.sleep(80)
    try:
        for eta in eta_configs:
            W_val, Q_val, eta_label = eta["W"], eta["Q"], eta["label"]

            for g_stream1 in gammas_stream1:
               
                for n in range(nstart, N_Experiments):
                    print("Synchronize clocks ...")
                    os.system("sudo ntpdate 212.227.145.233")
                    ssh.exec_command("sudo ntpdate 212.227.145.233")
                    print(" -> Done !\n")
                    if sched != "MLWDF":
                        parent_sat_path = f"./results/{BASE_FOLDERNAME}/"
                    else: # Definir rutas de resultados
                        parent_sat_path = f"./results/{BASE_FOLDERNAME}/{eta_label}/GAMMA_CRIT_{g_stream1:.1f}"
                    n_folder_name = f"ITER{n}"
                    full_sat_path = f"{parent_sat_path}/{n_folder_name}"

                    for per in pers:
                        print(f"\n[RUNNING] Eta:{eta_label} | G:{g_stream1} | ITER:{n}/{N_Experiments} | PER:{per}%")

                        # 1. Limpieza Previa
                        clean_local_dirs()
                        clean_remote_dirs(ssh)
                        # Asegurar que no hay proceso server corriendo
                        ssh.exec_command("killall server") 

                        # 2. Crear directorios de resultados
                        base_path = f"{full_sat_path}"
                        os.makedirs(f"{base_path}/go_client", exist_ok=True)
                        os.makedirs(f"{base_path}/go_server", exist_ok=True)
                        os.makedirs(f"{base_path}/LOGS_congestionWindow", exist_ok=True)

                        # 3. Configurar Scheduler (Local)
                        # NOTA: Esto asume que tu cliente local lee este JSON
                        with open("./quic-apps/go_client/conf_Scheduler_tmp.json", "r") as f:
                            data = json.load(f)
                        
                        data["Scheduler_name"] = sched
                        data["MLWDF_Params"] = [
                            {"Gamma": Gamma_stream0, "gammaW": W_val, "gammaQ": Q_val},
                            {"Gamma": g_stream1, "gammaW": W_val, "gammaQ": Q_val}
                        ]
                        with open("./quic-apps/go_client/conf_Scheduler.json", "w") as f:
                            json.dump(data, f, indent=4)

                        

                        
                        print(">>> Iniciando Servidor Remoto...")
                        
                        
                        _, stdout, _ = ssh.exec_command(
                            f"cd {REMOTE_BASE_DIR} && ./server ./docker-v2x-test.json", 
                            get_pty=True
                        )
                        
                        # Esperamos un pelín para que el server levante socket
                        time.sleep(1)
                        print(">>> Lanzando Cliente (Background)...")
                        
                        client_cmd = "cd ./quic-apps/go_client && ./client_OBU ./docker-v2x-test.json"
                        # Creamos el hilo que ejecutará el cliente
                        client_thread = threading.Thread(target=run_client_silent, args=(client_cmd,))
                        client_thread.start()

                        
                        print("--- VISUALIZANDO LOGS DEL SERVIDOR ---")

                        
                        while not stdout.channel.exit_status_ready():
                            # Seguimos imprimiendo lo que quede
                            if stdout.channel.recv_ready():
                                output = stdout.channel.recv(1024).decode('utf-8', errors='ignore')
                                sys.stdout.write(output)
                                sys.stdout.flush()
                            # if time.time() - start_wait > MAX_WAIT:
                            #     print(">>> Timeout esperando cierre del servidor.")
                            #     break
                            time.sleep(0.01)

                        
                        

                        #  Resultados
                        print(">>> Recolectando Logs...")
                        
                        # A) Descargar logs del servidor (SFTP)
                        remote_tmp_dir = f"{REMOTE_BASE_DIR}/tmp"
                        try:
                            files_rem = sftp.listdir(remote_tmp_dir)
                            for f_name in files_rem:
                                if f_name.endswith(".log"):
                                    sftp.get(f"{remote_tmp_dir}/{f_name}", f"{base_path}/go_server/{f_name}")
                        except FileNotFoundError:
                            print("   [WARN] No se encontraron logs en el servidor remoto.")
                        time.sleep(3)
    
                        ssh.exec_command(
                            f"rm {remote_tmp_dir}/*", 
                            get_pty=True
                        )
                        ssh.exec_command(
                            f"rm {REMOTE_BASE_DIR}/LOGS_congestionWindow/*", 
                            get_pty=True
                        )
                        
                        os.system(f'mv ./quic-apps/go_client/tmp/*.log {base_path}/go_client/ 2>/dev/null')
                        os.system(f'mv ./quic-apps/go_client/LOGS_congestionWindow/*.log {base_path}/LOGS_congestionWindow/ 2>/dev/null')
                        print(f"[INFO] Espero {sleeptime}s ...\n")
                        time.sleep(sleeptime)
                        print(" [INFO] Finalizado tiempo de espera\n")
                    
            # print(f"[COMPRESSING] Zipping {parent_sat_path}...")
            # compress_cmd = (
            #     f"tar -czf {parent_sat_path}.tar.gz -C {parent_sat_path} {BASE_FOLDERNAME} "
            #     f"&& rm -rf {parent_sat_path}"
            # )
            # os.system(compress_cmd)

    except KeyboardInterrupt:
        print("\n[STOP] Interrupción detectada. Cerrando conexiones...")
    except Exception as e:
        print(f"\n[ERROR] {e}")
    finally:
        sftp.close()
        ssh.close()
        print("Conexión cerrada.")

if __name__ == "__main__":
    main()