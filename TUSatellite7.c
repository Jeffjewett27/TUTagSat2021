/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"
#include "BitFlip.h"
                      // Include simple tools

const uint16_t addr = 32768;
uint8_t data = 0;
uint8_t output;

int main()                                    // Main function
{
  // Add startup code here.
  
  while(1)
  {
    setEepromBytes(addr, &data, 1);
    
    readEepromBytes(addr, &output, 1);
    
    printf("%i\n", output);
    
    break;
    
    // Add main loop code here.
    
  }  
}
