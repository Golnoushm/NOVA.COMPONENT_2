/*********************************************************************
 * DF Pong Controller with Pressure Sensors
 * 
 * This program implements a Bluetooth Low Energy controller for Pong,
 * using two pressure sensors to control UP and DOWN movements.
 *********************************************************************/

#include <ArduinoBLE.h>
#include "ble_functions.h"
#include "buzzer_functions.h"

// Name your controller!
const char* deviceName = "NOVA COMPONENT 2";

// Pin definitions
const int PRESSURE_UP_PIN = A0;   // Analog pin for UP movement (Pressure Sensor 1)
const int PRESSURE_DOWN_PIN = A1; // Analog pin for DOWN movement (Pressure Sensor 2)
const int BUZZER_PIN = 11;        // Pin for haptic feedback buzzer
const int LED_PIN = LED_BUILTIN;  // Status LED pin

// Movement state tracking
int currentMovement = 0; // Current movement value (0=none, 1=up, 2=down)

// Threshold values for pressure sensors
const int UP_THRESHOLD = 512;    // Threshold for UP movement
const int DOWN_THRESHOLD = 512;  // Threshold for DOWN movement

void setup()
{
  Serial.begin(9600);

  // Configure pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize BLE with device name and status LED
  setupBLE(deviceName, LED_PIN);

  // Initialize buzzer
  setupBuzzer(BUZZER_PIN);

  Serial.println("System initialized. Waiting for pressure sensor inputs...");
}

void loop()
{
  // Update BLE connection status and handle incoming data
  updateBLE();

  // Read sensor inputs and determine movement state
  handleInput();

  // Send the movement state to P5
  sendMovement(currentMovement);

  // Provide feedback with the buzzer
  updateBuzzer(currentMovement);
}

void handleInput()
{
  // Read pressure sensor values
  int upSensorValue = analogRead(PRESSURE_UP_PIN);
  int downSensorValue = analogRead(PRESSURE_DOWN_PIN);

  // Debug: Print sensor values for calibration
  Serial.print("UP Sensor: ");
  Serial.print(upSensorValue);
  Serial.print(" (Threshold: ");
  Serial.print(UP_THRESHOLD);
  Serial.print("), DOWN Sensor: ");
  Serial.print(downSensorValue);
  Serial.print(" (Threshold: ");
  Serial.println(DOWN_THRESHOLD);

  // Determine movement state based on thresholds
  if (upSensorValue > UP_THRESHOLD && downSensorValue <= DOWN_THRESHOLD)
  {
    currentMovement = 1; // UP movement
  }
  else if (downSensorValue > DOWN_THRESHOLD && upSensorValue <= UP_THRESHOLD)
  {
    currentMovement = 2; // DOWN movement
  }
  else
  {
    currentMovement = 0; // No movement
  }
}
