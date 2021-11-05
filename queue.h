#ifndef QUEUE_H
#define QUEUE_H

struct QNode {
    void* val;
    struct QNode* next;
};

struct Queue {
    struct QNode *front, *rear;
};
struct QNode* newNode(void* k);
struct Queue* createQueue();
void enQueue(struct Queue* q, void* k);
void deQueue(struct Queue* q);


#endif