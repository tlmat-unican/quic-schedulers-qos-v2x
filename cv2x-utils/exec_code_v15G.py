import paramiko
import os
import glob
import json
import time
import subprocess
import copy
import netifaces as ni
from clock_sync import *
from funcs_to_automatize_5gsetup import *

# ==========================================
# PARÁMETROS DE CONEXIÓN REMOTA (CLIENTE) - CORE
# ==========================================
CORE_IP = "10.45.0.1"
HOST_IP = "192.168.112.61"
HOST_USER = "administrator"
HOST_PASS = "password"

REMOTE_BASE_DIR = "v2x_scenarios/Dockers/quic-apps/go_client"
LOCAL_BASE_DIR = "v2x_scenarios/Dockers/quic-apps/go_server"

# ==========================================
# PARÁMETROS DE CONEXIÓN REMOTA (gnB) 
# ==========================================
GNB_IP = "192.168.112.62"
GNB_USER = "administrator"
GNB_PASS = "password"

# ==========================================
# PARÁMETROS DE SIMULACIÓN
# ==========================================
streamnum = 2
simTime = 120  # segundos
EXECUTION_TIMEOUT = 125  # Tiempo límite estricto para el experimento (segundos)

Capacidad = 20  # Mbps agregados
sched = "STBP"
sleeptime = 15
nstart = 5
N_Experiments = 10
BASE_FOLDERNAME = sched

# PATH DEL LOGFILE LOCAL DE RESULTADOS
LOG_FILE_PATH = "./log_experimentos.txt"

# CONFIGURACIÓN DE STREAMS Y ETAS
STREAM_RATIO = [2, 1]
eta_configs = [
    {"W": 0, "Q": 1, "label": "W0_Q1"},
    # {"W": 1, "Q": 1, "label": "W1_Q1"}
]
Gamma_stream0 = 1
gammas_stream1 = [1]

# PATHS JSON
CLIENT_JSON = "./Dockers/quic-apps/go_client/docker-v2x-test5G.json"
SERVER_JSON = "./Dockers/quic-apps/go_server/docker-v2x-test5G.json"
CLIENT_SCHED_JSON = "./Dockers/quic-apps/go_client/conf_Scheduler.json"
CLIENT_SCHED_TEMPLATE = "./Dockers/quic-apps/go_client/conf_Scheduler_tmp.json"
REMOTE_CLIENT_JSON = f"{REMOTE_BASE_DIR}/docker-v2x-test5G.json"
REMOTE_SCHED_JSON = f"{REMOTE_BASE_DIR}/conf_Scheduler.json"


def restart_gnb_procedure(sshgnb):
    """
    Detiene la gNB, limpia el fichero de logs anterior, inyecta el password
    para levantar el proceso en background de forma persistente y verifica si está corriendo.
    """

    print("[gNB] Matando instancias activas de nr-softmodem...")
    stdin, stdout, stderr = sshgnb.exec_command("sudo pkill -f nr-softmodem")
    
    time.sleep(2)

   
    print("[gNB] Limpiando fichero de logs e iniciando nr-softmodem...")
    gnb_persistent_cmd = (
        "cd ~/MAC_sched_OAI/openairinterface5g/cmake_targets/ran_build/build && "
        "./nr-softmodem "
        "-O ../../../targets/PROJECTS/GENERIC-NR-5GC/CONF/00_TNSM_gNB_USRPB210.conf "
        "--gNBs.[0].min_rxtxtime 6 --sa -E"
    )

    sshgnb.exec_command(gnb_persistent_cmd)
    
    # Tiempo de guarda para procesamiento de credenciales y arranque de sockets
    time.sleep(5)

    # 3. Comprobación activa de PID remoto
    print("[gNB] Verificando si el proceso nr-softmodem se inició correctamente...")
    _, stdout_check, _ = sshgnb.exec_command("pgrep -f nr-softmodem")
    pid_resultado = stdout_check.read().decode().strip()

    if pid_resultado:
        print(f"[gNB] ¡CONFIRMADO! gNB corriendo exitosamente en el Host remoto con PID(s): {pid_resultado}")
        print("[gNB] Esperando 10s extra para la carga estable del firmware de la USRP...")
        time.sleep(10)
        return True
    else:
        print("[CRÍTICO] La gNB NO está corriendo. El proceso nr-softmodem falló al iniciar o el password fue rechazado.")
        # Mostrar el tail para depurar de forma inmediata por consola
        _, stdout_err, _ = sshgnb.exec_command("tail -n 15 ~/MAC_sched_OAI/openairinterface5g/cmake_targets/ran_build/build/oai_gnb_output.log")
        print(f"[gNB LOG ERRORS]:\n{stdout_err.read().decode()}")
        return False

