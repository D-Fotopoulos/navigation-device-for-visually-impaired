#include <Wire.h>
#include <VL53L1X.h>

// The number of sensors in your system.
const uint8_t sensorCount = 2;
uint8_t last_distance[2]={0, 0};
uint8_t current_distance[2]={0, 0};
uint8_t elapsedTime=0;
float speed[2]={0, 0};

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = {2, 3};

VL53L1X sensors[sensorCount];

void setup()
{
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  pinMode(6, OUTPUT);  // Set pin 6 as an output

  // Disable/reset all sensors by driving their XSHUT pins low.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }

  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    sensors[i].setTimeout(10000);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors[i].setAddress(0x2A + i);

    sensors[i].startContinuous(50);
  }
}

void loop()
{
  delay(2000);
    unsigned long startTime = millis(); // Start time in milliseconds
    int minDistance = INT_MAX;  // Store closest distance
    int selectedSensor = -1;    // Store index of the best sensor

    const float speedThreshold = 1.5;   // Minimum speed to consider

    // First, read all sensor values and compute speed
    for (uint8_t i = 0; i < sensorCount; i++)
    {
      Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" Last_Distance: ");
        Serial.println(last_distance[i]);


        current_distance[i] = sensors[i].read();
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" Distance: ");
        Serial.print(current_distance[i]);

        if (sensors[i].timeoutOccurred()) {
            Serial.print(" TIMEOUT");
        }

        Serial.print("\t");

        // Calculate speed only if there's a previous valid reading
        if (last_distance[i] != 0) {
            speed[i] = (last_distance[i]-current_distance[i]) / (float)elapsedTime;
        } else {
            speed[i] = 0; // If no previous reading, set speed to zero
        }

        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" Speed: ");
        Serial.println(speed[i]);

        last_distance[i] = current_distance[i]; // Update last distance

    }

    // Find the closest valid sensor (only considering speed threshold)
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (speed[i] > speedThreshold) {  // Check speed condition
            if (current_distance[i] < minDistance) { // Choose closest
                minDistance = current_distance[i];
                selectedSensor = i;
            }
        }
    }

    // If a valid sensor is found, output tone
    if (selectedSensor != -1) {
        Serial.print("Selected Sensor: ");
        Serial.println(selectedSensor);
        digitalWrite(6, HIGH);
        //tone(6, 500, 10); // Output tone for the selected sensor
    } else {
        Serial.println("No valid sensor met the speed criteria.");
    }

    unsigned long endTime = millis(); // End time in milliseconds
    elapsedTime = endTime - startTime;

    Serial.print("Elapsed time: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    digitalWrite(6, LOW);
}
//learn to use other clocks

