import subprocess

class SSH
    def check_ssh_status():
        try:
            result = subprocess.run(
                ['systemctl', 'status', 'ssh'],
                capture_output=True,
                text=True,
                check=True
            )
            print("SSH Status:")
            print(result.stdout)
        except subprocess.CalledProcessError as e:
            print("Error checking SSH status:")
            print(e.stderr)


if __name__=="__main__":
    ssh= SSH
    ssh.check_ssh_status()
