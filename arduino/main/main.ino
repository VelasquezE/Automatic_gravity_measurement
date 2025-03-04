int measurements = 0;
bool stopFlag = false;

unsigned long t0 = 0; //time of first pass in same oscillation
unsigned long t1 = 0; // time of second pass in same oscillation

int detectionCounterSameOscillation = 0; 

// photoresistor
const int photoresistorPin = A0;
int photoresistorReading;

// electromagnet
const int electromagnetPin = 2; 

bool detectPendulum(int photoresistorReading);
bool waitToMeasure();
float calculatePeriod();
void initializeSetup();
void holdReleasePendulum();
void stopMeasuring();

void setup() {
  Serial.begin(9600);
  pinMode(electromagnetPin, OUTPUT);
}

void loop() {
  if (stopFlag) return; // ends the program after completed the repetitions

  photoresistorReading = analogRead(photoresistorPin);
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

  delay(1);
}

bool detectPendulum(int photoresistorReading)
{
  const int threshold = 400;

  if (photoresistorReading < threshold)
  {
    return true;
  }

  return false;
}

bool waitToMeasure()
{
  /* Due to the speed of the measurements, avoids detecting the 
     pendulum twice by waiting a set amount of time
  */
  static unsigned long lastDetection = 0;
  const unsigned long waitingTime = 50000; // [\mu s]

  if ((micros() - lastDetection) > waitingTime)
  {
    lastDetection = micros();
    return true;
  }

  return false;
}

float calculatePeriod()
{
  float period = 0;
  period = (2 * (t1 - t0)) * 1e-6; // [s]

  return period;
}

void initializeSetup()
{
  /* Holds the pendulum when the program starts
    and releases it after 10s to start taking
    measurements
  */
  static bool launchSystem = true;

  if (launchSystem)
  {
    digitalWrite(electromagnetPin, HIGH);
    delay(10000); 
    digitalWrite(electromagnetPin, LOW);
  }

  launchSystem = false; 
}

void holdReleasePendulum()
{
  /* Holds whem measurement is over, waits, 
     and releases again
  */
  digitalWrite(electromagnetPin, HIGH);
  delay(3000); //waits 3 seconds before realeasing the pendulum
  digitalWrite(electromagnetPin, LOW);

}

void stopMeasuring()
{
  const int repetitions = 5;
  if (measurements >= repetitions)
  {
    digitalWrite(electromagnetPin, LOW);
    stopFlag = true;
  }
}

