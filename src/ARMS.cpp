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
#include "p750.h"
#include "uv.h"


// Manual Cloud
SYSTEM_MODE(MANUAL);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);


// Structs instances from thresholds
//indices currentIndex;
//userThresholds thresholds;

// AQI service for sending AQI data
BleUuid AQIService("38bdd4ec-fee9-4e99-b045-a3911a7171cd");
// characteristic
BleUuid AQICharUuid("d6eedc89-58c6-4d64-8120-f31737032cd0");
BleCharacteristic AQICharacteristic("AQI", BleCharacteristicProperty::NOTIFY, AQICharUuid, AQIService);


// Service for retreiving thresholds
//BleUuid ThresholdService("74fa8bf8-f838-4ce2-97f3-fc618bd0031d");
//BleUuid ThreshCharUuid("3c1c90fb-5e1a-427b-9b43-17ce2d47ba9a");
// characterisitic
//BleCharacteristic pm1Char("Tresholds", BleCharacteristicProperty::WRITE_WO_RSP, ThreshCharUuid, ThresholdService, onDataReceived, NULL);

// status service: used for notifying the app to stop creating AQI
BleUuid StatusService("");
BleUuid photonStatusCharUuid("");
BleUuid appStatusCharUuid("");

BleCharacteristic photonStatus("Photon Status", BleCharacteristicProperty::NOTIFY, appStatusCharUuid, StatusService);
BleCharacteristic appStatus("App status", BleCharacteristicProperty::WRITE_WO_RSP, photonStatusCharUuid, StatusService, onDataReceived, NULL);

//  main loop for reading and notifying. New structure: Implement 1-hour and instant data sending to app. 
// Intervals: Real-time: every 5 minutes. Perform EMA - 1-hour: every 15/30 min. Perform arithmetic
void MainHandler();

// Handles band startup for collecting threshold data from app. RUNS ONCE in setup()
void BLEStartupHandler();

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

// set values
void waitForThresholds();
// change values during loop
void changeTreshISR(bool& mode, bool& isSet);


// pins
const uint8_t UV_pin = A1; // COMFIRM: confirm if uint8 works. switch back to int if not.
const uint8_t CO_pin = A2;
const uint8_t p750_read_pin = D0;
const uint8_t clock_pin = D1;

bool threshConfigMode = true;
bool isSet = false;

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  //Serial.begin(9600);


  // BLE section
  BLE.on();
  BLE.addCharacteristic(AQICharacteristic);
 

  BleAdvertisingData advData;

  advData.appendServiceUUID(AQIService);
  
  
  // Threshold constants from app
  // add setting to change user profiling in app
  // Interrupt for threshold changes


  // p750 setup
  setupI2C();

  
  // SPEC CO setup
  pinMode(CO_pin, INPUT);
  

  // UV setup
  pinMode(UV_pin, INPUT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // begin sensor reading
  MainHandler();
  

  // interrupts for thresholds notifications

  // interrupts for battery charging and depletion

  // interrupt for app connection.... correction: botton interrupt to wake up photon

}

void MainHandler() {
  static unsigned long lastReadTime = 0;
  //static bool inConfirmMinute = false;
  static uint16_t pm1_val = 0;
  static uint16_t pm2_5_val = 0;
  static uint16_t pm10_val = 0;
  static uint16_t voc_val = 0;
  static uint16_t co_val = 0;
  static uint16_t uv_val = 0;

  if (millis() - lastReadTime >= 300000) {
    // read sensors
    // Apply EMA
    // I2C from p750 reading requires 30 seconds

    // after I2C, read CO sensor for 

    // combine values to one buffer

    // send via AQIChar

    // sleep for 5 minutes

    // repeat



    
    // reset timer
    lastReadTime = millis();
    
  } 

}


void onDataReceived(const uint16_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  if (len == sizeof(userThresholds)) {
    memcpy(&thresholds, data, sizeof(thresholds));

  }
}




// change values during loop
void changeTreshISR(bool& mode, bool& isSet);
