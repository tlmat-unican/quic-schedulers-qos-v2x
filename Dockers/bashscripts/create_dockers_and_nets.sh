docker network create \
  --driver bridge \
  --subnet 10.1.0.0/24 \
  --opt com.docker.network.bridge.enable_ip_masquerade=true \
  --opt com.docker.network.bridge.name=br-left \
  leftnet

docker network create \
  --driver bridge \
  --subnet 10.2.0.0/24 \
  --opt com.docker.network.bridge.enable_ip_masquerade=true \
  --opt com.docker.network.bridge.name=br-right \
  rightnet


docker run -d --name sender \
  --privileged \
  --net leftnet --ip 10.1.0.2 \
  -v $(pwd):/home/ \
  ldiez/ns3docker \
  ash -c "python3 /home/AdaptRouteTable.py -d 10.2.0.0/24 -g 10.1.0.3 -i eth0 && tail -F anything"


docker run -d --name receiver \
  --privileged \
  --net rightnet --ip 10.2.0.2 \
  -v $(pwd):/home/ \
  ldiez/ns3docker \
  ash -c "python3 /home/AdaptRouteTable.py -d 10.1.0.0/24 -g 10.2.0.3 -i eth0 && tail -F anything"


docker update --restart=always sender
docker update --restart=always receiver
