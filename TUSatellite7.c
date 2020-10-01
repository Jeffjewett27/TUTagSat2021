/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"
#include "BitFlip.h"
                      // Include simple tools

const uint16_t startingAddr = 32768;

int main()                                    // Main function
{
  uint16_t addr = startingAddr;
  for (int i = 0; i<100; i++) {
    // Add startup code here.
    uint8_t data = 0;
    setEepromBytes(addr, &data, 1);
    
    if (rand() % 10 == 4) {
    uint8_t warp = rand() % 256;
    setEepromBytes(addr, &warp, 1); // UV ray hits the satellite
    }
        
    uint8_t flippedData = hasBitFlip(addr, &data, 1);
    print("Bit Flip: ");
    printf("%i\n",!!flippedData);
    
    if (flippedData){
      print("Address: ");
      printf("%i\n", addr);
      print("Flipped Data: ");
      printf("%i\n",flippedData);
      setEepromBytes(addr, &data, 1); // change back to original data
    }
    
    addr += 1;
  }  
 
}
