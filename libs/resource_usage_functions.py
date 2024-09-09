import psutil
import argparse
    

class Resource_Usage:
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
    
    
    
    
def main():
    parser = argparse.ArgumentParser(description="System Resource Monitor - Disk, RAM and CPU usage stats.")
    
    parser.add_argument(
        "command", 
        choices=["cpu-usage", "cpu-usage:percore", "ram-usage", "disk-usage"], 
        help="Command to get system resource usage: 'cpu-usage', 'cpu-usage:percore', 'ram-usage', 'disk-usage'"
    )
    
    args = parser.parse_args()
    
    usg = Resource_Usage()

    if args.command == "cpu-usage":
        print(usg.get_cpu_usage(percore=False))
    elif args.command == "cpu-usage:percore":
        print(usg.get_cpu_usage(percore=True))
    elif args.command == "ram-usage":
        print(usg.get_ram_usage())
    elif args.command == "disk-usage":
        print(usg.get_disk_usage())

if __name__ == "__main__":
    main()
