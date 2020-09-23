#include "simpletools.h"

i2c *rgbBus;  //I2C bus.
const uint8_t rgbControl = 0x29;  //Grants read/write capabilities.

const int RGB_SCL = 0;
const int RGB_SDA = 1;

void rgb_initI2C() {
  rgbBus = i2c_newbus(RGB_SCL, RGB_SDA, 0); //Two pin numbers, 0 = i2c mode.
}

void rgb_write(uint16_t address, uint8_t val) {
  if (rgbBus == NULL) {
    rgb_initI2C();
  }

  while(i2c_busy(rgbBus, rgbControl));
  print("Got to here");
  i2c_out(rgbBus, rgbControl, address, 2, &val, 1); //Write val to address.
}

uint8_t rgb_read(uint16_t address) {
  if (rgbBus == NULL) {
    rgb_initI2C();
  }

  while(i2c_busy(rgbBus, rgbControl));
  print("Got to here");
  uint8_t val;
  i2c_in(rgbBus, rgbControl, address, 2, &val, 1); //Read val from address.
  return val;
}