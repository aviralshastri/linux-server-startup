import subprocess

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
    
    def restart_ssh(self):
        if self.ssh_status:
            try:
                subprocess.run(['sudo', 'systemctl', 'restart', 'ssh'], check=True)
                self.ssh_status = self.check_ssh_status()
                print("SSH service restarted successfully.")
            except subprocess.CalledProcessError:
                print("Failed to restart SSH service.")
        else:
            print("SSH is not active; cannot restart.")
    
    def status_summary(self):
        status = "active" if self.ssh_status else "inactive"
        print(f"SSH Service is currently {status}.")

if __name__ == "__main__":
    ssh = SSH()
    ssh.status_summary()
    ssh.restart_ssh() 
    ssh.status_summary()