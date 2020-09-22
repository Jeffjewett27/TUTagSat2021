#include "simpletools.h"
#include "BitFlip.h"

i2c *bfBus;

const int BITFLIP_SCL = 28;
const int BITFLIP_SDA = 29;

const uint8_t BITFLIP_CONTROL = 0b1010000;  //grants read/write capabilities

void bitFlip_initI2C() {
  bfBus = i2c_newbus(BITFLIP_SCL,  BITFLIP_SDA,   0); //28 and 29 are i2c pin numbers and 0 is an i2c mode
}  

void readEepromBytes(uint16_t address, uint8_t *data, int numBytes) {                                   
  if (bfBus == NULL) {
    bitFlip_initI2C();
  } 
  
  while(i2c_busy(bfBus, BITFLIP_CONTROL));
  
  i2c_in(bfBus, BITFLIP_CONTROL, address, sizeof(uint16_t), data, numBytes); //read the value from EEPROM into pc
}

void setEepromBytes(uint16_t address, uint8_t *data, int numBytes) {
  if (bfBus == NULL) {
    bitFlip_initI2C();
  }    

  while(i2c_busy(bfBus, BITFLIP_CONTROL));
  i2c_out(bfBus, BITFLIP_CONTROL, address, sizeof(uint16_t), data, numBytes); //output the value of pc to EEPROM
}