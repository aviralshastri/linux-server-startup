import subprocess
import time
import argparse


class SSH:
    def __init__(self):
        self.ssh_status = self.check_ssh_status()

    def run_command(self, command, check=True, capture_output=True, text=True):
        """Utility to run a command using subprocess."""
        try:
            result = subprocess.run(
                command, check=check, capture_output=capture_output, text=text
            )
            return result.stdout.strip() if result.stdout else "", None
        except subprocess.CalledProcessError as e:
            return None, str(e)

    def check_ssh_status(self):
        output, error = self.run_command(['systemctl', 'is-active', 'ssh'])
        return output == "active" if output else False

    def start_ssh(self):
        _, error = self.run_command(['systemctl', 'start', 'ssh'])
        self.ssh_status = self.check_ssh_status()
        return "Success" if self.ssh_status else f"Failed: {error}"

    def stop_ssh(self):
        self.run_command(['pkill', '-f', 'ssh'], check=False)
        self.run_command(['systemctl', 'stop', 'ssh'], check=False)
        time.sleep(1)
        self.ssh_status = self.check_ssh_status()
        return "Failed" if self.ssh_status else "Success"

    def restart_ssh(self):
        if self.ssh_status:
            _, error = self.run_command(['systemctl', 'restart', 'ssh'])
            self.ssh_status = self.check_ssh_status()
            return "Success" if self.ssh_status else f"Failed: {error}"
        return self.start_ssh()

    def kill_connection(self, pid):
        _, error = self.run_command(["sudo", "kill", "-15", pid])
        return "Connection closed successfully." if not error else f"Failed to close connection: {error}"

    def list_connections(self):
        output, _ = self.run_command(['who', '-u'], capture_output=True)
        if output:
            devices = output.split('\n')
            connected_devices = [
                {
                    'user': parts[0],
                    'connection_type': parts[1],
                    'login_time': f"{parts[2]} {parts[3]}",
                    'idle_time': parts[4],
                    'pid': parts[5],
                    'ip': parts[6].strip('()') if '(' in parts[6] else 'Local'
                }
                for parts in (device.split() for device in devices) if len(parts) >= 7
            ]
            return connected_devices
        return []

    def boot_enable_ssh(self):
        _, error = self.run_command(['systemctl', 'enable', 'ssh'])
        return "SSH enabled on boot." if not error else f"Failed to enable SSH on boot: {error}"

    def boot_disable_ssh(self):
        _, error = self.run_command(['systemctl', 'disable', 'ssh'])
        return "SSH disabled on boot." if not error else f"Failed to disable SSH on boot: {error}"


def main():
    parser = argparse.ArgumentParser(description="Manage SSH service and connections.")
    parser.add_argument("command", help="The action to perform (start|stop|status|restart|list-connections|kill-connection|boot-enable|boot-disable).")
    parser.add_argument("--pid", help="PID of the connection to kill (required for kill-connection).", default=None)

    args = parser.parse_args()
    ssh = SSH()

    if args.command == "start":
        print("Starting SSH:", ssh.start_ssh())
    elif args.command == "stop":
        print("Stopping SSH:", ssh.stop_ssh())
    elif args.command == "status":
        print("Current SSH status:", "Running" if ssh.check_ssh_status() else "Not Running")
    elif args.command == "restart":
        print("Restarting SSH:", ssh.restart_ssh())
    elif args.command == "list-connections":
        connections = ssh.list_connections()
        if connections:
            for conn in connections:
                print(f"User: {conn['user']}, Type: {conn['connection_type']}, PID: {conn['pid']}, IP: {conn['ip']}")
        else:
            print("No active connections found.")
    elif args.command == "kill-connection":
        if args.pid:
            print(ssh.kill_connection(args.pid))
        else:
            print("Please provide a PID using --pid for kill-connection.")
    elif args.command == "boot-enable":
        print(ssh.boot_enable_ssh())
    elif args.command == "boot-disable":
        print(ssh.boot_disable_ssh())
    else:
        print("Invalid command!")
        parser.print_help()


if __name__ == "__main__":
    main()
