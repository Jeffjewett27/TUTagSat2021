#include "simpletools.h"
#include "PacketQueue.h"

void startGridEyeThread(PacketQueue *queue);
void gridEyeThread();

float imageSums(uint16_t *image, float *sum, float *sqrSum);

void compressImage(int16_t *image, uint8_t *compressed, int segment);
//void decompressImage(uint8_t *compressed, int16_t *image);
float getPixelTemp(int16_t val, int isFahr);
void copyCompressedToPacket(Packet *packet, uint8_t *compressed);
void enqueueImage(uint8_t *compressed);

void readImgToBuffer(int16_t *buffer, int segment);
void setFrameRate(uint8_t is1fps);
void gridI2cInit();