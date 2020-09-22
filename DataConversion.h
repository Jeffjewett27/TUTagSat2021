/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#ifndef DATA_CONVERSION
#define DATA_CONVERSION

uint16_t reduceFloat16bit(float f, int isSigned, uint8_t k);
float extendFloatFrom16bit(uint16_t b, int isSigned, int k);

#endif