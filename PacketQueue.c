/*
 * Author: Caleb Eckerly
 * Modified By: Jeff Jewett
 * Modification Date: 9/22/20
 */

#include "simpletools.h"
#include "PacketQueue.h"

void initializeQueue(PacketQueue *q){
  q->size = 0;
  q->front = 0;
  q->back = 0;
}
  
int isQueueEmpty(PacketQueue *q){
  return (getQueueSize(q) == 0);
}
  
void clearQueue(PacketQueue *q){
  while(q->size != 0){
    dequeue(q);
  }
}

int getQueueSize(PacketQueue *q){
  return(q->size);
}  
      
Packet peekQueue(PacketQueue *q){
  return(q->front->value);
}

void enqueue(PacketQueue *q, Packet val){  
  PacketNode *finger;
  finger = malloc(sizeof(PacketNode));
  finger->value = val;
  finger->next = 0;
  if(isQueueEmpty(q)){
    q->front = finger;
    q->back = q->front;
  }else{
    q->back->next = finger;
    q->back = finger;
  }
  q->size++;
}     
    
Packet dequeue(PacketQueue *q){
 PacketNode *finger = q->front;
 Packet value = q->front->value;
 q->front = q->front->next;
 q->size--;
 free(finger);
 if(q->size == 0) initializeQueue(q);
 return value;
}

/* (Below shows how to declare and initialize an instance of the queue)
int main(){
  PacketQueue *q;
  q = malloc(sizeof(PacketQueue));
  initializeQueue(q);    
} 
*/  