import serial
import time

# Define the serial port and baud rate
SERIAL_PORT = 'COM4'  # Change to your Arduino's serial port
BAUD_RATE = 9600

# Open serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

def send_message(message):
    """Send a message to the Arduino."""
    ser.write((message + '\n').encode())

try:
    while True:
        message = input("Enter the message to send: ")
        send_message(message)
        time.sleep(1)

except KeyboardInterrupt:
    print("Program interrupted.")

finally:
    ser.close()
