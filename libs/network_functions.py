import socket
import argparse
import speedtest
import re
import subprocess
import logging
import os


logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

class Network:
    
    def __init__(self):
        pass
    
    def check_connection(self, host='8.8.8.8', port=53, timeout=5):
        """
        Check if the device can connect to a specific host and port.
        """
        try:
            socket.setdefaulttimeout(timeout)
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((host, port))
            return True
        except socket.error:
            return False
    
    def get_ping(self, host='8.8.8.8'):
        """
        Get the ping latency in milliseconds.
        """
        try:
            ping_command = ['ping', '-c', '1', host] if os.name != 'nt' else ['ping', '-n', '1', host]
            output = subprocess.check_output(ping_command, stderr=subprocess.STDOUT, text=True)
            
            latency_pattern = re.compile(r'(\d+\.?\d*) ms') if os.name != 'nt' else re.compile(r'Average = (\d+)ms')
            match = latency_pattern.search(output)
            if match:
                return round(float(match.group(1)), 2)
        except subprocess.CalledProcessError:
            logging.error("Ping command failed.")
        return None
    
    def get_internet_speed(self):
        """
        Test download and upload speeds in Mbps.
        """
        try:
            st = speedtest.Speedtest()
            st.get_best_server()
            download_speed = st.download() / 1_000_000
            upload_speed = st.upload() / 1_000_000 
            return round(download_speed, 2), round(upload_speed, 2)
        except Exception as e:
            logging.error(f"Error getting internet speed: {e}")
        return None, None

def main():
    parser = argparse.ArgumentParser(description="Network Monitoring - internet connection, ping, and speed test.")
    
    parser.add_argument(
        "command", 
        choices=['check-network', 'get-ping', 'get-speed'], 
        help="Command to check network connection, get ping latency, or get internet speed"
    )
    
    args = parser.parse_args()
    
    net = Network()

    if args.command == "check-network":
        status = "online" if net.check_connection() else "offline"
        logging.info(f"Device is {status}")
    
    elif args.command == "get-ping":
        ping = net.get_ping()
        if ping is not None:
            logging.info(f"Ping latency: {ping} ms")
        else:
            logging.info("Failed to determine ping latency.")
    
    elif args.command == "get-speed":
        download_speed, upload_speed = net.get_internet_speed()
        if download_speed is not None and upload_speed is not None:
            logging.info(f"Download Speed: {download_speed} Mbps")
            logging.info(f"Upload Speed: {upload_speed} Mbps")
        else:
            logging.info("Failed to determine internet speed.")

if __name__ == "__main__":
    main()
