#include "GridEyeSensor.h"
#include "BitFlip.h"

#define GRID_BUFFER_SIZE 16
#define CMP_IMG_SIZE 96
#define GRID_SCL 17
#define GRID_SDA 18
#define GRID_EN 19
#define GRID_FN_CODE_A 0x30
#define GRID_FN_CODE_B 0x31
#define GRID_FN_CODE_C 0x32
#define NUM_GRID_SAMPLES 36
#define GRID_SAMPLE_INT 5000

i2c *gridBus;
const uint8_t gridControl = 0b1101001;
const uint8_t GRID_PIXEL_OFFSET = 0x80;

int *thread;
PacketQueue *priorityQueue;
uint8_t imgId = 0;

void gridI2cInit() {
  gridBus = i2c_newbus(GRID_SCL, GRID_SDA, 0);
  high(GRID_EN);
  pause(10);
  setFrameRate(1);
}  

void setFrameRate(uint8_t is1fps) {  
  while(i2c_busy(gridBus, gridControl));
  
  uint8_t address = 0x02;
  i2c_out(gridBus, gridControl, address, sizeof(uint8_t), &is1fps, sizeof(uint8_t));
}

void startGridEyeThread(PacketQueue *queue) {
  priorityQueue = queue;
  gridI2cInit();
  thread = cog_run(gridEyeThread, 192);
}  

void gridEyeThread() {
  int16_t buffer[GRID_BUFFER_SIZE];
  uint8_t bestImage[CMP_IMG_SIZE];
  prepareBitFlipArray(bestImage); //to reduce num threads, call bit flip periodically
  pause(1000); //give time to take image
  while(1) {
    float variance = 0; //set a minimum threshold
    int shouldSend = 0;
    for (int i = 0; i < NUM_GRID_SAMPLES; i++) {
      float sum = 0.0;
      float sqrSum = 0.0;
      for (int j = 0; j < 4; j++) {
          readImgToBuffer(buffer, j);
          imageSums(buffer, &sum, &sqrSum);
      }        
      float imgVar = ((64 * sqrSum) - sum * sum) / 4096; //shortcut variance formula: ((n*sqrSum)-sum^2)/n^2, for n=64 pixels
      if (imgVar > variance) {
        shouldSend = 1;
        variance = imgVar;
        for (int j = 0; j < 4; j++) {
          readImgToBuffer(buffer, j);
          compressImage(buffer, bestImage, j);
          shouldSend = 1;
        }        
      }
      pause(GRID_SAMPLE_INT);
    }
    if (shouldSend) {
      enqueueImage(bestImage);
    }
    if (imgId % 8 == 1) {
      checkBitFlips(bestImage, priorityQueue);
    }
  }    
}  

void readImgToBuffer(int16_t *buffer, int segment) {
  if (gridBus == NULL) {
    gridI2cInit();
  } 
  
  while(i2c_busy(gridBus, gridControl));
  
  i2c_in(gridBus, gridControl, GRID_PIXEL_OFFSET + GRID_BUFFER_SIZE * 2 * segment, sizeof(uint8_t), (uint8_t *)buffer, GRID_BUFFER_SIZE * 2);
  return;
}  

//from https://github.com/sparkfun/SparkFun_GridEYE_Arduino_Library/blob/master/src/SparkFun_GridEYE_Arduino_Library.cpp
float getPixelTemp(int16_t val, int isFahr) {
  if(val & (1 << 11))
  {
    // if temperature is negative, mask out the sign byte and 
    // make the float negative
    val &= ~(1 << 11);
    val *= -1;
  }

  float degrees = val * 0.25;
  if (isFahr) {
   degrees = degrees * 1.8 + 32; 
  }    

  return degrees;
}

float imageSums(uint16_t *image, float *sum, float *sqrSum) {
  for (int i = 0; i < GRID_BUFFER_SIZE; i++) {
    float val = getPixelTemp(image[i], 0);
    *sum += val;
    *sqrSum += val * val;
  }    
}

void compressImage(int16_t *image, uint8_t *compressed, int segment) {
  int imgIdx = 0;
  int compIdx = 24 * segment;
  for (int i = 0; i < 8; i++) {
    uint16_t first = image[imgIdx];
    uint16_t second = image[imgIdx + 1];
    uint8_t compOne = first >> 4;
    uint8_t compTwo = ((first << 4) & 0xf0) | ((second >> 8) & 0x0f);
    uint8_t compThree = second & 0xff;
    compressed[compIdx] = compOne;
    compressed[compIdx+1] = compTwo;
    compressed[compIdx+2] = compThree;
    imgIdx += 2;
    compIdx += 3;
  }
}

//for data analysis only
/*void decompressImage(uint8_t *compressed, int16_t *image) {
  int imgIdx = 0;
  int compIdx = 0;
  for (int i = 0; i < 32; i++) {
    uint8_t compOne = compressed[compIdx];
    uint8_t compTwo = compressed[compIdx + 1];
    uint8_t compThree = compressed[compIdx + 2];
    uint16_t first = (((uint16_t)compOne) << 4) | (compTwo >> 4);
    uint16_t second = (((uint16_t)(compTwo & 0x0f)) << 4) | compThree;
    image[imgIdx] = first;
    image[imgIdx + 1] = second;
    
    imgIdx += 2;
    compIdx += 3;
  }
}*/ 

void enqueueImage(uint8_t *compressed) {
  if (getQueueSize(priorityQueue) > 1) {
    return;
  }    
  Packet packet;
  setPacketHeader(&packet, GRID_FN_CODE_A, imgId, 0);
  copyCompressedToPacket(&packet, compressed);
  enqueue(priorityQueue, packet);

  setPacketHeader(&packet, GRID_FN_CODE_B, imgId, 0);
  copyCompressedToPacket(&packet, compressed + 32);
  enqueue(priorityQueue, packet);

  setPacketHeader(&packet, GRID_FN_CODE_C, imgId, 0);
  copyCompressedToPacket(&packet, compressed + 64);
  enqueue(priorityQueue, packet);
  
  imgId++;
}

void copyCompressedToPacket(Packet *packet, uint8_t *compressed) {
  for (int i = 0; i < 32; i++) {
    packet->ArrayType.oneByte[i] = compressed[i];
  }    
}