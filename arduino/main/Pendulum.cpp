#include "Pendulum.h"

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

void Pendulum::initializeSetup()
{
  /* Holds the pendulum when the program starts
    and releases it after 10s to start taking
    measurements
  */
  if (isFirstRun)
  {
    digitalWrite(ELECTROMAGNET_PIN, HIGH);
    // Waits 6 seconds before starting measurement
    unsigned long startTime = millis();
    while (millis() - startTime < 6000)
    {

    }
    digitalWrite(ELECTROMAGNET_PIN, LOW);
  }

  isFirstRun = false; 
}

void Pendulum::holdReleasePendulum()
{
  /* Holds whem measurement is over, waits, 
     and releases again
  */
  static unsigned long holdStartTime = 0;
  static bool isWaiting = false;
  unsigned long currentTime = millis();

  if (!isWaiting)
  {
    digitalWrite(ELECTROMAGNET_PIN, HIGH);
    holdStartTime = currentTime;
    isWaiting = true;
  }

  // Wait 3 seconds before realeasing again
  if (isWaiting && (currentTime - holdStartTime >= 3000))
  {
    digitalWrite(ELECTROMAGNET_PIN, LOW);
    isWaiting = false;
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

void Pendulum::update()
{
  if (stopFlag) return; // ends the program after completed the repetitions

  int photoresistorReading = analogRead(PHOTORESISTOR_PIN);

  initializeSetup();

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
          Serial.print("Period: ");
          Serial.println(period, 3);

          // activate electromagnet
          holdReleasePendulum();

          detectionCounterSameOscillation = 0;
          measurements++;
          break;
      }
    }
  }

  stopMeasuring();
}