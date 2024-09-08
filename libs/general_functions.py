import psutil
import argparse

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
        """Return the CPU temperature if available, otherwise provide a fallback message."""
        try:
            temps = psutil.sensors_temperatures()
            if not temps:
                return "Temperature sensors not found."
            
            cpu_temp = temps.get('coretemp')
            if cpu_temp:
                return {f"Sensor {i}": f"{entry.current}°C" for i, entry in enumerate(cpu_temp)}
            else:
                return "CPU temperature sensor not detected."
        except (AttributeError, KeyError, Exception) as e:
            return f"Error retrieving temperature: {str(e)}"

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
