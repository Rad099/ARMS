/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/18/23
 * University of Illinois Chicago
 * ECE Department
 * p750 sensor source file
 * Contains all functions for readings from the sensor
 * and contains I2C protocol functions
 * 
 */

#include "particle.h"
#include "p750.h"


// setup and begin
void setupI2C() {
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Wire.begin();
    
    
}


void setWireSleep(bool sleep) {
    if (sleep) {
        
    }
}

uint16_t readData(uint8_t highReg) {
    byte error;
    uint16_t value[2];
    Wire.beginTransmission(I2CAddr);
    Wire.write(highReg);
    error = Wire.endTransmission();
    delay(2000);
    if (error == 0) {
        Wire.requestFrom(I2CAddr, (uint8_t) 2);
        byte index = 0;
        while (Wire.available() && index < 2) {
            value[index++] = Wire.read();
        }

        return ((value[0] << 8) | value[1]);
 
    } else {
        Serial.printlnf("Error %d", error);
        return -1;
    }
}
