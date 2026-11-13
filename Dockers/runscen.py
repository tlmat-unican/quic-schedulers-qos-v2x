import os
from SetupFunctions import *




def runscen():
    os.system('sudo chmod 666 /var/run/docker.sock')
    os.system('sudo sysctl -w net.core.rmem_max=26214400')
    os.system('sudo sysctl -w net.core.wmem_max=26214400')



    # Setup virtual networks
    RunDockers()
    print("Dockers Done\n")
    CreateAndAttachTaps()
  


if __name__=="__main__":
    runscen()