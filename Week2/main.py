import serial
import time

# Serial port configuration
ser = serial.Serial('COM19', 9600)  # Replace with your port
time.sleep(2)  # Allow time for Arduino to reset

# File to store data
filename = "DHT22_data.csv"

with open(filename, 'a') as file:
    while True:
        try:
            if ser.in_waiting:
                raw_data = ser.readline().decode('utf-8').strip()
                timestamp = time.strftime('%Y%m%d%H%M%S')
                file.write(f"{timestamp},{raw_data}\n")
                print(f"{timestamp},{raw_data}")
        except KeyboardInterrupt:
            print("Stopped by user.")
            break
        except Exception as e:
            print(f"Error: {e}")
            break

ser.close()
