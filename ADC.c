#include "simpletools.h"
#include "ADC.h"
#include "Pins.h"

int __Mvref = 330;
int __Mbits = 6;
int __Mdata = 0b110000;
int __Mres = 12;

//hundreths of Volts
uint16_t readAnalog(int channel) {
  int mdata = __Mdata | ((channel & 0x7) << 1);
  high(__McsPin);
  low(__MclkPin);
  low(__McsPin);
  shift_out(__MdiPin, __MclkPin, MSBFIRST, __Mbits, mdata);
  int __Mvolts = shift_in(__MdoPin, __MclkPin, MSBPOST, __Mres);
  high(__McsPin);
  high(__MclkPin);
  return ((__Mvolts * __Mvref) / pow(2, __Mres));
} 