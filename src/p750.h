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
extern const uint8_t pm1Reg_H = 0x00;
extern const uint8_t pm1Reg_L = 0x01;
extern const uint8_t pm2_5Reg_H = 0x02;
extern const uint8_t pm2_5Reg_L = 0x03;
extern const uint8_t pm10Reg_H = 0x04;
extern const uint8_t pm10Reg_L = 0x05;
extern const uint8_t vocReg_H = 0x20;
extern const uint8_t vocReg_L = 0x21;
extern const uint8_t I2CAddr = 0x12;

void setupI2C();


int toggleTransmission(u_int8_t mode);

void setWireSleep(bool sleep);

uint16_t readData(uint8_t highReg, uint8_t lowReg);

#endif






