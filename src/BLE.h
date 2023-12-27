/*
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * BLE Source code,
 * This file includes all needed functions and constants for BLE usage
 *
*/


#include "particle.h"




// BLE functions

// advertise
int ble_advertisement();

// connect
void onConnected(const BlePeerDevice& peer, void* context);
void onDisconnected(const BlePeerDevice& peer, void* context);

// send data
int ble_sendData(const uint8_t* data, size_t size);

