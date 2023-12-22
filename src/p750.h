/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/18/23
 * University of Illinois Chicago
 * ECE Department
 * p750 sensor header file
 * Contains all functions for readings from the sensor
 * and contains I2C protocol functions
 * 
 */


#include "particle.h"

int address = 0x12; // p750 I2C address given by datasheet



// setup and begin
void setupI2C();


int toggleTransmission(int mode);


int requestData(int length);







