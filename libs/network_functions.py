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
        Get the IP of all interfaces.
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
        Updates the best server in settings if a new one is found.
        """
        try:
            start_time = time.time()
            st = speedtest.Speedtest()
            if server:
                st.get_servers([server])
                selected_server = st.get_best_server()
            else:
                selected_server = st.get_best_server()
            
            if selected_server['id'] != self.settings.get('best_server'):
                self.settings['best_server'] = selected_server['id']
                self.save_settings()
                print(f"Updated best server in settings to: {selected_server['id']}")
            
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
        Perform a quicker speed test using a saved server and reduced sample count.
        """
        server = self.settings.get('best_server')
        
        def quick_test(server_id):
            try:
                start_time = time.time()
                st = speedtest.Speedtest()
                st.get_servers([server_id])
                st.get_best_server()
                def dummy_callback(*args, **kwargs):
                    pass

                download_speed = st.download(threads=1, callback=dummy_callback) / 1_000_000
                upload_speed = st.upload(threads=1, callback=dummy_callback) / 1_000_000
                
                end_time = time.time()
                elapsed_time = round(end_time - start_time, 2)
                
                return round(download_speed, 2), round(upload_speed, 2), elapsed_time
            except Exception as e:
                print(f"Error in quick speed test with server {server_id}: {e}")
                return None, None, None

        if server:
            print(f"Using saved server: {server}")
            download_speed, upload_speed, elapsed_time = quick_test(server)

        if download_speed is None or upload_speed is None:
            print("Saved server failed or not found. Finding best server...")
            try:
                st = speedtest.Speedtest()
                st.get_best_server()
                server = st.best['id']
                print(f"Found best server: {server}")
                download_speed, upload_speed, elapsed_time = quick_test(server)
                
                if download_speed is not None and upload_speed is not None:
                    self.settings['best_server'] = server
                    self.save_settings()
                    print(f"New best server saved in settings: {server}")
                else:
                    print("Failed to get speed with new server.")
            except Exception as e:
                print(f"Error finding best server: {e}")

        return download_speed, upload_speed, elapsed_time
    
    def get_internet_speed_custom(self):
        """
        Test download and upload speeds using a custom server from the settings.
        """
        server = self.settings.get('custom_speed_server')
        
        if not server:
            print("No custom server saved in settings.")
            return None, None, None
        
        try:
            print(f"Using custom server: {server}")
            download_speed, upload_speed, elapsed_time = self.get_internet_speed(server)
            
            if download_speed is None or upload_speed is None:
                print(f"Error using custom server {server}. Unable to get speed test results.")
            else:
                return download_speed, upload_speed, elapsed_time
        
        except Exception as e:
            print(f"Error using custom server {server}: {str(e)}")
        
        return None, None, None
    

    def restart_network(self):
        """
        Stops, checks, restarts network services, and validates network connectivity.
        """
        def run_command(command, check=True):
            """Executes a system command and optionally checks for errors."""
            try:
                result = subprocess.run(command, capture_output=True, text=True, check=check)
                return result
            except subprocess.CalledProcessError as e:
                print(f"Command failed: {e}")
                return None

        def is_service_active(service_name):
            """Checks if a systemd service is active."""
            result = run_command(['systemctl', 'is-active', service_name], check=False)
            return result and result.stdout.strip() == 'active'

        def ping_host(host):
            """Pings a host and returns True if it is reachable, False otherwise."""
            result = run_command(['ping', '-c', '1', host], check=False)
            return result and result.returncode == 0

        print("Stopping network services...")
        run_command(['systemctl', 'stop', 'systemd-networkd.socket'])
        run_command(['systemctl', 'stop', 'systemd-networkd'])
        
        time.sleep(2)

        while is_service_active('systemd-networkd') or is_service_active('systemd-networkd.socket'):
            print("Services still active. Stopping again...")
            run_command(['systemctl', 'stop', 'systemd-networkd.socket'])
            run_command(['systemctl', 'stop', 'systemd-networkd'])
            time.sleep(2)

        while ping_host('8.8.8.8'):
            print("Network still active. Stopping services again...")
            run_command(['systemctl', 'stop', 'systemd-networkd.socket'])
            run_command(['systemctl', 'stop', 'systemd-networkd'])
            time.sleep(2)

        print("Starting network services...")
        run_command(['systemctl', 'start', 'systemd-networkd.socket'])
        run_command(['systemctl', 'start', 'systemd-networkd'])

        while not is_service_active('systemd-networkd') or not is_service_active('systemd-networkd.socket'):
            print("Services not active. Restarting...")
            run_command(['systemctl', 'start', 'systemd-networkd.socket'])
            run_command(['systemctl', 'start', 'systemd-networkd'])
            time.sleep(2)

        while not ping_host('8.8.8.8'):
            print("Network not live. Restarting services...")
            run_command(['systemctl', 'start', 'systemd-networkd.socket'])
            run_command(['systemctl', 'start', 'systemd-networkd'])
            time.sleep(2)

        print("Network is live and services are active.")
    
    def get_all_servers(self):
        """
        Fetch and display the details of the best speedtest servers.
        """
        try:
            st = speedtest.Speedtest()
            servers = st.get_servers()
            return servers

        except Exception as e:
            print(f"Error fetching best speedtest servers: {e}")
            return None
    
    def get_all_servers(self):
        """
        Fetch and display the details of the best speedtest servers.
        """
        try:
            st = speedtest.Speedtest()
            servers = st.get_servers()
            return servers

        except Exception as e:
            print(f"Error fetching best speedtest servers: {e}")
            return None
    
    def get_best_ping_server(self):
        """
        Fetch and return details of the single best speedtest server.
        """
        try:
            st = speedtest.Speedtest()
            best_server = st.get_best_server()
            
            server_info = {
                'id': best_server['id'],
                'name': best_server['name'],
                'sponsor': best_server['sponsor'],
                'country': best_server['country'],
                'distance': round(best_server['d'], 2),
                'latency': round(best_server['latency'], 2)
            }
            
            return server_info

        except Exception as e:
            print(f"Error fetching best speedtest server: {e}")
            return None
        
        
