import psutil
import argparse
import subprocess
    

class GeneralFunctions:
    @staticmethod
    def to_GB(bytes_size):
        """Convert bytes to GB, rounded to 2 decimal places."""
        return round(bytes_size / (1024 ** 3), 2)
    
    def get_disk_usage(self):
        """Return disk usage statistics."""
        disk_usage = psutil.disk_usage('/')
        return {
            "total": self.to_GB(disk_usage.total),
            "free": self.to_GB(disk_usage.free),
            "used": self.to_GB(disk_usage.used),
            "percent": disk_usage.percent
        }

    def get_ram_usage(self):
        """Return RAM usage statistics."""
        ram_usage = psutil.virtual_memory()
        return {
            "total": self.to_GB(ram_usage.total),
            "available": self.to_GB(ram_usage.available),
            "used": self.to_GB(ram_usage.used),
            "percent": ram_usage.percent
        }

    def get_cpu_usage(self, percore=False):
        """Return CPU usage statistics. Optionally return usage per core."""
        if percore:
            return {f"Core {i}": usage for i, usage in enumerate(psutil.cpu_percent(interval=0.5, percpu=True))}
        return {"cpu_percent": psutil.cpu_percent(interval=0.5)}

    def get_cpu_temperature(self):
        try:
            result = subprocess.run(['sensors'], capture_output=True, text=True, check=True)
            output = result.stdout

            temperatures = {}
            for line in output.splitlines():
                if '°C' in line and ('Core' in line or 'Tctl' in line or 'CPUTIN' in line):
                    parts = line.split(':')
                    if len(parts) > 1:
                        temp_label = parts[0].strip()
                        temp_value = parts[1].strip().split()[0]
                        temperatures[temp_label] = temp_value

            if not temperatures:
                return {"cpu_temp": "Temperature not found"}
            return {"cpu_temp": temperatures}
        except subprocess.CalledProcessError as e:
            return {"cpu_temp": f"Failed to retrieve temperature: {e}"}

def main():
    parser = argparse.ArgumentParser(description="System Resource Monitor - Disk, RAM, CPU usage and temperature stats.")
    
    parser.add_argument(
        "command", 
        choices=["cpu-usage", "cpu-usage:percore", "ram-usage", "disk-usage", "cpu-temp"], 
        help="Command to get system resource usage: 'cpu-usage', 'cpu-usage:percore', 'ram-usage', 'disk-usage', 'cpu-temp'"
    )
    
    args = parser.parse_args()
    
    gf = GeneralFunctions()

    if args.command == "cpu-usage":
        print(gf.get_cpu_usage(percore=False))
    elif args.command == "cpu-usage:percore":
        print(gf.get_cpu_usage(percore=True))
    elif args.command == "ram-usage":
        print(gf.get_ram_usage())
    elif args.command == "disk-usage":
        print(gf.get_disk_usage())
    elif args.command == "cpu-temp":
        print(gf.get_cpu_temperature())

if __name__ == "__main__":
    main()
