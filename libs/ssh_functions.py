import subprocess
import time
import argparse


class SSHManager:
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
        """Check if SSH service is active."""
        output, _ = self.run_command(['systemctl', 'is-active', 'ssh'])
        return output == "active"

    def start_ssh(self):
        """Start the SSH service."""
        _, error = self.run_command(['systemctl', 'start', 'ssh'])
        self.ssh_status = self.check_ssh_status()
        return "Success" if self.ssh_status else f"Failed: {error}"

    def stop_ssh(self):
        """Stop the SSH service."""
        self.run_command(['pkill', '-f', 'ssh'], check=False)
        self.run_command(['systemctl', 'stop', 'ssh'], check=False)
        time.sleep(1)  # Allow time for the service to stop
        self.ssh_status = self.check_ssh_status()
        return "Success" if not self.ssh_status else "Failed"

    def restart_ssh(self):
        """Restart the SSH service."""
        if self.ssh_status:
            _, error = self.run_command(['systemctl', 'restart', 'ssh'])
            self.ssh_status = self.check_ssh_status()
            return "Success" if self.ssh_status else f"Failed: {error}"
        return self.start_ssh()

    def kill_connection(self, pid):
        """Kill an SSH connection by PID."""
        _, error = self.run_command(["sudo", "kill", "-15", pid])
        return "Connection closed successfully." if not error else f"Failed to close connection: {error}"

    def list_connections(self):
        """List active SSH connections."""
        output, _ = self.run_command(['who', '-u'])
        if output:
            connections = []
            for line in output.splitlines():
                parts = line.split()
                if len(parts) >= 7:
                    connections.append({
                        'user': parts[0],
                        'connection_type': parts[1],
                        'login_time': f"{parts[2]} {parts[3]}",
                        'idle_time': parts[4],
                        'pid': parts[5],
                        'ip': parts[6].strip('()') if '(' in parts[6] else 'Local'
                    })
            return connections
        return []

    def enable_ssh_on_boot(self):
        """Enable SSH to start on boot."""
        _, error = self.run_command(['systemctl', 'enable', 'ssh'])
        return "SSH enabled on boot." if not error else f"Failed to enable SSH on boot: {error}"

    def disable_ssh_on_boot(self):
        """Disable SSH from starting on boot."""
        _, error = self.run_command(['systemctl', 'disable', 'ssh'])
        return "SSH disabled on boot." if not error else f"Failed to disable SSH on boot: {error}"


def main():
    parser = argparse.ArgumentParser(description="Manage SSH service and connections.")
    parser.add_argument("command", choices=[
        "start", "stop", "status", "restart", "list-connections", "kill-connection", "boot-enable", "boot-disable"
    ], help="Action to perform.")
    parser.add_argument("--pid", help="PID of the connection to kill (required for kill-connection).", default=None)

    args = parser.parse_args()
    ssh_manager = SSHManager()

    command_map = {
        "start": ssh_manager.start_ssh,
        "stop": ssh_manager.stop_ssh,
        "status": lambda: "Running" if ssh_manager.check_ssh_status() else "Not Running",
        "restart": ssh_manager.restart_ssh,
        "list-connections": lambda: [
            f"User: {conn['user']}, Type: {conn['connection_type']}, PID: {conn['pid']}, IP: {conn['ip']}"
            for conn in ssh_manager.list_connections()
        ],
        "kill-connection": lambda: (
            ssh_manager.kill_connection(args.pid) if args.pid else "Please provide a PID using --pid for kill-connection."
        ),
        "boot-enable": ssh_manager.enable_ssh_on_boot,
        "boot-disable": ssh_manager.disable_ssh_on_boot
    }

    result = command_map.get(args.command, lambda: "Invalid command!")()
    if isinstance(result, list):
        for item in result:
            print(item)
    else:
        print(result)


if __name__ == "__main__":
    main()
