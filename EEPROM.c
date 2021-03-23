/*
 * Author: Jeff Jewett
 * Modification Date: 9/22/20
*/

#include "simpletools.h"
#include "EEPROM.h"
#include "Pins.h"

const uint8_t EEPROM_I2C_ADDRESS = 0b1010000;  //grants read/write capabilities

i2c *eeBus;  // I2C bus ID

void eepromInitI2C() {
  eeBus = i2c_newbus(EEPROM_SCL,  EEPROM_SDA,   0); 
}  

void eepromWriteArray(uint16_t address, uint8_t *data, int numBytes) {
  if (eeBus == NULL) {
    eepromInitI2C();
  }    

  while(i2c_busy(eeBus, EEPROM_I2C_ADDRESS));
  i2c_out(eeBus, EEPROM_I2C_ADDRESS, address, sizeof(uint16_t), data, numBytes);
}  

void eepromWriteUint8(uint16_t address, uint8_t data) {
  eepromWriteArray(address, &data, 1);
}

void eepromReadArray(uint16_t address, uint8_t *buffer, int numBytes) {
  if (eeBus == NULL) {
    eepromInitI2C();
  }    

  while(i2c_busy(eeBus, EEPROM_I2C_ADDRESS));
  i2c_in(eeBus, EEPROM_I2C_ADDRESS, address, sizeof(uint16_t), buffer, numBytes);
}  

uint8_t eepromReadUint8(uint16_t address) {
  uint8_t val;
  eepromReadArray(address, &val, 1);
  return val;
}