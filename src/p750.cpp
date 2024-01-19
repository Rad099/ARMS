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
    Wire.begin();
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    
}


int toggleTransmission(int mode) {
    if (mode == 1) {
        Wire.beginTransmission(0x12);
    } else {
        Wire.endTransmission();
    }
}

void setWireSleep(bool sleep) {
    if (sleep) {
        
    }
}

uint16_t readData(uint8_t highReg, uint8_t lowReg) {
    Wire.beginTransmission(I2CAddr);
    Wire.write(highReg);
    Wire.endTransmission(false);

    Wire.requestFrom((int)I2CAddr, (uint8_t) 2);
    if (Wire.available() >= 2) {
        uint16_t highByte = Wire.read();
        uint16_t lowByte = Wire.read();
        return ((highByte << 8) | lowByte);
    } else {
        return 0; // error occurred
    }
}
