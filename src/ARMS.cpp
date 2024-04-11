/* 
 * Project ARMS
 * Author: Mustafa Munaim and Ridwan Alrefai 
 * Date: 12/10/23
 * University of Illinois Chicago
 * ECE Department
 * Main source code of the ARMS project.
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
//#include "BLE.h"
// NOTE: This file is currently standalone and does not use any other file in this Repo except "co.h"
#include "co.h"

/* GLOBAL DEFINES SECTION */

#define pm1Reg_H  0x00
#define pm1Reg_L  0x01
#define pm2_5Reg_H  0x02
#define pm2_5Reg_L  0x03
#define pm10Reg_H  0x04
#define pm10Reg_L  0x05
#define vocReg_H  0x20
#define vocReg_L  0x21
#define co2Reg_H  0x22
#define co2Reg_L  0x23
#define I2CAddr  0x12
#define stop_pm 0xB6
#define stop_gas 0x28
#define p750_reset 0x85
const uint8_t coPin = A5;
const uint8_t uvPin = A2;

bool isConnected = false;
bool isActive = true;
unsigned long lastReadTime = 0;
bool sensorsReady = false;
bool isAveragingMode = false;

unsigned long previousMillis = millis();
unsigned long interval = 5000; // Interval at which to perform reading (5 min or 5 seconds)
const long setupTime = 30000;
unsigned long previousBatteryUpdateMillis = 0;
const long batteryUpdateInterval = 10000; // 10 seconds
unsigned long lastAveragingStartTime = 0; // Track the start time of the last averaging phase
bool isCollectingForAveraging = false;
bool isInstantMode = true;
bool isOff = false;


uint16_t curr_pm1 = 0;
    
uint16_t curr_pm2_5 = 0;
   
uint16_t curr_pm10 = 0;
 
uint16_t curr_vocIAQI = 0;
 
uint16_t curr_co2 = 0;

uint16_t curr_co = 0;

uint8_t  curr_UVIndex = 0;




// arrays for averages
uint8_t  avg_count = 0;
uint16_t pm1Array[30] = {0}; 
uint16_t pm2_5Array[30] = {0}; 
uint16_t pm10Array[30] = {0}; 
uint16_t vocArray[30] = {0}; 
uint16_t coArray[30] = {0}; 
uint16_t co2Array[30] = {0}; 

/* END GLOBAL DEFINES SECTION */



/* FUNCTION DEFINES SECTION */

void I2CHandler();
void setupI2C();
float BatteryStatus();
void sendBatteryStatusOverBLE();
void readUV();
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
uint16_t readData(uint8_t highReg, uint8_t lowReg);
uint16_t averageData(uint16_t arr[], int size);
void enableSensors();
void controlSensorPower(uint8_t sensorAddress, uint8_t powerState, bool enabling);
void disableSensors();
void sendAQIData(uint16_t pm1, uint16_t pm2_5, uint16_t pm10, uint16_t voc, uint16_t co2, uint16_t co);
uint8_t readRegister(uint8_t reg);
void setupSensor();


/* END FUNCTION DEFINES SECTION */



/*BLE SECTION*/

// services
BleUuid AQIService("38bdd4ec-fee9-4e99-b045-a3911a7171cd");
BleUuid statusService("64f6bc2a-88e6-4fbe-83cd-2c109a1998ca");
BleUuid batteryServiceUUID((uint16_t)0x180F);


// characteristic uuids
BleUuid AQICharUuid("d6eedc89-58c6-4d64-8120-f31737032cd0");
BleUuid batteryCharUuid((uint16_t)0x2A19);
BleUuid uvCharUuid("d421e927-9aad-488d-830e-2302b1698432");
BleUuid appStatusCharUuid("9200dc4d-e3d0-41be-8385-9a1e74540057");

// characteristics
BleCharacteristic AQICharacteristic("AQI", BleCharacteristicProperty::NOTIFY, AQICharUuid, AQIService);
BleCharacteristic batteryLevelCharacteristic("Battery Level", BleCharacteristicProperty::NOTIFY, batteryCharUuid, batteryServiceUUID);
BleCharacteristic uvCharacteristic("UV Value", BleCharacteristicProperty::NOTIFY, uvCharUuid, AQIService);

