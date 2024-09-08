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
        
        temperatures = {}
        try:
            temps = psutil.sensors_temperatures()

            if 'coretemp' in temps:
                temperatures['coretemp'] = [temp.current for temp in temps['coretemp']]
            if 'k10temp' in temps:
                temperatures['k10temp'] = [temp.current for temp in temps['k10temp']]
            if 'amdgpu' in temps:
                temperatures['amdgpu'] = [temp.current for temp in temps['amdgpu']]
            if 'nct6795' in temps:
                temperatures['nct6795'] = [temp.current for temp in temps['nct6795']]
        
        except Exception as e:
            temperatures['psutil_error'] = str(e)
        
        try:
            result = subprocess.run(['sensors'], capture_output=True, text=True, check=True)
            output = result.stdout

            for line in output.splitlines():
                if '°C' in line:
                    if 'Tctl' in line:
                        temp_label = 'Tctl'
                        temp_value = line.split(':')[1].strip().split()[0]
                        temperatures[temp_label] = temp_value
                    elif 'CPUTIN' in line:
                        temp_label = 'CPUTIN'
                        temp_value = line.split(':')[1].strip().split()[0]
                        temperatures[temp_label] = temp_value

        except subprocess.CalledProcessError as e:
            temperatures['sensors_error'] = str(e)

        if not temperatures:
            temperatures['status'] = 'Temperature data not available'

        return {"cpu_temp": temperatures}
    
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
