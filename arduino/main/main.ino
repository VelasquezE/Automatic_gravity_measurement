#include "Pendulum.h"

Pendulum pendulum;

void setup() {
  Serial.begin(9600);
  pinMode(ELECTROMAGNET_PIN, OUTPUT);
}

void loop() {
  pendulum.update();
}



