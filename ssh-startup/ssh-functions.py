import subprocess
import sys

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
            return "Success" if self.check_ssh_status else "Failed"
        except subprocess.CalledProcessError:
            ("Failed to start SSH service.")
    
    def stop_ssh(self):
        try:
            subprocess.run(
                ['systemctl', 'stop', 'ssh'], 
                capture_output=True,
                text=True,
                check=True
            )
            return "Failed" if self.check_ssh_status else "Success"
        except subprocess.CalledProcessError:
            ("Failed to start SSH service.")
    
    def restart_ssh(self):
        if self.ssh_status:
            try:
                subprocess.run(['sudo', 'systemctl', 'restart', 'ssh'], check=True)
                self.ssh_status = self.check_ssh_status()
                return "Success" if self.check_ssh_status() else "Failed"
            except subprocess.CalledProcessError:
                print("Failed to restart SSH service.")
                return "Failed"
        else:
            print("SSH is not active; cannot restart.")
            return "Failed"

if __name__ == "__main__":
    args=sys.argv
    ssh = SSH()
    if args[1]=="start":
        print("Starting SSH: ",ssh.start_ssh())
    elif args[1]=="stop":
        print("Shutting down SSH: ",ssh.stop_ssh())
    elif args[1]=="check-status":
        print("Current SSH status: ","Running" if ssh.check_ssh_status() else "Not Running")
    elif args[1]=="restart":
        print("Restarting SSH: ",ssh.restart_ssh())
    else:
        print("Invalid system arguments!")