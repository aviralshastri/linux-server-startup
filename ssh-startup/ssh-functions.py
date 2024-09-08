import subprocess
import sys
import time

class SSH:
    def __init__(self):
        self.ssh_status = self.check_ssh_status()

    def check_ssh_status(self):
        try:
            result = subprocess.run(
                ['systemctl', 'is-active', 'ssh'], 
                capture_output=True,
                text=True,
                check=True
            )
            return result.stdout.strip() == "active"
        except subprocess.CalledProcessError:
            return False
    
    def start_ssh(self):
        try:
            subprocess.run(
                ['systemctl', 'start', 'ssh'], 
                capture_output=True,
                text=True,
                check=True
            )
            self.ssh_status = self.check_ssh_status()
            return "Success" if self.ssh_status else "Failed"
        except subprocess.CalledProcessError:
            return "Failed to start SSH service."
    
    def stop_ssh(self):
        try:
            current_sessions = self.list_connections()
            for session in current_sessions:
                if "tty" not in session['connection_type']:
                    self.force_kill_connection(session["pid"])
            
            subprocess.run(
                ['systemctl', 'stop', 'ssh'], 
                capture_output=True,
                text=True,
                check=True
            )
            time.sleep(1)
            self.ssh_status = self.check_ssh_status()
            return "Failed" if self.ssh_status else "Success"
        except subprocess.CalledProcessError:
            return "Failed to stop SSH service."
    
    def restart_ssh(self):
        if self.ssh_status:
            try:
                subprocess.run(['systemctl', 'restart', 'ssh'], check=True)
                self.ssh_status = self.check_ssh_status()
                return "Success" if self.ssh_status else "Failed"
            except subprocess.CalledProcessError:
                return "Failed to restart SSH service."
        else:
            start_result = self.start_ssh()
            if start_result == "Success":
                return "SSH was not active. Started SSH service successfully."
            else:
                return "Failed to start SSH service when attempting to restart."
    
    def kill_connection(self, pid):
        try:
            subprocess.run(
                ["kill", pid],
                capture_output=True,
                text=True,
                check=True
            )
            return "Connection closed successfully."
        except subprocess.CalledProcessError:
            return "Failed to close connection"
    
    def force_kill_connection(self, pid):
        try:
            subprocess.run(
                ["kill", "-9", pid],
                capture_output=True,
                text=True,
                check=True
            )
            return "Connection force closed successfully."
        except subprocess.CalledProcessError:
            return "Failed to force close connection"
    
    def list_connections(self):
        result = subprocess.run(['who', '-u'], stdout=subprocess.PIPE)
        devices = result.stdout.decode('utf-8').strip().split('\n')
        connected_devices = []

        for device in devices:
            parts = device.split()
            if len(parts) >= 8:
                user = parts[0]                  
                connection_type = parts[1]       
                login_time = f"{parts[2]} {parts[3]}" 
                idle_time = parts[4]            
                pid = parts[5]                  
                ip = parts[6].strip('()') if '(' in parts[6] else 'Local'  
                connected_devices.append({
                    'user': user,
                    'connection_type': connection_type,
                    'login_time': login_time,
                    'idle_time': idle_time,
                    'pid': pid,
                    'ip': ip
                })

        return connected_devices
    
    def boot_enable_ssh(self):
        try:
            subprocess.run(['systemctl', 'enable', 'ssh'], capture_output=True, text=True, check=True)
            return "SSH enabled on boot."
        except subprocess.CalledProcessError:
            return "Failed to enable SSH on boot."

    def boot_disable_ssh(self):
        try:
            subprocess.run(['systemctl', 'disable', 'ssh'], capture_output=True, text=True, check=True)
            return "SSH disabled on boot."
        except subprocess.CalledProcessError:
            return "Failed to disable SSH on boot."


if __name__ == "__main__":
    
    if len(sys.argv) < 2:
        print("Usage: python script.py <start|stop|status|restart|list-connections|boot-enable|boot-disable>")
        sys.exit(1)
    command = sys.argv[1]
    ssh = SSH()
    
    if command == "start":
        print("Starting SSH:", ssh.start_ssh())
    elif command == "stop":
        print("Stopping SSH:", ssh.stop_ssh())
        print("SSH Status after stop:", "Running" if ssh.check_ssh_status() else "Not Running")
    elif command == "status":
        print("Current SSH status:", "Running" if ssh.check_ssh_status() else "Not Running")
    elif command == "restart":
        print("Restarting SSH:", ssh.restart_ssh())
    elif command == "list-connections":
        print("List of SSH connections:\n", ssh.list_connections())
    elif command == "boot-enable":
        print(ssh.boot_enable_ssh())
    elif command == "boot-disable":
        print(ssh.boot_disable_ssh())
    else:
        print("Invalid command. Usage: python script.py <start|stop|status|restart|list-connections|boot-enable|boot-disable>")