def force_ue_reconnection():
    """Limpia el estado del módem Quectel mediante hardware/software y fuerza reconexión."""
    print("[UE] Aplicando RESET de hardware al módem Quectel...")
    
    #  Primer intento tras el reset de hardware básico
    reset_ue_connection() 
    ip = try_ue_connection(max_wait=5)
    
    #  Si no funciona, entramos en un bucle de reintentos controlados
    intentos = 0
    max_intentos = 3
    
    while ip is None and intentos < max_intentos:
        intentos += 1
        print(f"[UE] Intento {intentos}/{max_intentos} - Reconexión fallida. Reintentando ciclo...")
        
        reset_ue_connection()
        ip = try_ue_connection(max_wait=5)
        
        if ip is not None:
            print(f"[UE] Reconexión exitosa. IP obtenida: {ip}")
            return ip

    # Si el bucle terminó y la IP sigue siendo None, aplicamos el reset pesado (Soft/MMCLI)
    if ip is None:
        print("[UE] Error persistente. Aplicando reset de software por mmcli (espera de 120s)...")
        os.system("sudo mmcli -m any --reset")
        while True:
            # Ejecutamos mmcli -L para comprobar el estado
            result = subprocess.run(["mmcli", "-L"], capture_output=True, text=True)
            output = result.stdout.lower()
            
            # Si el módem aparece en la lista y no es un falso positivo
            if "modem" in output and "no modems" not in output:
                print("\n[UE] ¡Módem detectado!")
                break
                
            print(".", end="", flush=True)  # Imprime un punto cada 2 segundos
            time.sleep(2)
        
        while ip is None and intentos < max_intentos:
            intentos += 1
            print(f"[UE] Intento {intentos}/{max_intentos} - Reconexión fallida. Reintentando ciclo...")
            
            reset_ue_connection()
            ip = try_ue_connection(max_wait=5)
            
            if ip is not None:
                print(f"[UE] Reconexión exitosa. IP obtenida: {ip}")
                return ip
            if ip is not None:
                print(f"[UE] Reconexión exitosa tras mmcli. IP obtenida: {ip}")
            
    return ip


def verify_data_plane(target_ip, count=3):
    """Verifica si la interfaz realmente transmite datos enviando pings al Core."""
    print(f"[UE] Probando conectividad real con plano de datos en {target_ip}...")
    res = os.system(f"ping -c {count} -W 2 {target_ip} > /dev/null 2>&1")
    return res == 0


def log_experiment_status(sched_name, eta_lbl, gamma_val, iteration, status):
    """Escribe el resultado del experimento en el logfile."""
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    log_line = f"[{timestamp}] Scheduler:{sched_name} | Eta_Config:{eta_lbl} | Gamma_Stream1:{gamma_val} | Iteration:{iteration} -> STATUS:{status}\n"
    with open(LOG_FILE_PATH, "a") as f:
        f.write(log_line)


