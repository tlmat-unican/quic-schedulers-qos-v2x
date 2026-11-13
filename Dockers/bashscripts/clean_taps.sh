#!/bin/bash

# Función para eliminar una interfaz TAP
delete_tap() {
    local iface=$1
    echo "-- Eliminando $iface del sistema"
    
    # Intentar bajarla primero
    sudo ip link set "$iface" down 2>/dev/null
    
    # Intentar eliminarla de cualquier puente
    sudo brctl delif br-left "$iface" 2>/dev/null
    sudo brctl delif br-right "$iface" 2>/dev/null
    
    # Eliminar la interfaz TAP
    sudo tunctl -d "$iface" 2>/dev/null
}

# Eliminar TAPs
delete_tap tap-left
delete_tap tap-right

echo "Interfaces TAP eliminadas."
