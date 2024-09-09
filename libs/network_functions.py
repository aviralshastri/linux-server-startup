import socket
import argparse

class Network:
    
    def __init__(self):
        pass
    
    def check_connection(self,host='8.8.8.8', port=53, timeout=5):
        try:
            socket.setdefaulttimeout(timeout)
            socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
            return True
        except socket.error:
            return False

    
    def get_internet_speed(self):
        pass
    
    def get_ping(self):
        pass
    

def main():
    parser = argparse.ArgumentParser(description="Network Monitoring - internet connection.")
    
    parser.add_argument(
        "command", 
        choices=['check-network'], 
        help="Command to get system resource usage: 'check-network'"
    )
    
    args = parser.parse_args()
    
    net = Network()

    if args.command == "check-network":
        print("Device is online" if net.check_connection() else "Device is offline")

if __name__ == "__main__":
    main()