def main():
    print("[HYBRID] Servidor LOCAL / Cliente REMOTO")

    # Inicializar/Limpiar el logfile local al arrancar el script
    with open(LOG_FILE_PATH, "a") as f:
        f.write(f"\n=== NUEVA SESIÓN DE SIMULACIÓN ({time.strftime('%Y-%m-%d %H:%M:%S')}) ===\n")

    # Conexiones SSH
    ssh = create_ssh_client(HOST_IP, HOST_USER, HOST_PASS)
    sftp = ssh.open_sftp()
    print(f">>> Conectado a Cliente/Core: {HOST_IP}")

    sshgnb = create_ssh_client(GNB_IP, GNB_USER, GNB_PASS)
    print(f">>> Conectado a gNB: {GNB_IP}")

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
        for eta in eta_configs:
            W_val = eta["W"]
            Q_val = eta["Q"]
            eta_label = eta["label"]

            for g_stream1 in gammas_stream1:
                for n in range(nstart, N_Experiments):
                    
                    print("\nSynchronize clocks ...")
                    synchronize_and_verify(ssh)

                    # ------------------------------------------------------------
                    # REINICIO OBLIGATORIO Y VERIFICACIÓN DE LA INFRAESTRUCTURA 5G
                    # ------------------------------------------------------------
                    print(f"\n--- [ITERACION {n}] REINICIANDO GNB OBLIGATORIAMENTE ---")
                    gnb_viva = restart_gnb_procedure(sshgnb)

                    if not gnb_viva:
                        print("[MAIN] Saltando iteración actual por fallo crítico al levantar la gNB.")
                        log_experiment_status(sched, eta_label, g_stream1, n, "BAD_GNB_START")
                        continue

                    # Comprobar estado del enlace del módem Quectel hacia la nueva celda
                    ip = wait_for_ip(timeout=10)
                    if ip is not None:
                        # Si tiene IP, verificar que no sea una interfaz zombi (plano de datos congelado)
                        if not verify_data_plane(CORE_IP):
                            print("[UE] Interfaz zombi detectada tras el reinicio de gNB.")
                            ip = None

                    if ip is None:
                        print("[UE] Forzando reconexión del módem Quectel...")
                        ip = force_ue_reconnection()
                        
                        # Doble verificación estricta de seguridad
                        if not verify_data_plane(CORE_IP):
                            print("[CRÍTICO] El enlace sigue sin responder. Reintentando ciclo limpio extra...")
                            # restart_gnb_procedure(sshgnb)
                            ip = force_ue_reconnection()

                    print(f"[MAIN] Enlace 5G completamente verificado. Listo con IP: {ip}")
                    # ------------------------------------------------------------

                    clean_remote_dirs(ssh, REMOTE_BASE_DIR)
                    clean_local_dirs(LOCAL_BASE_DIR)
                    filesize = int((Capacidad * simTime) / 8)

                    if sched == "MLWDF":
                        parent_path = f"./results/{BASE_FOLDERNAME}/{eta_label}/GAMMA_CRIT_{g_stream1:.1f}"
                    else:
                        parent_path = f"./results/{BASE_FOLDERNAME}"

                    iter_path = f"{parent_path}/ITER{n}"
                    print(f"[RUNNING] ETA:{eta_label} | G:{g_stream1} | ITER:{n}")

                    # Preparación de directorios locales para almacenamiento de logs válido
                    os.makedirs(f"{iter_path}/go_server", exist_ok=True)
                    os.makedirs(f"{iter_path}/go_client", exist_ok=True)
                    os.makedirs(f"{iter_path}/LOGS_congestionWindow", exist_ok=True)

                    # Configuración dinámica de Archivos de Parámetros (JSON)
                    with open(CLIENT_SCHED_TEMPLATE) as f:
                        sched_data = json.load(f)
                    sched_data["Scheduler_name"] = sched
                    sched_data["MLWDF_Params"] = [
                        {"Gamma": Gamma_stream0, "gammaW": W_val, "gammaQ": Q_val},
                        {"Gamma": g_stream1, "gammaW": W_val, "gammaQ": Q_val}
                    ]
                    with open(CLIENT_SCHED_JSON, "w") as f:
                        json.dump(sched_data, f, indent=4)

                    configure_streams(CLIENT_JSON, streamnum, Capacidad, filesize, ip, ratio=STREAM_RATIO)
                    configure_streams(SERVER_JSON, streamnum, Capacidad, filesize, ip, ratio=STREAM_RATIO)

                    sftp.put(CLIENT_SCHED_JSON, REMOTE_SCHED_JSON)
                    sftp.put(CLIENT_JSON, REMOTE_CLIENT_JSON)

                    # ==========================================
                    # EJECUCIÓN ASÍNCRONA CON CONTROL DE TIMEOUT
                    # ==========================================
                    print(">>> Lanzando SERVIDOR LOCAL...")
                    server_proc = subprocess.Popen(
                        "cd ./Dockers/quic-apps/go_server && ./server ./docker-v2x-test5G.json",
                        shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1
                    )

                    print(">>> Lanzando CLIENTE REMOTO (No bloqueante)...")
                    transport = ssh.get_transport()
                    channel = transport.open_session()
                    channel.get_pty()
                    channel.exec_command(f"cd {REMOTE_BASE_DIR} && ./client_OBU ./docker-v2x-test5G.json")

                    start_time = time.time()
                    timed_out = False

                    server_proc.stdout.flush()
                    os.set_blocking(server_proc.stdout.fileno(), False)

                    print(f"[MONITOR] Monitoreando experimento (Max: {EXECUTION_TIMEOUT}s)...")
                    while True:
                        elapsed_time = time.time() - start_time

                        # Romper bucle si el servidor local ya terminó de procesar
                        if server_proc.poll() is not None:
                            break

                        # Romper bucle si nos excedemos de la ventana de ejecución permitida
                        if elapsed_time > EXECUTION_TIMEOUT:
                            timed_out = True
                            break

                        try:
                            line = server_proc.stdout.readline()
                            if line:
                                print(f"[SERVER] {line}", end='')
                        except Exception:
                            pass

                        time.sleep(0.5)

                    # ==========================================
                    # EVALUACIÓN DE RESULTADOS POST-SIMULACIÓN
                    # ==========================================
                    if timed_out:
                        print(f"\n[ALERTA] Bad ejecución del experimento en cuestión: {sched} | Eta: {eta_label} | Gamma: {g_stream1} | Iter: {n} - status: BAD")
                        
                        # Guardar estado en logfile estructurado
                        log_experiment_status(sched, eta_label, g_stream1, n, "BAD")

                        print("[ACCION] Matando procesos locales colgados...")
                        server_proc.terminate()
                        server_proc.wait()

                        print("[ACCION] Matando cliente remoto colgado...")
                        channel.close()
                        ssh.exec_command("pkill -f client_OBU")
                        
                        print("[INFO] Saltando recolección de logs debido a iteración colgada.")
                        continue  # Salta directo al siguiente ciclo (donde se matará y limpiará la gNB de nuevo)

                    else:
                        print(f">>> Servidor local finalizado correctamente ({sched} | Eta: {eta_label} | Gamma: {g_stream1} | Iter: {n}) - status: GOOD")
                        
                        # Guardar estado exitoso en logfile estructurado
                        log_experiment_status(sched, eta_label, g_stream1, n, "GOOD")

                        server_proc.stdout.close()
                        channel.close()
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

                    clean_remote_dirs(ssh)
                    print(f"[INFO] Espero tiempo de guarda entre experimentos ({sleeptime}s) ...")
                    time.sleep(sleeptime)

    finally:
        print("[SHUTDOWN] Deteniendo todos los procesos y apagando gNB remota de forma segura...")
        stdin, stdout, stderr = sshgnb.exec_command("sudo pkill -f nr-softmodem", get_pty=True)
        stdin.write(GNB_PASS + '\n')
        stdin.flush()
        stdout.read()

        sftp.close()
        ssh.close()
        sshgnb.close()
        print("Conexiones finalizadas. Script terminado.")


if __name__ == "__main__":
    main()