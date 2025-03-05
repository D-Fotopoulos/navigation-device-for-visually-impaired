#include <Wire.h>
#include <VL53L1X.h>

// The number of sensors in your system.
const uint8_t sensorCount = 2;
uint8_t last_distance[2]={0, 0};
uint8_t elapsedTime=0;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = { 2, 3};

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
  unsigned long startTime = millis(); // Start time in milliseconds
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    uint8_t current=sensors[i].read();
    Serial.print(current);
    if (sensors[i].timeoutOccurred()) { 
      Serial.print(" TIMEOUT"); 
      //pinMode(xshutPins[i], OUTPUT);
      //digitalWrite(xshutPins[i], LOW);
      //Serial.print(" RESET");
     } // Reinitialize the sensor if necessary}
    Serial.print('\t');      // Wait 1 second
    if(last_distance[i]!=0){
      float speed=(last_distance[i]-current)/elapsedTime;
      Serial.print("This is speed: ");
      Serial.print(speed);
    }
    

    last_distance[i]=current;//assign it to the last distance after all calculations
    
  }

  Serial.println();
  tone(6, 500);
  unsigned long endTime = millis(); // End time in milliseconds
  elapsedTime = endTime - startTime;

  Serial.print("Elapsed time: ");
  Serial.print(elapsedTime);
  Serial.println(" ms");

}
