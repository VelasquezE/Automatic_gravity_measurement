#pragma once
#include <Arduino.h>
#include "config.h"

class Pendulum
{
private:
  int measurements {0};
  bool stopFlag {false};
  bool isFirstRun {true};
  unsigned long t0 {0}; //time of first pass in same oscillation
  unsigned long t1 {0}; // time of second pass in same oscillation
  int detectionCounterSameOscillation {0}; 

public:
  void update();
private:
  bool detectPendulum(int photoresistorReading);
  bool waitToMeasure();
  float calculatePeriod();
  void initializeSetup();
  void holdReleasePendulum();
  void stopMeasuring(); 
};

