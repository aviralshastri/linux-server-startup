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

        return temperatures

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