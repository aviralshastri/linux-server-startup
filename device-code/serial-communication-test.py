import serial

# Adjust these parameters based on your setup
serial_port = 'COM4'  # Change this to your serial port, e.g., COM3 on Windows
baud_rate = 9600

# Open the serial port
ser = serial.Serial(serial_port, baud_rate, timeout=1)

print("Starting to read serial data...")

try:
    while True:
        if ser.in_waiting > 0:
            message = ser.readline()
            print(f"Received: {message}")
except KeyboardInterrupt:
    print("Program interrupted.")
finally:
    ser.close()
