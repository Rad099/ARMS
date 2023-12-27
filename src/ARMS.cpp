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

// service
BleUuid AQIService("38bdd4ec-fee9-4e99-b045-a3911a7171cd");

// characteristics
BleUuid vocCharUuid("d6eedc89-58c6-4d64-8120-f31737032cd0");
BleUuid pm2_5CharUuid("a1378902-6397-4f84-ac7b-0271e6f0ee17");
BleUuid pm10CharUuid("ab3386a7-68a2-43c2-97ff-5ddc95a0ca43");
BleUuid pm1CharUuid("e5e09d95-f6ef-45b9-8703-9ed4eb642131");
BleUuid coCharUuid("3f0f59d8-e00d-47c6-9dc2-42007f79f4b8");
BleUuid uvCharUuid("643456f8-d08e-4f33-abea-0c8fb65cdfad");

BleCharacteristic vocCharacteristic("voc", BleCharacteristicProperty::NOTIFY, vocCharUuid, AQIService);
BleCharacteristic pm2_5Characteristic("pm2_5", BleCharacteristicProperty::NOTIFY, pm2_5CharUuid, AQIService);
BleCharacteristic pm1Characteristic("pm1", BleCharacteristicProperty::NOTIFY, pm1CharUuid, AQIService);
BleCharacteristic pm10Characteristic("pm10", BleCharacteristicProperty::NOTIFY, pm10CharUuid, AQIService);
BleCharacteristic coCharacteristic("co", BleCharacteristicProperty::NOTIFY, coCharUuid, AQIService);
BleCharacteristic uvCharacteristic("uv", BleCharacteristicProperty::NOTIFY, uvCharUuid, AQIService);


volatile bool wakeUp = false;

void wakeUpISR() {
  wakeUp = true;
}

void MainHandler();

// pins
const int UV_pin = A1;
const int CO_pin = A2;
const int p750_read_pin = D0;
const int clock_pin = D1;

const int sleepTime = 20000;
const int bufferTime = 5000;

SystemSleepConfiguration config;



// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here


  Serial.begin(9600);


  // BLE section
  BLE.on();
  BLE.addCharacteristic(uvCharacteristic);
  BLE.addCharacteristic(coCharacteristic);
  BLE.addCharacteristic(pm10Characteristic);
  BLE.addCharacteristic(pm1Characteristic);
  BLE.addCharacteristic(pm2_5Characteristic);
  BLE.addCharacteristic(vocCharacteristic);


  BleAdvertisingData advData;

  advData.appendServiceUUID(AQIService);
  BLE.advertise(&advData);
  //ble_advertisement();
  BLE.onConnected(onConnected, nullptr);
  BLE.onDisconnected(onDisconnected, nullptr);



  // Threshold constants from app
  // add setting to change user profiling in app
  // Interrupt for threshold changes


  // p750 setup
 
  



  // SPEC CO setup
  //pinMode(CO_pin, INPUT);
  //attachInterrupt(CO_pin, wakeUpISR, RISING);

  

  // UV setup
  pinMode(UV_pin, INPUT);
  attachInterrupt(UV_pin, wakeUpISR, RISING);




}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // begin sensor reading
  MainHandler();
  


  // apply sleep modes when compatible

  // interrupts for thresholds notifications

  // interrupts for battery charging and depletion

  // interrupt for app connection.... correction: botton interrupt to wake up photon

}

void MainHandler() {
  int currentIndex = 0;
  int sensorValue = 0;
  int isSafe = 1;
  unsigned long startTime = millis();
  bool sleep = true;

  while (millis() - startTime <= sleepTime) {

    // Round Robin Read
    switch(sensorValue) {
      case 0:
        sensorValue = analogRead(UV_pin);
        if (sensorValue >= UVThresh) {
          currentIndex = sensorValue;
          isSafe = 0;
          // TODO: send notification and change LEDs
        }
        break;
      case 1:
        sensorValue = analogRead(CO_pin);
        if (sensorValue >= COThresh) {
          currentIndex = sensorValue;
          isSafe = 0;
          // TODO: send notification and change LEDs
        }

      case 2:
        break; // TODO: implement I2C for each pollutant from p750

    }

    if (!isSafe) {
      sleep = false;
      startTime = millis();
    } 
    
  }

  if (sleep) {
      config.mode(SystemSleepMode::STOP)
      .gpio(UV_pin || CO_pin || p750_read_pin, RISING) // TODO: add button gpio
      .duration(60min);
      System.sleep(config);

    }

}