#include "simpletools.h"
#include "BitFlip.h"
#include "EEPROM.h"
#include "PacketQueue.h"
#include "Packet.h"

#define BITFLIP_BUFFER_SIZE 32

const int BITFLIP_FN_CODE = 0x33;

const uint16_t BITFLIP_MIN_ADDR = 32768;
const uint16_t BITFLIP_MAX_ADDR = 65535;

int bitFlipIter = 0;

void prepareBitFlipArray(uint8_t *zeroes) {
  for (int i = 0; i < BITFLIP_BUFFER_SIZE; i++) {
    zeroes[i] = 0;
  }
  
  uint16_t addr = BITFLIP_MIN_ADDR;
  while (addr < BITFLIP_MAX_ADDR && addr >= BITFLIP_MIN_ADDR) {
    eepromWriteArray(addr, zeroes, BITFLIP_BUFFER_SIZE);
    addr += BITFLIP_BUFFER_SIZE;
  }
}

void checkBitFlips(uint8_t *zeroes, PacketQueue *queue) {
    int count = 0;
    Packet packet;
    makePacketDataZero(&packet);
    uint16_t addr = BITFLIP_MIN_ADDR;
    while (addr < BITFLIP_MAX_ADDR && addr >= BITFLIP_MIN_ADDR) {
      eepromReadArray(addr, zeroes, BITFLIP_BUFFER_SIZE);
      for (int i = 0; i < BITFLIP_BUFFER_SIZE; i++) {
        if (zeroes[i]) {
          eepromWriteUint8(addr + i, 0);
          packet.ArrayType.twoByte[count*2] = addr + i;
          packet.ArrayType.twoByte[count*2+1] = zeroes[i];
          count++;
          if (count >= 32) {
            //if there are 32 bit flips in 10 minutes, we have a problem
           break; 
          }
        }        
      }
      addr += BITFLIP_BUFFER_SIZE;
    }
    
    if (count > 0) {
      setPacketHeader(&packet, BITFLIP_FN_CODE, bitFlipIter, count);
      enqueue(queue, packet);
    }
    bitFlipIter++;
}