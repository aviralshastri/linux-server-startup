import subprocess

class SSH:
    @staticmethod
    def check_ssh_status():
        try:
            result = subprocess.run(
                ['systemctl', 'is-active', 'ssh'], 
                capture_output=True,
                text=True,
                check=True
            )
            if result.stdout.strip() == "active":
                return True
            else:
                return False
        except subprocess.CalledProcessError:
            return False


if __name__ == "__main__":
    ssh = SSH()
    status = ssh.check_ssh_status()
    print("SSH is active:", status)
