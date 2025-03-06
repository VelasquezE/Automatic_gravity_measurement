#include "Pendulum.h"

void Pendulum::update()
{  
  if (stopFlag) return;
  
  int photoresistorReading = analogRead(PHOTORESISTOR_PIN);

  if (detectPendulum(photoresistorReading))
  {
    if (waitToMeasure())
    { 
      switch (detectionCounterSameOscillation){
        case 0:
          t0 = micros();
          detectionCounterSameOscillation++;
          break;
        case 1:
          t1 = micros();

          float period = calculatePeriod();
          Serial.println(period, 4);

          detectionCounterSameOscillation = 0;
          measurements++;
          break;
      }
    }
    
  }

  holdReleasePendulum();
  
  stopMeasuring();
}

bool Pendulum::detectPendulum(int photoresistorReading)
{
  if (photoresistorReading < THRESHOLD)
  {
    return true;
  }

  return false;
}

bool Pendulum::waitToMeasure()
{
  /* Due to the speed of the measurements, avoids detecting the 
     pendulum twice by waiting a set amount of time
  */
  static unsigned long lastDetection = 0;

  if ((micros() - lastDetection) > WAITING_TIME)
  {
    lastDetection = micros();
    return true;
  }

  return false;
}

float Pendulum::calculatePeriod()
{
  float period = 0;
  period = (2 * (t1 - t0)) * 1e-6; // [s]

  return period;
}

void Pendulum::holdReleasePendulum()
{
  /* Holds whem measurement is over, waits, 
     and releases again
  */
  unsigned long currentTime = millis();
  static int lastState = 0;
  static unsigned long timeElectromagnetTurnOn = 0;

  if (currentTime - timeElectromagnetTurnOn > 3000)
  {
    timeElectromagnetTurnOn = currentTime;

    if (lastState == 0)
    {
      digitalWrite(ELECTROMAGNET_PIN, HIGH);
      lastState = 1;
    }
    else
    {
      digitalWrite(ELECTROMAGNET_PIN, LOW);
      lastState = 0;
    }
  }  
  
}

void Pendulum::stopMeasuring()
{
  if (measurements >= REPETITIONS)
  {
    digitalWrite(ELECTROMAGNET_PIN, LOW);
    stopFlag = true;
    isFirstRun = true; 
  }
}


