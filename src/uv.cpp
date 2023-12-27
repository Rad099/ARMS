/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/18/23
 * University of Illinois Chicago
 * ECE Department
 * p750 sensor source file
 * Contains all functions for uv readings and converions
 * 
 */

#include "particle.h"


float uvIndex(int& analog) {
    float voltage = (analog * 3.3) / 4095;
    float index = voltage / 0.1;

    return index;
}