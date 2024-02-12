/*
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * Battery Source code,
 * This file includes all needed functions for battery usage
 *
*/

#include "battery.h"

float BatteryStatus() {
    float voltage = analogRead(A6) / 819.2;
    Serial.println("Voltage = "+String(voltage));

    float currentCharge = (voltage-3.0) / (1.2);
    Serial.println("Current charge = "+String(currentCharge));
    
    return currentCharge;
}

void chargingHandler() {
    isCharging = digitalRead(CHG);
}