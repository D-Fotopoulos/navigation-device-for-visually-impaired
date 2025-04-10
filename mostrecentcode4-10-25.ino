#include <Wire.h>
#include <VL53L1X.h>
#include "Timer1tone.h"
const uint8_t sensorCount = 3;

VL53L1X sensors[sensorCount];

const uint8_t xshutPins[sensorCount] = { 11,12,13 };
const uint8_t BATTERYCHECKPIN = 14;

void playBeepingTone(int side, float frequency, int intensity);


Timer1Tone timer1(9);

void setup()
{
  int playing = 0;
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

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

    sensors[i].setTimeout(500);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors[i].setDistanceMode(VL53L1X::Long);
    sensors[i].setMeasurementTimingBudget(50000);
    //sensors[i].setROISize(100,100);

    sensors[i].setAddress(0x2A + i);

    sensors[i].startContinuous(100);
  }
}
const float MAXRANGE = 4000;
const float FREQRANGE = 1000;
const float MINFREQ = 600;
float lastdistances[sensorCount] = {};

int BATTERYLOW = 0;
int BATTERYCRITICAL = 0;



void loop()
{
  /*
  int BATTERYVOLTAGE = analogRead(BATTERYCHECKPIN);
  if(BATTERYLOW == 0)
  {
    if(BATTERYVOLTAGE < 680)
    {
      BATTERYLOW = 1;
      //play sound to indicate to user
      tone(5, 700);
      timer1.begin(900);
      delay(2000);
      timer1.stop();
      

    }
  }
  Serial.print("This is the battery: "); Serial.println(BATTERYVOLTAGE);
  if(BATTERYCRITICAL == 0)
  {
    if(BATTERYVOLTAGE < 640)
    {
      BATTERYCRITICAL = 1;
      //play sound to indicate to user
      tone(5, 400);
      timer1.begin(600);
      delay(2000);
      timer1.stop();
    }
  }
  if((BATTERYCRITICAL==1 | BATTERYLOW == 1)&& BATTERYVOLTAGE>700){
    BATTERYCRITICAL = 0;
    BATTERYLOW = 0;
  }



  */
  int playing = 0;
  float distances[sensorCount]  = {};
  float speeds[sensorCount] = {};
  int selectedSensor = -1;
  for(int i = 0; i < sensorCount; i++)
  {  
    distances[i] = sensors[i].read();

    speeds[i] = distances[i] - lastdistances[i];

    Serial.print(distances[i]);
    Serial.print("      ");
    Serial.print("      ");
    Serial.print(speeds[i]);
    Serial.print("      ");
    Serial.print("      ");
    if (sensors[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }


    lastdistances[i] = distances[i];
  }


  //float distance = sum/5;
  float minDistance = 100       000;
  /*for(int i = 1; i < sensorCount; i++)
  {
    //float frequency = (1 - (distances[i] / MAXRANGE))*FREQRANGE + MINFREQ;
    if(distances[i] < minDistance)
    {
      playtone
    }
  } 
  */

  for(int i = 0; i < sensorCount; i++)
  {
    if(speeds[i] < -30 && distances[i] < minDistance && distances[i] < 2500.0)
    {
      minDistance = distances[i];
      selectedSensor = i;
    }

  }
  
  //float frequency = (1 - (minDistance / MAXRANGE))*FREQRANGE + MINFREQ;

  //side dependent frequencies
  float frontfrequency = 900;
  float backfrequency = 500;

  int beepdelay = minDistance / 12;


  if(selectedSensor != -1)
  {
    //tone(5,frequency - 300);

    // use timer1 for left

    //use tone for right

      switch(selectedSensor){

        //object detected on the right
        case 0:

          for(int i = 0; i < 2;i++)
          {
            tone(5, frontfrequency);
            delay(50);
            noTone(5);
            delay(beepdelay);
          }
          
          
        break;
        //object on left
        case 1:

          for(int i = 0; i < 2;i++)
          {
            timer1.begin(frontfrequency);
            delay(50);
            timer1.stop();
            delay(beepdelay);
          }
        break;
        //in front
        case 2:

          for(int i = 0; i < 2;i++)
          {
            tone(5, frontfrequency);
            timer1.begin(frontfrequency);
            delay(50);
            noTone(5);
            timer1.stop();
            delay(beepdelay);
          }
        break;
        //behind
        case 3:
          for(int i = 0; i < 2;i++)
          {
            tone(5, backfrequency);
            timer1.begin(backfrequency);
            delay(50);
            noTone(5);
            timer1.stop();
            delay(beepdelay);
          }
        break;
        default:
          tone(5,100);

      }

    //timer1.begin(frequency);
    Serial.print("Selected Sensor: ");
    Serial.print(selectedSensor);
    playing = 2000;
  }
  else
  {
    noTone(5);
    timer1.stop();
    Serial.print("Threshold not met");
  }
  
  Serial.print("     ");
  Serial.print(playing);
  //Serial.print(frequency);
  
  Serial.println();
}


