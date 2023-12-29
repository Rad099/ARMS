/* 
 * Project ARMS
 * Author: Ridwan Alrefai
 * Date: 12/10/23
 * University of Illinois Chicago
 * ECE Department
 * Thresholds header file
 * Provides all functions to set and change threshold values
 * and functions to transfer values from the application
 */

#include "thresholds.h"
#include "Particle.h"

struct userThresholds {
  u_int16_t PM1Thresh;
  u_int16_t PM2_5Thresh;
  u_int16_t PM10Thresh;
  u_int16_t VOCThresh;
  u_int16_t COThresh; 
  u_int16_t UVThresh; 

};

// retrieve values



// set values
void waitForThresholds(bool isSet) {
    
}



// change values during loop
void changeTreshISR(bool& mode, bool& isSet) {
    if (BLE.connected()) {
        // check for new data in buffer

    }
}

int compareStructs(userThresholds* prev, userThresholds* curr) {
    if (prev->COThresh != curr->COThresh || prev->PM10Thresh != curr->PM10Thresh || prev->PM1Thresh != curr->PM1Thresh || prev->PM2_5Thresh != curr->PM2_5Thresh || prev->UVThresh != curr->UVThresh || prev->VOCThresh != curr->VOCThresh) {
        return 1;
    } else {
        return 0;
    }
}
