#include "thingProperties.h"
#include <Arduino_LSM6DS3.h>

const float Z_THRESHOLD = 0.40;   // Z threshold to trigger alarm
const unsigned long RESET_DELAY = 10000;  // 10 seconds for auto-reset

unsigned long alarmStartTime = 0;
bool alarmActive = false;

void setup() {
  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true);
  }

  Serial.println("IMU initialized successfully.");
}

void loop() {
  ArduinoCloud.update();

  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    accelX = x;
    accelY = y;
    accelZ = z;

    Serial.print("Accel X: "); Serial.print(x, 3);
    Serial.print(" | Y: "); Serial.print(y, 3);
    Serial.print(" | Z: "); Serial.println(z, 3);

    // Trigger alarm
    if (z > Z_THRESHOLD && !alarmActive) {
      alarmTrigger = true;
      alarmActive = true;
      alarmStartTime = millis();
      Serial.println("Alarm Triggered");
    }

    // Auto reset
    if (alarmActive && (millis() - alarmStartTime > RESET_DELAY)) {
      alarmTrigger = false;
      alarmActive = false;
      Serial.println("Alarm auto-reset after timeout");
    }

    // Manual reset
    if (alarmOff) {
      alarmTrigger = false;
      alarmActive = false;
      alarmOff = false;
      Serial.println("Alarm manually reset from dashboard");
    }

    // Reversed GOOD/BAD status logic
    if (z < Z_THRESHOLD) {
      if (Status != "BAD") {
        Status = "BAD";
        Serial.println("Status updated: BAD");
      }
    } else {
      if (Status != "GOOD") {
        Status = "GOOD";
        Serial.println("Status updated: GOOD");
      }
    }
  }

  delay(500);
}

// ---------------- Callback Functions ---------------- //

void onAccelXChange() {}
void onAccelYChange() {}
void onAccelZChange() {}

void onAlarmTriggerChange() {
  Serial.print("Alarm trigger changed to: ");
  Serial.println(alarmTrigger ? "ON" : "OFF");
}

void onAlarmOffChange() {
  Serial.println("Alarm reset toggled");
}

void onStatusChange() {
  Serial.print("Status changed to: ");
  Serial.println(Status);
}
