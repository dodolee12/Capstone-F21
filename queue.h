#ifndef QUEUE_H
#define QUEUE_H

#include "command.h"

struct Queue {
    int front, rear, size;
    unsigned capacity;
    struct Command* array;
};

struct Queue* createQueue(int capacity);
void enQueue(struct Queue* q, struct Command k);
void deQueue(struct Queue* q);
int isEmpty(struct Queue* queue);
int isFull(struct Queue* queue);
struct Command* front(struct Queue* queue);



#endif
