/*
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * BLE Source code,
 * This file includes all needed functions and constants for BLE usage
 *
*/


#include "particle.h"

const char* UUID = "38bdd4ec-fee9-4e99-b045-a3911a7171cd";

// service
BleUuid AQIService("d06d6cd1-f482-4a87-9199-328384bb7d87");

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
BleCharacteristic uvCharacteristic("voc", BleCharacteristicProperty::NOTIFY, uvCharUuid, AQIService);



// BLE functions

// advertise
int ble_advertisement();

// connect
void onConnected(const BlePeerDevice& peer, void* context);
void onDisconnected(const BlePeerDevice& peer, void* context);

// send data
int ble_sendData(const uint8_t* data, size_t size);

