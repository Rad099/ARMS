/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * University of Illinois Chicago
 * ECE Department
 * Main source code of the ARMS project.
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "BLE.h"
#include "thresholds.h"
#include "p750.h"


// Manual Cloud
SYSTEM_MODE(MANUAL);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

int PM1Thresh = 0; // TODO: implement
int PM2_5Thresh = 0; // TODO: implement
int PM10Thresh = 0; // TODO: implement
int VOCThresh = 0; // TODO: implement
int COThresh = 0; // TODO: implement
int UVThresh = 0; // TODO: implement 

volatile bool wakeUp = false;

void wakeUpISR() {
  wakeUp = true;
}

// pins
const int UV_pin = A1;
const int CO_pin = A2;
const int p750_read_pin = D0;
const int clock_pin = D1;

const int sleepTime = 20000;

SystemSleepConfiguration config;



// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  Serial.begin(9600);


  // BLE section
  //BLE.on();
  //ble_advertisement();
  //BLE.onConnected(onConnected, nullptr);
  //BLE.onDisconnected(onDisconnected, nullptr);



  // Threshold constants from app
  // add setting to change user profiling in app
  // Interrupt for threshold changes


  // p750 setup
 
  



  // SPEC CO setup
  pinMode(CO_pin, INPUT);
  attachInterrupt(CO_pin, wakeUpISR, RISING);

  

  // UV setup
  pinMode(UV_pin, INPUT);
  attachInterrupt(UV_pin, wakeUpISR, RISING);




}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  static int currentIndex = 0;
  int sensorValue = 0;
  unsigned long startTime = millis();
  bool sleep = true;

  // begin sensor reading

  while (millis() - startTime < sleepTime) {
    // Round Robin Read
    switch(currentIndex) {
      case 0:
      sensorValue = analogRead(UV_pin);
      break;
  
    }

    if (sensorValue >= 100) {
      sleep = false;
      break;
    }

    if (sleep) {
      config.mode(SystemSleepMode::STOP)
      .gpio(UV_pin, RISING)
      .duration(15min);
      System.sleep(config);
    }

    if (wakeUp) {
      wakeUp = false;
    }

  }

  


  // apply sleep modes when compatible

  // interrupts for thresholds notifications

  // interrupts for battery charging and depletion

  // interrupt for app connection

}
