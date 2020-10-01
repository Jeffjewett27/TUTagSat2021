#include "simpletools.h"
#include "PulseCount.h"

const int LTF_PIN = 11;  //Light to frequency
const int RAD_PIN = 16;  //Radiation (particle detector)

volatile uint32_t ltfCount = 0;  //Count of pulses
volatile int ltfOverflow = 0;    //Count of overflow occurrences
volatile int ltfReset = 0;       //Flag to mark a pulse count as needing reset

volatile uint32_t radCount = 0;  
volatile int radOverflow = 0; 
volatile int radReset = 0; 

int *pcCog;

void startPulseCountThread() {
  pcCog = cog_run(pulseCountThread, 128);
}

void stopPulseCountThread() {
  cog_end(pcCog);
}  
  
void initPulseCount_ltf() {
  ltfCount = 0;
  ltfOverflow = 0;
  ltfReset = 0;
}

void initPulseCount_rad() {
  radCount = 0;
  radOverflow = 0;
  radReset = 0;
}

void pulseCountThread() {
  int ltfFreeze = 0;
  int radFreeze = 0;
  initPulseCount_ltf();
  initPulseCount_rad();
  
  while (1) {
    //Light to frequency
    if (ltfReset) {
      initPulseCount_ltf();
    }    
    if (ltfFreeze) {
      ltfFreeze = 0;
    } 
    else if (input(LTF_PIN)) {
      ltfCount++;
      if (ltfCount == 0) {  //Check if an overflow has happened
        ltfOverflow++; 
      }        
      ltfFreeze = 1;
    }  
    
    //Radiation
    if (radReset) {
      initPulseCount_rad();
    }    
    if (radFreeze) {
      radFreeze = 0;
    } 
    else if (input(RAD_PIN)) {
      radCount++;
      if (radCount == 0) {  //Check if an overflow has happened
        radOverflow++; 
      }        
      radFreeze = 1;
    }        
  }    
}  

uint32_t lightToFrequency_read_reset() {
  uint32_t val = ltfCount;
  if (ltfOverflow) {
    //If there was an overflow, return max value instead of some lower amount
    val = ~0; //Max value for unsigned int
  }
  ltfReset = 1; //Let the other thread take care of resetting to avoid race condition
  return val;
}  

uint32_t radiation_read_reset() {
  uint32_t val = radCount;
  if (radOverflow) {
    //If there was an overflow, return max value instead of some lower amount
    val = ~0; //Max value for unsigned int
  }
  radReset = 1; //Let the other thread take care of resetting to avoid race condition
  return val;
}  