import socket
import argparse
import speedtest
import re
import subprocess
import os
import json
import time
import psutil

SETTINGS_FILE = 'settings.json'

class Network:
    
    def __init__(self):
        self.settings = self.load_settings()
    
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
    
    def get_ip(self):
        """
        Get the ip of all interfaces.
        """
        addresses = psutil.net_if_addrs()
        ip_list = {}
        
        for interface_name, interface_addresses in addresses.items():
            for address in interface_addresses:
                if address.family == socket.AF_INET:
                    ip_list[interface_name] = address.address
        
        return ip_list
    
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
            print("Ping command failed.")
        return None

    def load_settings(self):
        """
        Load the settings from the JSON file.
        """
        if os.path.exists(SETTINGS_FILE):
            try:
                with open(SETTINGS_FILE, 'r') as f:
                    return json.load(f)
            except (json.JSONDecodeError, ValueError):
                print(f"Error reading {SETTINGS_FILE}. Reinitializing with default settings.")
        return {"best_server": None, "custom_speed_server": None}

    def save_settings(self):
        """
        Save the settings to the JSON file.
        """
        with open(SETTINGS_FILE, 'w') as f:
            json.dump(self.settings, f, indent=4)

    def get_internet_speed(self, server=None):
        """
        Test download and upload speeds in Mbps using a specific server.
        """
        try:
            start_time = time.time()
            st = speedtest.Speedtest()
            if server:
                st.get_servers([server])
            st.get_best_server()
            download_speed = st.download() / 1_000_000 
            upload_speed = st.upload() / 1_000_000 
            end_time = time.time()
            elapsed_time = round(end_time - start_time, 2)
            return round(download_speed, 2), round(upload_speed, 2), elapsed_time
        except Exception as e:
            print(f"Error getting internet speed: {e}")
        return None, None, None

    def get_internet_speed_quick(self):
        """
        Test download and upload speeds using the saved best server or find one and save it.
        """
        if self.settings.get('best_server'):
            server = self.settings['best_server']
        else:
            server = None
            download_speed, upload_speed, elapsed_time = self.get_internet_speed()
            if download_speed is not None and upload_speed is not None:
                self.settings['best_server'] = server
                self.save_settings()
        return self.get_internet_speed(server)

    def get_internet_speed_custom(self):
        """
        Test download and upload speeds using a custom server from the settings.
        """
        server = self.settings.get('custom_speed_server')
        if server:
            return self.get_internet_speed(server)
        else:
            print("Custom server not set in settings.")
        return None, None, None


def main():
    parser = argparse.ArgumentParser(description="Network Monitoring - IP address, internet connection, ping, and speed test.")
    
    parser.add_argument(
        "command", 
        choices=['check-network', 'get-ping', 'get-speed', 'get-speed:quick', 'get-speed:custom','get-ip'], 
        help="Command to check network connection, get ping latency, or get internet speed"
    )
    
    args = parser.parse_args()
    
    net = Network()

    if args.command == "check-network":
        status = "online" if net.check_connection() else "offline"
        print(f"Device is {status}")
    
    elif args.command == "get-ip":
        ip = net.get_ip()
        if ip is not None:
            print(ip)
        else:
            print("Failed to determine ip address of interfaces.")
    
    elif args.command == "get-ping":
        ping = net.get_ping()
        if ping is not None:
            print(f"Ping latency: {ping} ms")
        else:
            print("Failed to determine ping latency.")
    
    elif args.command == "get-speed":
        download_speed, upload_speed, elapsed_time = net.get_internet_speed()
        if download_speed is not None and upload_speed is not None:
            print(f"Download Speed: {download_speed} Mbps")
            print(f"Upload Speed: {upload_speed} Mbps")
            print(f"Time elapsed: {elapsed_time} seconds")
        else:
            print("Failed to determine internet speed.")
    
    elif args.command == "get-speed:quick":
        download_speed, upload_speed, elapsed_time = net.get_internet_speed_quick()
        if download_speed is not None and upload_speed is not None:
            print(f"Download Speed: {download_speed} Mbps (Quick Test)")
            print(f"Upload Speed: {upload_speed} Mbps (Quick Test)")
            print(f"Time elapsed: {elapsed_time} seconds")
        else:
            print("Failed to determine internet speed (Quick Test).")
    
    elif args.command == "get-speed:custom":
        download_speed, upload_speed, elapsed_time = net.get_internet_speed_custom()
        if download_speed is not None and upload_speed is not None:
            print(f"Download Speed: {download_speed} Mbps (Custom Test)")
            print(f"Upload Speed: {upload_speed} Mbps (Custom Test)")
            print(f"Time elapsed: {elapsed_time} seconds")
        else:
            print("Failed to determine internet speed (Custom Test).")

if __name__ == "__main__":
    main()
