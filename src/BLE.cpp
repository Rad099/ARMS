/*
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * BLE Source code,
 * This file includes all needed functions for BLE usage
 *
*/

#include "BLE.h"

// BLE functions

// advertise
int ble_advertisement() {
    BLE.advertise();
    BLE.setAdvertisingInterval(100);
    
}

// connect
void onConnected(const BlePeerDevice& peer, void* context) {

    BleAddress central_address = peer.address();
    char addrstr[18];
    snprintf(addrstr, sizeof(addrstr), "%02X:%02X:%02X:%02X:%02X:%02X", central_address[0], central_address[1], 
    central_address[2], central_address[3], central_address[4], central_address[5]);


    Serial.printlnf("connected to device: %s", addrstr);
}

void onDisconnected(const BlePeerDevice& peer, void* context) {
    Serial.println("Disconnected from device");
}

// send data
int ble_sendData(const uint8_t* data, size_t size) {
    BleCharacteristic c;
    c.setValue(data, size);
}


