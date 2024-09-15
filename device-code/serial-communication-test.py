import serial
import time

# Replace 'COM3' with your actual COM port
ser = serial.Serial('COM4', 115200, timeout=1)

try:
    while True:
        # Send a message to ESP32
        message = input("Enter message to send: ")
        ser.write(message.encode('utf-8'))
        
        # Wait for the response from ESP32
        time.sleep(1)
        response = ser.read(ser.in_waiting or 1).decode('utf-8')
        print(f"Received: {response}")

except KeyboardInterrupt:
    print("Program terminated")

finally:
    ser.close()
