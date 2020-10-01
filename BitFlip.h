void bitFlip_initI2C();

void readEepromBytes(uint16_t address, uint8_t *data, int numBytes);

void setEepromBytes(uint16_t address, uint8_t *data, int numBytes);

int hasBitFlip(uint16_t address, uint8_t *expectedData, int numBytes);