#include "simpletools.h"

i2c *rgbBus;  //I2C bus.
const uint8_t rgbControl = 0x29;  //Grants read/write capabilities.
const uint8_t RGB_ENABLE = 0x00;
const uint8_t RGB_ENABLE_PON = 0x01;
const uint8_t RGB_ENABLE_AEN = 0x02;

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
  i2c_out(rgbBus, rgbControl, address, 2, &val, 1); //Write val to address.
}

uint8_t rgb_read(uint16_t address) {
  if (rgbBus == NULL) {
    rgb_initI2C();
  }

  while(i2c_busy(rgbBus, rgbControl));
  uint8_t val;
  i2c_in(rgbBus, rgbControl, address, 2, &val, 1); //Read val from address.
  return val;
}

//Attempting to replicate functionality seen at https://github.com/adafruit/Adafruit_TCS34725/blob/master/Adafruit_TCS34725.cpp.
void rgb_enable() {
  rgb_write(RGB_ENABLE, RGB_ENABLE_PON);
  rgb_write(RGB_ENABLE, RGB_ENABLE_PON | RGB_ENABLE_AEN);
}  
