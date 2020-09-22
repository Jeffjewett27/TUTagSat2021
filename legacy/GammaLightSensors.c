/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "GammaLightSensors.h"

const int GAMMA_PIN = 0;
const int LIGHT_PIN = 2;

volatile uint32_t gCount = 0;
volatile uint32_t lCount = 0;
volatile int gOverflow = 0;
volatile int lOverflow = 0;
volatile int gReset = 0;
volatile int lReset = 0;

int *lgCog;

void startLightGammaThread() {
  lgCog = cog_run(gamma_light_thread, 128);
}

void stopLightGammaThread() {
  cog_end(lgCog);
}  
  
void gammaInitialize() {
  gCount = 0;
  gOverflow = 0;
  gReset = 0;
}

void lightInitialize() {
  lCount = 0;
  lOverflow = 0;
  lReset = 0;
}

//run the gamma light sensor, started by 'startLightGammaThread'
void gammaLightThread() {
  int gFreeze = 0;
  int lFreeze = 0;
  gammaInitialize();
  lightInitialize();
  while (1) {
    if (gReset) {
      gammaInitialize();
    }      
    if (gFreeze) {
      gFreeze = 0;
    } else if (input(GAMMA_PIN)) {
      
      gCount++;
      if (gCount == 0) {  //check if an overflow has happened
        gOverflow++; 
      }        
      gFreeze = 1;
    }            
    
    if (lReset) {
      lightInitialize();
    }      
    if (lFreeze) {
      lFreeze = 0;
    } else if (input(LIGHT_PIN)) {
      lCount++;
      if (lCount == 0) {  //check if an overflow has happened
        lOverflow++; 
      }  
      lFreeze = 1;
    }
  }    
}  

uint32_t gammaReadReset() {
  uint32_t val = gCount;
  if (gOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  gReset = 1; //let the other thread take care of resetting to avoid race condition
  return val;
}  

uint32_t lightToFrequencyReadReset() {
  uint32_t val = lCount;
  if (lOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  lReset = 1; //let the other thread take care of resetting to avoid race condition
  return val;
}  