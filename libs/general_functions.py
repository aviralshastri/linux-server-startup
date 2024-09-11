import psutil
import subprocess
import argparse

class General:
    def __init__(self):
        pass
    
    def get_cpu_temperature(self):
        temperatures = {}
        try:
            temps = psutil.sensors_temperatures()
            for sensor in ['coretemp', 'k10temp', 'amdgpu', 'nct6795']:
                if sensor in temps:
                    temperatures[sensor] = [temp.current for temp in temps[sensor]]
        except Exception as e:
            temperatures['psutil_error'] = str(e)

        try:
            result = subprocess.run(['sensors'], capture_output=True, text=True, check=True)
            output = result.stdout
            for line in output.splitlines():
                if '°C' in line:
                    label, value = self._parse_sensor_line(line)
                    if label:
                        temperatures[label] = value
        except subprocess.CalledProcessError as e:
            temperatures['sensors_error'] = str(e)

        if not temperatures:
            temperatures['status'] = 'Temperature data not available'

        return temperatures

    def _parse_sensor_line(self, line):
        """
        Parse a line from the 'sensors' command output to extract temperature labels and values.
        """
        if 'Tctl' in line:
            return 'Tctl', line.split(':')[1].strip().split()[0]
        elif 'CPUTIN' in line:
            return 'CPUTIN', line.split(':')[1].strip().split()[0]
        return None, None

def main():
    parser = argparse.ArgumentParser(description="General stats monitoring - temperature stats.")
    
    parser.add_argument(
        "command", 
        choices=["cpu-temp"], 
        help="Command to get system resource usage: 'cpu-temp'"
    )
    
    args = parser.parse_args()
    
    gen = General()

    if args.command == "cpu-temp":
        print(gen.get_cpu_temperature())

if __name__ == "__main__":
    main()
