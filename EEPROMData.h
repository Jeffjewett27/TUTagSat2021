//stores pc in the eeprom
void setPacketCount(uint8_t pc);

void incrementPacketCount(uint8_t inc);

//reads pc from eeprom
uint8_t getPacketCount();  

//sets a unique value in eeprom
void setOnCheck();

//resets unique value to 0 in eeprom (use before sending into space)
void resetOnCheck();

//checks if unique value has been set
uint8_t isOnCheckSet();
