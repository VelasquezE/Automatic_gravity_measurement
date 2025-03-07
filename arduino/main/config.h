#pragma once

// Pins
constexpr int PHOTORESISTOR_PIN = A0;
constexpr int ELECTROMAGNET_PIN = 2; 

// Constants
constexpr unsigned long WAITING_TIME = 80000; // [\mu s]
constexpr int THRESHOLD = 400;
constexpr int REPETITIONS = 100;
constexpr unsigned long FORCED_RELEASE_INTERVAL = 10000;
constexpr int TIME_OFF_ON = 3000;

