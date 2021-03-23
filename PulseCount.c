#include "simpletools.h"
#include "PulseCount.h"
#include "Pins.h"

#define UINT16T_MAX 0xffff
#define UINT8T_MAX 0xff
#define LTF_FN_CODE 0x16
#define RAD_FN_CODE 0x17

volatile uint16_t ltfCount = 0;  //Count of pulses
volatile int ltfOverflow = 0;
volatile int ltfReset = 0;
uint16_t ltfPulseCounts[16];     //Array of 1 minute's worth of pulse counts
int *ltfCog;

volatile uint8_t radCount = 0; 
volatile int radOverflow = 0; 
volatile int radReset = 0; 
uint8_t radPulseCounts[32];
int *radCog;


void startLtfThread() {
  ltfCog = cog_run(ltfThread, 64);
}

void stopLtfThread() {
  cog_end(ltfCog);
}  

void ltfThread() {
  while(1) {
    if (ltfReset) {
      initLtf();
    }
    int temp = ltfCount;
    temp += count(LTF_PIN, 10);
    if (temp > UINT16T_MAX) {
      ltfOverflow = 1;
    }
    ltfCount = temp;
  }    
}  
  
void initLtf() {
  ltfCount = 0;
  ltfOverflow = 0;
  ltfReset = 0;
}

uint16_t ltfReadReset() {
  uint16_t val = ltfOverflow ? UINT16T_MAX : ltfCount;
  ltfCount = 0;
  ltfReset = 1;
  return val;
}

void lightToFrequencyRead(int i) {
  uint16_t val = ltfReadReset();
  ltfPulseCounts[i] = val;
}  

void startRadThread() {
  radCog = cog_run(radThread, 64);
}

void stopRadThread() {
  cog_end(radCog);
}  

void radThread() {
  while(1) {
    if (radReset) {
      initRad();
    }
    int temp = radCount;
    temp += count(RAD_PIN, 10);
    if (temp > UINT8T_MAX) {
      radOverflow = 1;
    }
    radCount = temp;
  }    
}  
  
void initRad() {
  radCount = 0;
  radOverflow = 0;
  radReset = 0;
}

uint8_t radReadReset() {
  uint16_t val = radOverflow ? UINT8T_MAX : radCount;
  radCount = 0;
  radReset = 1;
  return val;
}

void radiationRead(int i) {
  uint8_t val = radReadReset();
  radPulseCounts[i] = val;
}

Packet getLightToFrequencyPacket(uint8_t iter, uint8_t pc) {
  Packet p;
  setPacketHeader(&p, LTF_FN_CODE, iter, pc);
  for (int i = 0; i < PACKET_NUM_2_BYTE; i++) {
    p.ArrayType.twoByte[i] = ltfPulseCounts[i];
  } 
  return p;
} 

Packet getRadiationPacket(uint8_t iter, uint8_t pc) {
  Packet p;
  setPacketHeader(&p, RAD_FN_CODE, iter, pc);
  for (int i = 0; i < PACKET_NUM_1_BYTE; i++) {
    p.ArrayType.oneByte[i] = radPulseCounts[i];
  }
  return p;
} 
