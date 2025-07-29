#include "thingProperties.h"
#include <Arduino_LSM6DS3.h>

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(1500);  // Give time for Serial Monitor to connect

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Debug level for connection status and errors (0–4)
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Initialize the IMU sensor
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true);  // Stop execution
  }

  Serial.println("IMU initialized successfully.");
}

void loop() {
  ArduinoCloud.update();

  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    // Send to IoT Cloud
    accelX = x;
    accelY = y;
    accelZ = z;

    // Print to Serial Monitor
    Serial.print("Accel X: "); Serial.print(x, 3);
    Serial.print(" | Y: "); Serial.print(y, 3);
    Serial.print(" | Z: "); Serial.println(z, 3);
  }

  delay(500);  // Adjust sampling rate
}

// These callback functions are defined for cloud syncing (optional)
void onXchange() {
  Serial.println("accelX changed on cloud.");
}

void onYchange() {
  Serial.println("accelY changed on cloud.");
}

void onZchange() {
  Serial.println("accelZ changed on cloud.");
}