BleCharacteristic appStatusCharacteristic("AppState", BleCharacteristicProperty::WRITE_WO_RSP, appStatusCharUuid, statusService, onDataReceived, NULL);


BleAdvertisingData advData;

/*END BLE SECTION */

void onDisconnected(const BlePeerDevice& peer, void* context);
void onConnected(const BlePeerDevice& peer, void* context);





// Manual Cloud
SYSTEM_MODE(MANUAL);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);


// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
  
  Serial.begin(9600);

  // SPEC CO setup
  pinMode(coPin, INPUT);

  // UV setup
  pinMode(uvPin, INPUT);

  // p750 setup
  setupI2C();

  

  // BLE section
  BLE.on();
  BLE.addCharacteristic(AQICharacteristic);
  BLE.addCharacteristic(batteryLevelCharacteristic);
  BLE.addCharacteristic(uvCharacteristic);
  BLE.addCharacteristic(appStatusCharacteristic);

  advData.appendServiceUUID(AQIService);
  advData.appendServiceUUID(batteryServiceUUID);
  advData.appendServiceUUID(statusService);
  BLE.onDisconnected(onDisconnected);
  BLE.onConnected(onConnected);
  BLE.advertise(&advData);
 
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
 // Serial.println("Entering loop");
  unsigned long currentMillis = millis();

  if (!sensorsReady && currentMillis - lastReadTime > setupTime && !isOff) { // 30 seconds for warm-up
        Serial.println("setup time complete");
        sensorsReady = true;
        lastReadTime = currentMillis; // Reset last read time after warm-up
    }

    if (isActive && !isOff) {
      isInstantMode = true;
    }

  // Sensor enable/disable logic
    if (isActive && isOff) {
        enableSensors();
        isOff = false;
        isInstantMode = true;
        Serial.println("Device waking up from ble");
    } else if (currentMillis - lastReadTime >= 300000 && isOff) { // 5 minutes for background mode
        enableSensors(); // Enable sensors for averaging
        isOff = false;
        isAveragingMode = true;
        Serial.println("Device waking up after 5 min");
        
    } else if (!isOff && !isAveragingMode && !isActive) {
        disableSensors();
        isOff = true;
        isInstantMode = false;
        Serial.println("Device going to sleep...");
    }


    if (isAveragingMode && sensorsReady) {
        unsigned long averageStart = millis();
        avg_count = 0;
        while (millis() - averageStart < 30000 && avg_count < 30) {
            if (millis() - lastReadTime >= 1000) {
              I2CHandler();
              pm1Array[avg_count] = curr_pm1;
              pm2_5Array[avg_count] = curr_pm2_5;
              pm10Array[avg_count] = curr_pm10;
              vocArray[avg_count] = curr_vocIAQI;
              co2Array[avg_count] = curr_co2;
              coArray[avg_count] = curr_co;
              avg_count++;
              lastReadTime = millis();
            }  
        }

        if (avg_count <= 30) {
          sendAQIData(averageData(pm1Array, avg_count), averageData(pm2_5Array, avg_count),
                            averageData(pm10Array, avg_count), averageData(vocArray, avg_count),
                            averageData(co2Array, avg_count), averageData(coArray, avg_count));
        }

        isAveragingMode = false;
        lastReadTime = millis();
    }

  if (isInstantMode && sensorsReady && isActive && currentMillis - lastReadTime >= 5000) {
    Serial.println("Instant Mode");
    I2CHandler();
    readUV();
     Serial.println("Reading instant values...");
     sendAQIData(curr_pm1, curr_pm2_5, curr_pm10, curr_vocIAQI, curr_co2, curr_co);
     lastReadTime = currentMillis;
  
}

