import psutil
import argparse


class ResourceUsage:
    @staticmethod
    def to_gb(bytes_size):
        """Convert bytes to GB, rounded to 2 decimal places."""
        return round(bytes_size / (1024 ** 3), 2)
    
    def get_disk_usage(self):
        """Return disk usage statistics."""
        disk_usage = psutil.disk_usage('/')
        return {
            "total": self.to_gb(disk_usage.total),
            "free": self.to_gb(disk_usage.free),
            "used": self.to_gb(disk_usage.used),
            "percent": disk_usage.percent
        }

    def get_ram_usage(self):
        """Return RAM usage statistics."""
        ram_usage = psutil.virtual_memory()
        return {
            "total": self.to_gb(ram_usage.total),
            "available": self.to_gb(ram_usage.available),
            "used": self.to_gb(ram_usage.used),
            "percent": ram_usage.percent
        }

    def get_cpu_usage(self, percore=False):
        """Return CPU usage statistics. Optionally return usage per core."""
        if percore:

            return {f"Core {i}": usage for i, usage in enumerate(psutil.cpu_percent(interval=0.5, percpu=True))}
        return {"cpu_percent": psutil.cpu_percent(interval=0.5)}


def main():
    parser = argparse.ArgumentParser(description="System Resource Monitor - Disk, RAM, and CPU usage stats.")
    
    parser.add_argument(
        "command", 
        choices=["cpu-usage", "cpu-usage:percore", "ram-usage", "disk-usage"], 
        help="Command to get system resource usage: 'cpu-usage', 'cpu-usage:percore', 'ram-usage', 'disk-usage'"
    )
    
    args = parser.parse_args()
    
    resource_usage = ResourceUsage()


    command_map = {
        "cpu-usage": lambda: resource_usage.get_cpu_usage(percore=False),
        "cpu-usage:percore": lambda: resource_usage.get_cpu_usage(percore=True),
        "ram-usage": resource_usage.get_ram_usage,
        "disk-usage": resource_usage.get_disk_usage
    }

    result = command_map[args.command]()
    print(result)


if __name__ == "__main__":
    main()
