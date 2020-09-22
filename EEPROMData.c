#include "EEPROM.h"
#include "EEPROMData.h"

//Addresses start at MIN_EEPROM_ADDR to avoid overwriting program memory
const uint16_t MIN_EEPROM_ADDR = 32768;
//Reserve EEPROM_DATA_SIZE bytes to store satellite information
const uint16_t EEPROM_DATA_SIZE = 32;
//Offset some number of bytes from MIN_EEPROM_ADDR (but less than EEPROM_DATA_SIZE)
const uint16_t PACKET_COUNT_OFFSET = 0;
const uint16_t ON_CHECK_OFFSET = 1;

const uint8_t ON_CHECK_VALUE = 183;  //arbitrary nonzero constant value

//stores pc in the eeprom
void setPacketCount(uint8_t packetCount) {
  uint16_t address = MIN_EEPROM_ADDR + PACKET_COUNT_OFFSET;
  eepromWriteUint8(address, packetCount);
}  

void incrementPacketCount(uint8_t inc) {
  uint16_t address = MIN_EEPROM_ADDR + PACKET_COUNT_OFFSET;
  uint8_t packetCount = getPacketCount();
  setPacketCount(packetCount + inc);
}  

//reads pc from eeprom
uint8_t getPacketCount() {
  uint16_t address = MIN_EEPROM_ADDR + PACKET_COUNT_OFFSET;
  uint8_t packetCount = eepromReadUint8(address);
}   

//sets a unique value in eeprom
void setOnCheck() {
  uint16_t address = MIN_EEPROM_ADDR + ON_CHECK_OFFSET;
  eepromWriteUint8(address, ON_CHECK_VALUE);
}  

//resets unique value to 0 in eeprom (use before sending into orbit)
void resetOnCheck() {
  uint16_t address = MIN_EEPROM_ADDR + ON_CHECK_OFFSET;
  eepromWriteUint8(address, 0);
}  

//checks if unique value has been set
uint8_t isOnCheckSet() {
  uint16_t address = MIN_EEPROM_ADDR + ON_CHECK_OFFSET;
  uint8_t onCheck = eepromReadUint8(address);
  return onCheck == ON_CHECK_VALUE;
}  
