#ifndef INDEX_H
#define INDEX_H


#include "Particle.h"


// Breakpoint constants based on Journal: TODO: confirm implementation
struct Breakpoint {
    float bpLow;
    float bpHigh;
    uint16_t indexLow;
    uint16_t indexHigh;
};

extern const Breakpoint pm2_5Breakpoints[7];
extern const Breakpoint pm10_Breakpoints[7];
extern const Breakpoint voc_Breakpoints[7];
extern const Breakpoint co_Breakpoints[7];

float AQI(uint8_t C, uint8_t BPL, uint8_t BPH, uint8_t IL, uint8_t IH);

float findBreakpoint(uint8_t c, const Breakpoint breakpoints[]);






#endif