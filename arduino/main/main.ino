// photoresistor
const int photoresistorPin = A0;
int photoresistorReading;
const int threshold = 400;
unsigned long t0 = 0;
unsigned long t1 = 0;
float halfPeriod = 0;
int ii = 0; // counts pendulum detection

//Avoiding detecting the pendulum twice
unsigned long lastDetection = 0; //
const unsigned long waitingTime = 50000; // [\mu s]

// electromagnet
const int electromagnetPin = 2; 

void setup() {
  Serial.begin(9600);
  pinMode(electromagnetPin, OUTPUT);
}

void loop() {
  photoresistorReading = analogRead(photoresistorPin);
  
  if (photoresistorReading < threshold)
  {
    if ((micros() - lastDetection) > waitingTime)
    {
      lastDetection = micros();
      switch (ii){
        case 0:
          t0 = micros();
          ii++;
          break;
        case 1:
          t1 = micros();
          halfPeriod = (t1 - t0) / 1000.0; //[ms]
          Serial.print("Half period: ");
          Serial.println(halfPeriod, 3);

          // activate electromagnet
          digitalWrite(electromagnetPin, HIGH);
          delay(3000); //waits 2 seconds before realeasing the pendulum
          digitalWrite(electromagnetPin, LOW);

          ii = 0;
          break;
      }
    }
  }

  delay(1);
}


