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

#ifndef THRESHOLDS_H
#define THRESHOLDS_H


struct userThresholds;


// set values
void waitForThresholds(bool isSet);



// change values during loop
void changeTreshISR(bool& mode, bool& isSet);

int compareStructs(userThresholds* prev, userThresholds* curr);


#endif



