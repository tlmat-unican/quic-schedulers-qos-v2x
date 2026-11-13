import time
import os

NTP_SERVER = "212.227.145.233"


# =========================
# LOCAL SYNC
# =========================
def sync_local_clock(server=NTP_SERVER):
    print(f"\n[NTP LOCAL] Sync con {server}")
    os.system(f"sudo -n ntpdate {server}")


def check_ntp_offset_local(server=NTP_SERVER):
    print("\n[NTP LOCAL OFFSET]")
    os.system(f"sudo -n ntpdate -q {server}")


# =========================
# SSH SAFE EXECUTION
# =========================
def run_cmd(ssh, cmd):
    stdin, stdout, stderr = ssh.exec_command(cmd)

    out = stdout.read().decode().strip()
    err = stderr.read().decode().strip()

    return out, err


# =========================
# REMOTE SYNC (SIN SUDO)
# =========================
def sync_remote_clock(ssh, server=NTP_SERVER):
    print(f"\n[NTP REMOTO] Sync con {server}")


    out, err = run_cmd(ssh, f"ntpdate {server}")

    if out:
        print(out)
    if err:
        print(err)


def check_ntp_offset_remote(ssh, server=NTP_SERVER):
    print("\n[NTP REMOTO OFFSET]")


    out, err = run_cmd(ssh, f"ntpdate -q {server}")

    if out:
        print(out)
    if err:
        print(err)


# =========================
# CLOCK COMPARISON
# =========================
def get_remote_time(ssh):
    out, err = run_cmd(
        ssh,
        "python3 -c 'import time; print(time.time())'"
    )
    return float(out)





# =========================
# PIPELINE
# =========================
def synchronize_and_verify(ssh):

    print("\n===================================")
    print(" CLOCK SYNCHRONIZATION CHECK ")
    print("===================================")

    sync_local_clock()
    sync_remote_clock(ssh)

    print("\nEsperando estabilización...")
    time.sleep(2)

    check_ntp_offset_local()
    check_ntp_offset_remote(ssh)

    print("===================================\n")