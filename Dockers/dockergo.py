import sys
from SetupFunctions import *
from runscen import runscen
from stopscen import stopscen
import os
import glob


def main():
    if len(sys.argv) < 2:
        print("Usage: python dockergo.py [deploy|stop]")
        return

    action = sys.argv[1].lower()

    if action == "deploy":
        runscen()
    elif action == "stop":
        stopscen()
    else:
        print("Invalid option. Use 'deploy' or 'stop'.")


if __name__ == "__main__":
    main()