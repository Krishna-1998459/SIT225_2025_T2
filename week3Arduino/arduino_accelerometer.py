import sys
import traceback
from arduino_iot_cloud import ArduinoCloudClient
from datetime import datetime
import csv

# Your Device credentials from Arduino IoT Cloud
DEVICE_ID = "7f21a336-73ad-49b1-821e-e2bfe40e6912"
SECRET_KEY = "YsOfTIi1m8DCwAEVKSjVkUAAP"

# Open CSV file for writing
csv_file = open("accelerometer_data.csv", mode="a", newline="")
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["timestamp", "accelx", "accely", "accelz"])

# Shared dictionary to hold the latest values
accel_data = {"accelx": None, "accely": None, "accelz": None}

# Callback functions
def on_accelx_changed(client, value):
    accel_data["accelx"] = value
    write_to_csv()

def on_accely_changed(client, value):
    accel_data["accely"] = value
    write_to_csv()

def on_accelz_changed(client, value):
    accel_data["accelz"] = value
    write_to_csv()

# Write row to CSV when all three values are available
def write_to_csv():
    if all(v is not None for v in accel_data.values()):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        csv_writer.writerow([
            timestamp,
            accel_data["accelx"],
            accel_data["accely"],
            accel_data["accelz"]
        ])
        csv_file.flush()
        print(f"{timestamp} | X: {accel_data['accelx']}, Y: {accel_data['accely']}, Z: {accel_data['accelz']}")
        # Reset values to wait for next full set
        accel_data["accelx"] = accel_data["accely"] = accel_data["accelz"] = None

# Main function
def main():
    print("Starting accelerometer data collection...")

    # Instantiate the client
    client = ArduinoCloudClient(
        device_id=DEVICE_ID,
        username=DEVICE_ID,
        password=SECRET_KEY
    )

    # Register variables EXACTLY as they appear in Arduino IoT Cloud
    client.register("accelx", value=None, on_write=on_accelx_changed)
    client.register("accely", value=None, on_write=on_accely_changed)
    client.register("accelz", value=None, on_write=on_accelz_changed)

    # Start the client loop (runs forever)
    client.start()

# Exception handling
if __name__ == "__main__":
    try:
        main()
    except Exception:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        print("An error occurred:")
        traceback.print_exception(exc_type, exc_value, exc_traceback)