/*
// Collect data if it's the right time
  if (sensorsReady && currentMillis - lastReadTime >= interval) {
        Serial.println("Time to read");
        I2CHandler(); // Always collect data when it's time
        readUV();     // Read UV sensor data

        if (isActive) {
          Serial.println("Reading instant values...");
            // App is in the foreground, send data immediately
            sendAQIData(curr_pm1, curr_pm2_5, curr_pm10, curr_vocIAQI, curr_co2, curr_co);
            lastReadTime = currentMillis;
        } else if (!isActive) {
            Serial.println("Reading averages...");
            // We're in the 30-second data collection period for averaging
            // Store data for averaging
            if (!isCollectingForAveraging) {
              isCollectingForAveraging = true;
              lastAveragingStartTime = currentMillis;
            }

             while (isCollectingForAveraging && avg_count < 30 && currentMillis - lastAveragingStartTime < 30000) {
                currentMillis = millis();
                Serial.println("Collecting data...");
                pm1Array[avg_count] = curr_pm1;
                pm2_5Array[avg_count] = curr_pm2_5;
                pm10Array[avg_count] = curr_pm10;
                vocArray[avg_count] = curr_vocIAQI;
                co2Array[avg_count] = curr_co2;
                coArray[avg_count] = curr_co;
                avg_count++;
                I2CHandler();
             }



             if (avg_count == 30 || currentMillis - lastAveragingStartTime >= 30000) {
                sendAQIData(averageData(pm1Array, avg_count), averageData(pm2_5Array, avg_count),
                            averageData(pm10Array, avg_count), averageData(vocArray, avg_count),
                            averageData(co2Array, avg_count), averageData(coArray, avg_count));
                Serial.println("Averages sent");
                readUV();
                isCollectingForAveraging = false;
                isAveragingMode = false;
                avg_count = 0;
                lastReadTime = currentMillis; // Reset last read time after sending averaged data
            }
        }
    }

*/
// battery logic
if (isConnected && (currentMillis - previousBatteryUpdateMillis >= batteryUpdateInterval && isActive)) {
        previousBatteryUpdateMillis = currentMillis;
        sendBatteryStatusOverBLE();
}

}
void I2CHandler() {
  
    uint16_t pm1 = readData(pm1Reg_H, pm1Reg_L);
    curr_pm1 = pm1;
    
    uint16_t pm2_5 = readData(pm2_5Reg_H, pm2_5Reg_L);
    curr_pm2_5 = pm2_5;

   
    uint16_t pm10  = readData(pm10Reg_H, pm10Reg_L);
    curr_pm10 = pm10;
 
    uint16_t vocIAQI = readData(vocReg_H, vocReg_L);
    curr_vocIAQI = vocIAQI;
  
 
    uint16_t co2Value = readData(co2Reg_H, co2Reg_L);
    curr_co2 = co2Value;

    int co_analog = analogRead(coPin);

    uint16_t coValue = readCO(co_analog);
    curr_co = coValue;

    Serial.printlnf("PM1: %d Pm2.5: %d pm10: %d voc: %d, co2: %d, co: %d", pm1, pm2_5, pm10, vocIAQI, co2Value, coValue);

}

void setupI2C() {
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Wire.begin();
}

u_int16_t readData(uint8_t highReg, uint8_t lowReg) {
    byte error;
    uint8_t value[2];
    Wire.beginTransmission(0x12);
    Wire.write(highReg);
    error = Wire.endTransmission();
    delay(200);
    if (error == 0) {
      Wire.requestFrom(0x12, (uint8_t) 2);
      int index = 0;
      while (Wire.available() && index < 2) {
          value[index++] = Wire.read();
      }
      
      return ((value[0] << 8) | value[1]);

    } else {
      Serial.printlnf("Read didnt work with error %d", error);
      return -1;
    }
}

// p750 control
void controlSensorPower(uint8_t sensorAddress, uint8_t powerState, bool enabling) {
    Wire.beginTransmission(I2CAddr);
    Wire.write(sensorAddress);
    Wire.write(powerState); 
    if (enabling) {
    Wire.endTransmission();
  } else {
    Wire.endTransmission(false);
  }
}

float BatteryStatus() {
    float voltage = analogRead(A6) / 819.2;
    float currentCharge = (voltage-3.0) / (1.2);
    Serial.println("Current charge = "+String(currentCharge));
    
    return currentCharge;
}

