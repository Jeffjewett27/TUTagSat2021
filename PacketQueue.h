/*
 * Author: Caleb Eckerly
 * Modified By: Jeff Jewett
 * Modification Date: 9/22/20
 */

#include "Packet.h"

#ifndef QUEUE_NODE
#define QUEUE_NODE
struct PacketNode{
  Packet value;
  struct PacketNode *next;
};
typedef struct node node;
#endif

#ifndef QUEUE_STRUCT
#define QUEUE_STRUCT
//a queue, duh
struct PacketQueue{
  int size;
  node *front;
  node *back;
};
typedef struct PacketQueue PacketQueue;
#endif

// Prototypes
void initializeQueue(PacketQueue *q);
int isQueueEmpty(PacketQueue *q);
void clearQueue(PacketQueue *q);
int getQueueSize(PacketQueue *q);
Packet peekQueue(PacketQueue *q);
void enqueue(PacketQueue *q, Packet val);
Packet dequeue(PacketQueue *q);


