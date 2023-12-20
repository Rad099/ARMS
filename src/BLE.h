#include "particle.h"

const char* UUID = "38bdd4ec-fee9-4e99-b045-a3911a7171cd";

// BLE functions

// advertise
void ble_advertisement();

// connect
void onConnected(const BlePeerDevice& peer, void* context);
void onDisconnected(const BlePeerDevice& peer, void* context);

// send data
int ble_sendData(const uint8_t* data, size_t size);

