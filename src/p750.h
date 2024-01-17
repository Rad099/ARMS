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
#ifndef P750_H
#define P750_H

#include "particle.h"

// I2C Command registers
extern const uint8_t pm1Reg = 0x00;
extern const uint8_t pm2_5Reg = 0x02;
extern const uint8_t pm10Reg = 0x04;
extern const uint8_t vocReg = 0x20;
extern const uint8_t I2CAddr = 0x12;

void setupI2C();


int toggleTransmission(u_int8_t mode);

void setWireSleep(bool sleep);



int readData(uint8_t type);

#endif






