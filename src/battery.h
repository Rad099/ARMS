/*
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * Battery Source code,
 * This file includes all needed functions for battery usage
 *
*/

#ifndef BATTERY_H
#define BATTERY_H


#include "Particle.h"

extern volatile bool isCharging = false;

float BatteryStatus();

void chargingHandler();





#endif