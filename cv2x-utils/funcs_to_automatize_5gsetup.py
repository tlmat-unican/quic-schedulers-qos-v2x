
from asyncio import timeout
from tracemalloc import start

import paramiko
import os
import glob
import json
import time
import subprocess
import copy
import netifaces as ni



def force_ue_reconnection():
    """Limpia el estado del módem Quectel mediante hardware/software y fuerza reconexión."""
    print("[UE] Aplicando RESET de hardware al módem Quectel...")
    
    # Primer intento tras el reset de hardware básico
    reset_ue_connection() 
    ip = try_ue_connection(max_wait=5)
    
    # Si no funciona, entramos en un bucle de reintentos controlados
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
        print("[UE] Error persistente. Aplicando reset de software por mmcli...")
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


def log_experiment_status(sched_name, eta_lbl, gamma_val, iteration, status, logfile_path):
    """Escribe el resultado del experimento en el logfile."""
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    log_line = f"[{timestamp}] Scheduler:{sched_name} | Eta_Config:{eta_lbl} | Gamma_Stream1:{gamma_val} | Iteration:{iteration} -> STATUS:{status}\n"
    with open(logfile_path, "a") as f:
        f.write(log_line)





# ==========================================
# AUXILIARES
# ==========================================
def create_ssh_client(ip, user, password, timeout=10):
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

    client.connect(
        hostname=ip,
        username=user,
        password=password,
        timeout=timeout
    )

    return client

def clean_remote_dirs(ssh, remote_base_dir):
    ssh.exec_command(f"rm -f {remote_base_dir}/tmp/*.log")
    ssh.exec_command(f"rm -f {remote_base_dir}/LOGS_congestionWindow/*.log")

def clean_local_dirs(local_base_dir):
    os.system(f"rm -f {local_base_dir}/tmp/*.log")
    os.system(f"rm -f {local_base_dir}/LOGS_congestionWindow/*.log")

def wait_for_ip(interface="wwan0", timeout=120):
    start = time.time()

    while time.time() - start < timeout:
        try:
            ip = ni.ifaddresses(interface)[ni.AF_INET][0]['addr']

            if ip:
                return ip

        except (KeyError, ValueError):
            pass

        time.sleep(1)

    return None
def try_ue_connection(max_wait=60, CORE_IP="10.45.0.1"):
    """
    Intenta que el UE conecte sin reiniciar nada.
    Solo espera IP + ping al core.
    """
    start = time.time()
    while time.time() - start < max_wait:
        try:
            ip = ni.ifaddresses('wwan0')[ni.AF_INET][0]['addr']
            if ip:
                print(f"[UE] IP detectada: {ip}")

                rc = os.system(
                    f"ping -I wwan0 -c 1 -W 1 {CORE_IP} > /dev/null 2>&1"
                )
                if rc == 0:
                    print("[UE] Conexión OK sin reset")
                return ip
        except Exception:
            pass
        time.sleep(1)
    return None

def reset_ue_connection():
    os.system("../autoconfig.sh")
    
def wait_for_ue_connection(timeout=180):
    """
    Espera a que el Quectel obtenga IP en wwan0 y
    tenga conectividad con el Core.
    """
    start = time.time()

    while time.time() - start < timeout:
        try:
            ip = ni.ifaddresses('wwan0')[ni.AF_INET][0]['addr']
            if ip:
                print(f"[UE] IP detectada: {ip}")
                
                # Lanzamos el ping para dejar registro en el log
                rc = os.system(
                    "ping -I wwan0 -c 1 -W 1 10.45.0.1 > /dev/null 2>&1"
                )
                if rc == 0:
                    print("[UE] Conectividad OK con el Core")
                    return ip
                else:
                    print("[UE] IP asignada, pero el Core aún no responde al ping (saliendo igualmente...)")

                

        except (KeyError, ValueError):
            pass

        time.sleep(1)
    return None

def configure_streams(json_path,streamnum,cap,total_filesize,ip_listener,ratio=None):
    if ratio is None:
        ratio = [1] * streamnum

    if len(ratio) != streamnum:
        raise ValueError(
            f"streamnum={streamnum} pero ratio={ratio}"
        )

    with open(json_path, "r") as f:
        data = json.load(f)

    data["Addr"] = f"{ip_listener}:20001"
    data["TotalStream"] = streamnum

    base_stream = data["InfoStream"][0]
    total_ratio = sum(ratio)

    streams = []

    for weight in ratio:

        stream = copy.deepcopy(base_stream)

        stream["Distribution"]["Rate"] = (
            cap * weight / total_ratio
        )

        stream["FileSize"] = (
            total_filesize * weight / total_ratio
        )

        streams.append(stream)

    data["InfoStream"] = streams

    with open(json_path, "w") as f:
        json.dump(data, f, indent=4)

    print("\n[STREAM CONFIGURATION]")

    for i, stream in enumerate(streams):
        print(
            f"Stream {i}: "
            f"Rate={stream['Distribution']['Rate']:.2f} Mbps | "
            f"FileSize={stream['FileSize']:.2f} MB"
        )
def restart_gnb(sshgnb, gnb_cmd):
    sshgnb.exec_command("sudo pkill -f nr-softmodem")
    time.sleep(2)

    sshgnb.exec_command(
        f"{gnb_cmd}"
    )