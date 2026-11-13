

import time
import sys
from pyminicircuits import Attenuator

def same_as_minicircuitsprogram():
    
    DWELL_TIME = 6.0  
    SECUENCIA = [0.0, 6.0, 12.0, 18.0, 12.0, 6.0]

    try:
        print("====================================================")
        print("   CLON DE AUTOMATIZACIÓN MINI-CIRCUITS (WINDOWS)   ")
        print("====================================================")
        print("Conectando al dispositivo USB HID...")
        
        # Inicializar el atenuador
        dispositivo = Attenuator()
        
        sn = dispositivo.get_serial()
        print(f" -> Modelo: RCDAT-6000-110 detectado.")
        print(f" -> S/N: {sn}")
        print(f" -> Dwell Time configurado: {DWELL_TIME} segundos.")
        print("----------------------------------------------------")
        print("Ejecutando secuencia en bucle... (Presiona Ctrl+C para detener)")
        print("----------------------------------------------------")

        # Inicializamos el índice de la secuencia
        indice = 0

        while True:
            valor_db = SECUENCIA[indice]
            dispositivo.set_attenuation(valor_db)
            hora_actual = time.strftime("%H:%M:%S")
            print(f"[{hora_actual}] Paso {indice + 1} -> Ajustado a: {valor_db:.2f} dB")
            
            # Avanzar el índice para el próximo ciclo (vuelve a 0 si llega al final)
            indice = (indice + 1) % len(SECUENCIA)
            time.sleep(DWELL_TIME)

    except KeyboardInterrupt:
        print("\n\n[INFO] Automatización detenida por el usuario (Ctrl+C).")
        print("[INFO] El atenuador mantendrá el último valor establecido.")
        
    except Exception as e:
        print(f"\n[ERROR] Error de comunicación con el hardware: {e}")
        print("\nPRO TIP PARA WINDOWS:")
        print("La causa más común de este error es que la GUI oficial de Mini-Circuits")
        print("(la de tu imagen) sigue abierta o en segundo plano. Windows bloquea el")
        print("dispositivo USB HID de forma exclusiva. Ciérrala por completo e intenta de nuevo.")
        sys.exit(1)

if __name__ == "__main__":
    # Asegurar que la salida de consola en Windows no tenga lag de buffer
    sys.stdout.reconfigure(line_buffering=True) if hasattr(sys.stdout, 'reconfigure') else None
    same_as_minicircuitsprogram()