def main():
    parser = argparse.ArgumentParser(description="Network Monitoring - IP address, internet connection, ping, fetching available servers and best servers, and speed test.")
    
    parser.add_argument(
        "command", 
        choices=['check-network', 'get-ping', 'get-speed', 'get-speed:quick', 'get-speed:custom', 'get-ip', 'restart-network', 'get-all-servers', 'get-best-server'], 
        help="Command to check network connection, restart network, get ping latency, get internet speed, or retrieve available servers"
    )
    
    args = parser.parse_args()
    
    net = Network()

    match args.command:
        case "check-network":
            status = "online" if net.check_connection() else "offline"
            print(f"Device is {status}")

        case "get-ip":
            ip = net.get_ip()
            if ip:
                print(ip)
            else:
                print("Failed to determine IP address of interfaces.")

        case "get-ping":
            ping = net.get_ping()
            if ping is not None:
                print(f"Ping latency: {ping} ms")
            else:
                print("Failed to determine ping latency.")

        case "get-speed":
            download_speed, upload_speed, elapsed_time = net.get_internet_speed()
            if download_speed is not None and upload_speed is not None:
                print(f"Download Speed: {download_speed} Mbps")
                print(f"Upload Speed: {upload_speed} Mbps")
                print(f"Time elapsed: {elapsed_time} seconds")
            else:
                print("Failed to determine internet speed.")

        case "get-speed:quick":
            download_speed, upload_speed, elapsed_time = net.get_internet_speed_quick()
            if download_speed is not None and upload_speed is not None:
                print(f"Download Speed: {download_speed} Mbps (Quick Test)")
                print(f"Upload Speed: {upload_speed} Mbps (Quick Test)")
                print(f"Time elapsed: {elapsed_time} seconds")
            else:
                print("Failed to determine internet speed (Quick Test).")

        case "get-speed:custom":
            download_speed, upload_speed, elapsed_time = net.get_internet_speed_custom()
            if download_speed is not None and upload_speed is not None:
                print(f"Download Speed: {download_speed} Mbps (Custom Test)")
                print(f"Upload Speed: {upload_speed} Mbps (Custom Test)")
                print(f"Time elapsed: {elapsed_time} seconds")
            else:
                print("Failed to determine internet speed (Custom Test).")

        case "restart-network":
            net.restart_network()

        case "get-all-servers":
            all_servers = net.get_all_servers()
            if all_servers:
                print(all_servers)
            else:
                print("Failed to retrieve all servers.")

        case "get-best-server":
            best_servers = net.get_best_ping_server()
            if best_servers:
                print(best_servers)
            else:
                print("Failed to retrieve best servers.")

if __name__ == "__main__":
    main()
