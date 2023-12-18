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
#include "Particle.h"
#include "BLE.h"

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

volatile bool wakeUp = false;

void wakeUpISR() {
  wakeUp = true;
}

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  Serial.begin(9600);


  // BLE section
  BLE.on();
  ble_advertisement();
  BLE.onConnected(onConnected, nullptr);
  BLE.onDisconnected(onDisconnected, nullptr);



  // Threshold constants from app
  // add setting to change user profiling in app
  // Interrupt for threshold changes


  // p750 setup



  // SPEC CO setup

  

  // UV setup




}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  // begin sensor reading

  // apply sleep modes when compatible

  // interrupts for thresholds notifications

  // interrupts for battery charging and depletion

}
