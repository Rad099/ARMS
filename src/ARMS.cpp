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
#include "uv.h"


// Manual Cloud
SYSTEM_MODE(MANUAL);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// I2C Command registers
const uint8_t pm1Reg = 0x00;
const uint8_t pm2_5Reg = 0x02;
const uint8_t pm10Reg = 0x04;
const uint8_t vocReg = 0x20;
const uint8_t I2CAddr = 0x12;


struct indices {
  u_int16_t PM1; 
  u_int16_t PM2_5; 
  u_int16_t PM10; 
  u_int16_t VOC; 
  u_int16_t CO; 
  u_int16_t UV;
};



indices currentIndex;
userThresholds* thresholds;

// AQI service for sending AQI data
BleUuid AQIService("38bdd4ec-fee9-4e99-b045-a3911a7171cd");
// characteristic
BleUuid AQICharUuid("d6eedc89-58c6-4d64-8120-f31737032cd0");
BleCharacteristic AQICharacteristic("AQI", BleCharacteristicProperty::NOTIFY, AQICharUuid, AQIService);


// Service for retreiving thresholds
BleUuid ThresholdService("");
BleUuid ThreshUuid("");
// characterisitic
BleCharacteristic pm1Char("Tresholds", BleCharacteristicProperty::WRITE_WO_RSP, ThreshUuid, ThresholdService, onDataReceived, NULL);


void MainHandler(); //  main loop for reading and notifying
void BLEStartupHandler(); // Handles band startup for collecting threshold data from app. RUNS ONCE in setup()
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
// set values
void waitForThresholds();
// change values during loop
void changeTreshISR(bool& mode, bool& isSet);




// pins
const int UV_pin = A1;
const int CO_pin = A2;
const int p750_read_pin = D0;
const int clock_pin = D1;

const int sleepTime = 20000;
const int bufferTime = 5000;


bool threshConfigMode = true;
bool isSet = false;

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  //Serial.begin(9600);


  // BLE section
  BLE.on();
  // setup thresholds



  BLE.addCharacteristic(AQICharacteristic);
 

  BleAdvertisingData advData;

  advData.appendServiceUUID(AQIService);
  BLEStartupHandler();



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
  static bool inConfirmMinute = false;

 // if (millis() - lastReadTime > (inConfirmMinute ? 1000: 60000)) {

      // read the sesnors

    
    
 // }



}


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  if (len == sizeof(userThresholds*)) {
    memcpy(&thresholds, data, sizeof(thresholds));

  }
}

// set values
//void waitForThresholds() {

//}



// change values during loop
void changeTreshISR(bool& mode, bool& isSet);