void sendBatteryStatusOverBLE() {
    float batteryStatus = BatteryStatus(); 
    uint8_t batteryLevel = static_cast<uint8_t>(batteryStatus * 100);

    // Update the BLE characteristic with the battery level
    batteryLevelCharacteristic.setValue(&batteryLevel, 1);

   // Serial.printlnf("Battery level (%d%%) sent over BLE.", batteryLevel);
}

void onDisconnected(const BlePeerDevice& peer, void* context) {
    Serial.println("Disconnected from device");
    isConnected = false;
    RGB.control(false);
    BLE.advertise(&advData);
}

void readUV() {
  int uvValue = analogRead(uvPin);
  float voltage = uvValue*(3.3/4095);
  float index = voltage / 0.1;

  uint8_t uv = static_cast<uint8_t>(round(index));
  uvCharacteristic.setValue(&uv, 1);
  Serial.println(uv);
}

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    if (len > 0) {
        uint8_t receivedStatus = data[0];
        if (receivedStatus == 0) {
        interval = 300000; // 5 minutes in milliseconds
        isActive = false;
    } else if (receivedStatus == 1) {
        isActive = true;
        isInstantMode = true;
        interval = 5000; // 5 seconds in milliseconds
    }
    
    Serial.printlnf("Received app status: %d, interval set to %ld ms", receivedStatus, interval);

    } else {
      Serial.println("Error with app state  message.");
    }
}

void onConnected(const BlePeerDevice& peer, void* context) {
  Serial.println("Device Connected to app");
  RGB.control(true);
  isConnected = true;
  RGB.color(0, 0, 255);
  BLE.stopAdvertising();
}


void enableSensors() {
    controlSensorPower(stop_pm, 0x00, true); 
    controlSensorPower(stop_gas, 0x01, true);
    sensorsReady = false;
    lastReadTime = millis();
    RGB.control(true);
    RGB.color(0, 255, 0);
    //Wire.reset();
    //controlSensorPower(p750_reset, 0x01);
}

void disableSensors() {
    controlSensorPower(stop_pm, 0x01, false); 
    controlSensorPower(stop_gas, 0x00, false);
    RGB.control(true);
    RGB.color(255,255,224);
}

void sendAQIData(uint16_t pm1, uint16_t pm2_5, uint16_t pm10, uint16_t voc, uint16_t co2, uint16_t co) {

    if (pm1 < 0 || pm1 > 500) {
      pm1 = 0;
    } if (pm2_5 < 0 || pm2_5 > 500) {
      pm2_5 = 0;
    }  if (pm10 < 0 || pm10 > 500) {
      pm10 = 0;
    } if (voc < 0 || voc > 500) {
      voc = 0;
    }  if (co2 < 0) {
      co2 = 0;
    } if (co < 0 || co > 500) {
      co = 0;
    }
    
    uint16_t values[] = {pm1, pm2_5, pm10, voc, co2, co};
    const size_t numValues = sizeof(values) / sizeof(values[0]);
    uint8_t dataBuffer[numValues * 2]; // Each uint16_t will be split into 2 uint8_t

    for (size_t i = 0; i < numValues; ++i) {
        // Split each uint16_t into two bytes and store in the buffer
        dataBuffer[2 * i] = values[i] >> 8; // High byte
        dataBuffer[2 * i + 1] = values[i] & 0xFF; // Low byte
    }

    AQICharacteristic.setValue(dataBuffer, sizeof(dataBuffer));
    Serial.println("Data sent over BLE");
    Serial.printlnf("PM1: %d Pm2.5: %d pm10: %d voc: %d, co2: %d, co: %d", pm1, pm2_5, pm10, voc, co2, co);
}



uint16_t averageData(uint16_t arr[], int size) {
  int i, sum = 0;
  uint16_t avg;

  for (i = 0; i < size; ++i) {
    sum += arr[i];

  }

  avg = static_cast<uint16_t>(round((double(sum)/size)));

  return avg;
}


