/*
 * Author: Jeff Jewett
 * Modification Date: 9/22/20
*/

#include "simpletools.h"
#include "EEPROM.h"

const uint8_t EEPROM_I2C_ADDRESS = 0b1010000;  //grants read/write capabilities

const int EEPROM_SCL = 28;
const int EEPROM_SDA = 29;

i2c *eeBus;  // I2C bus ID

void eepromInitI2C() {
  //28 and 29 are i2c pin numbers and 0 is an i2c mode
  eeBus = i2c_newbus(EEPROM_SCL,  EEPROM_SDA,   0); 
}  

//TODO implement EEPROM.h interface