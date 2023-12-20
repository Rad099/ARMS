/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/18/23
 * University of Illinois Chicago
 * ECE Department
 * p750 sensor source file
 * Contains all functions for readings from the sensor
 * and contains I2P protocol functions
 * 
 */

#include "particle.h"
#include "p750.h"


// setup and begin
void setupI2C() {
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    Wire.begin();
    
}


int toggleTransmission(int mode) {
    if (mode == 1) {
        Wire.beginTransmission(address);
    } else {
        Wire.endTransmission();
    }
}


int requestData(int length);
