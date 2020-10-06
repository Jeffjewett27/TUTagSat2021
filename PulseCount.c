#include "simpletools.h"
#include "PulseCount.h"

const int LTF_PIN = 11;  //Light to frequency
const int RAD_PIN = 16;  //Radiation (particle detector)

const uint8_t LTF_FN = 0x01;
const uint8_t RAD_FN = 0x02;

volatile uint16_t ltfCount = 0;  //Count of pulses
volatile int ltfOverflow = 0;    //Count of overflow occurrences
volatile int ltfReset = 0;       //Flag to mark a pulse count as needing reset
uint16_t ltfPulseCounts[16];     //Array of 1 minute's worth of pulse counts

volatile uint8_t radCount = 0; 
volatile int radOverflow = 0; 
volatile int radReset = 0; 
uint8_t radPulseCounts[32];

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

uint16_t lightToFrequency_read_reset() {
  uint16_t val = ltfCount;
  if (ltfOverflow) {
    //If there was an overflow, return max value
    val = ~0; //Max value for unsigned int
  }
  ltfReset = 1; //Let the other thread take care of resetting to avoid race condition
  return val;
}  

uint8_t radiation_read_reset() {
  uint8_t val = radCount;
  if (radOverflow) {
    val = ~0;
  }
  radReset = 1;
  return val;
}  

void lightToFrequency_read(int i) {
  uint16_t val = lightToFrequency_read_reset();
  ltfPulseCounts[i] = val;
  printf("LTF -- val: %d, i: %d\n", val, i);
}  

void radiation_read(int i) {
  uint8_t val = radiation_read_reset();
  radPulseCounts[i] = val;
  printf("RAD -- val: %d, i: %d\n", val, i);
} 

Packet getLightToFrequencyPacket(uint8_t iter, uint8_t pc) {
  Packet p;
  setPacketHeader(&p, LTF_FN, iter, pc);
  for (int i=0; i<16; i++) {
    printf("%d, ", ltfPulseCounts[i]);
    p.ArrayType.twoByte[i] = ltfPulseCounts[i];
  } 
  print("\n\n");   
  return p;
} 

Packet getRadiationPacket(uint8_t iter, uint8_t pc) {
  Packet p;
  setPacketHeader(&p, RAD_FN, iter, pc);
  for (int i=0; i<32; i++) {
    printf("%d, ", radPulseCounts[i]);
    p.ArrayType.oneByte[i] = radPulseCounts[i];
  }    
  print("\n\n"); 
  return p;
} 
