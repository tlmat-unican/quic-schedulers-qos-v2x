sudo tunctl -t tap-left
sudo tunctl -t tap-right
sudo ip link set tap-left promisc on
sudo ip link set tap-right promisc on
sudo ip link set tap-left up
sudo ip link set tap-right up
sudo brctl addif br-left tap-left
sudo brctl addif br-right tap-right