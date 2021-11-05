#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// A utility function to create an empty queue
struct Queue* createQueue(int capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (struct Command*) malloc(
        queue->capacity * sizeof(struct Command));
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// The function to add a key k to q
void enQueue(struct Queue* queue, struct Command k)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = k;
    queue->size = queue->size + 1;
}
 
// Function to remove a key from given queue q
void deQueue(struct Queue* queue)
{
    if (isEmpty(queue))
        return;
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
}

struct Command* front(struct Queue* queue){
    return &queue->array[queue->front];
}
