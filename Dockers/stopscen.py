import os
from SetupFunctions import *




def stopscen():
    # os.system('sudo chmod 666 /var/run/docker.sock')
    # os.system('sudo sysctl -w net.core.rmem_max=26214400')
    # os.system('sudo sysctl -w net.core.wmem_max=26214400')


    # cmd = 'sudo rm -r ./c_example/tmp/* '
    # os.system(cmd)
    # cmd = 'sudo rm -r ./c_example/go_client/tmp/* '
    # os.system(cmd)

    # # Setup virtual networks
    # RunDockers()
    # time.sleep(6)
    # print("Dockers Done\n")
    # CreateAndAttachTaps()
    # time.sleep(6)





   #####################################################
    # Tear down virtual network 
    ClearDockers()
    time.sleep(3)
    DetachAndRemoveTaps()
    # print("Bye")

if __name__=="__main__":
    stopscen()