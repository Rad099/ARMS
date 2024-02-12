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
#include "battery.h"


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

// status service: used for notifying the app to stop creating AQI
BleUuid StatusService("");
BleUuid photonStatusCharUuid("");
BleUuid appStatusCharUuid("");

// battery service
BleUuid BatteryService("180F");
BleCharacteristic BatteryLevelCharacteristic("2A19", BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY);


BleCharacteristic photonStatus("Photon Status", BleCharacteristicProperty::NOTIFY, appStatusCharUuid, StatusService);
BleCharacteristic appStatus("App status", BleCharacteristicProperty::WRITE_WO_RSP, photonStatusCharUuid, StatusService, onDataReceived, NULL);

//  main loop for reading and notifying. New structure: Implement 1-hour and instant data sending to app. 
// Intervals: Real-time: every 5 minutes. Perform EMA - 1-hour: every 15/30 min. Perform arithmetic
void MainHandler();

// Handles band startup for collecting threshold data from app. RUNS ONCE in setup()
void BLEStartupHandler();

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);


// handles reading from AQI sensors and sends via BLE
void I2CHandler();


// pins
const uint8_t UV_pin = A1;
const uint8_t CO_pin = A2;
const uint8_t set_pin = D2;


bool threshConfigMode = true;
bool isSet = false;


// SECTION: global storage and variables
char sensorBuffer[12];


// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  Serial.begin(9600);


  // BLE section
  BLE.on();
  BLE.addCharacteristic(AQICharacteristic);
 

  BleAdvertisingData advData;

  // battery charging status interrupt
  pinMode(CHG, INPUT_PULLUP);
  attachInterrupt(CHG, chargingHandler, CHANGE);

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

  if (millis() - lastReadTime >= 300000) {
    // read sensors
    // Apply EMA
    // I2C from p750 reading requires 30 seconds
    I2CHandler();

    // after I2C, read CO sensor for 

    // combine values to one buffer
    // send via AQIChar

    // sleep for 5 minutes

    // repeat



    
    // reset timer
    lastReadTime = millis();
    
  } 

}


//void onDataReceived(const uint16_t* data, size_t len, const BlePeerDevice& peer, void* context) {
 // if (len == sizeof(userThresholds)) {
   // memcpy(&thresholds, data, sizeof(thresholds));
    
  //}
//}

void I2CHandler() {

  delay(30000);
  // TODO: Manage sleep mode for sensor

  uint16_t pm1 = readData(pm1Reg_H);
  uint16_t pm2_5 = readData(pm2_5Reg_H);
  uint16_t pm10  = readData(pm10Reg_H);
  uint16_t vocIAQI = readData(vocReg_H);
  uint16_t co2 = readData(co2Reg_H);
  // manage CO here for simplicity
  uint16_t co = 0;

  // serialization
  uint16_t values[] = {pm1, pm2_5, pm10, vocIAQI, co2, co};
  const size_t numValues = sizeof(values) / sizeof(values[0]);
  uint8_t data[numValues*2];

  for (size_t i = 0; i < numValues; ++i) {
    data[2*i] = values[i] >> 8; // high byte
    data[2*i + 1] = values[i] & 0xFF; // low byte

  }

  AQICharacteristic.setValue(data, sizeof(data));

  Serial.printlnf("PM1: %d, PM2.5: %d, PM10: %d, IAQ: %d", pm1, pm2_5, pm10, vocIAQI);
}


