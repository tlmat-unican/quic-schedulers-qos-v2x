import os
import time 
import subprocess
import json
#####################################################
# Setup functions
#####################################################
def RunDockers ():
  cmd = 'docker compose up'
  pr = subprocess.Popen(cmd, shell=True)

def CreateAndAttachTaps ():
  os.system('sudo tunctl -t tap-left')
  os.system('sudo tunctl -t tap-right')
  os.system('sudo ip link set tap-left promisc on')
  os.system('sudo ip link set tap-right promisc on')
  os.system('sudo ip link set tap-left up')
  os.system('sudo ip link set tap-right up')
  while True:
    print('-- Attaching tap-left to br-left')
    ret = os.system('sudo brctl addif br-left tap-left')
    if ret == 0: 
      break
    time.sleep(5)  
  while True:
    print('-- Attaching tap-right to br-right')
    ret = os.system('sudo brctl addif br-right tap-right')
    if ret == 0: 
      break
    time.sleep(5)  

def RunNs3 (ns3Path, scenario, ConfigParams="",silent=True):

  if ConfigParams!="":
    ConfigParamsStr="{}".format(ConfigParams) 
    cmd = 'cd {} && ./ns3 run "scratch/{} {}"'.format(ns3Path,scenario,ConfigParamsStr)
  else:
    cmd = 'cd {} && ./ns3 run scratch/{}'.format(ns3Path, scenario)
  print("[ns-3] Configuration")
  print(f">> {cmd}")

  # Suppress logs if silent=True
  if silent:
    pr = subprocess.Popen(cmd, shell=True,
                            stdout=subprocess.DEVNULL,
                            stderr=subprocess.DEVNULL)
  else:
    pr = subprocess.Popen(cmd, shell=True)


  return pr
  

#####################################################
# Tear down functions
#####################################################
def ClearDockers ():
  '''
  Clear all docker-related stuff
  - Stop containers
  - Remove containers
  - Remove docker networks (virtual birdges)
  '''
  os.system ('docker kill $(docker ps -q)')
  os.system ('docker rm $(docker ps -a -q)')
  os.system ('docker network prune -f')


def DetachAndRemoveTaps ():
  '''
  Clear setup related to taps
  - Detach from virtual bridges (if docker networks do not exist, it throughs a message error)
  - Set down the tap devices
  - Remove the tap devices
  '''
  os.system('sudo brctl delif br-left tap-left')
  os.system('sudo brctl delif br-right tap-right')
  os.system('sudo ip link set dev tap-left down')  
  os.system('sudo ip link set dev tap-right down')  
  os.system('sudo tunctl -d tap-left')
  os.system('sudo tunctl -d tap-right')


def StopNs3 (scenario) :
  '''
  Step the ns-3 scenario
  '''

  cmd = 'sudo killall {}'.format(scenario)
  print(cmd)
  os.system (cmd)

