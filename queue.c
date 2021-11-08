#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*!
* @brief Initialization function for queue_t object
*
* @param[in] capacity The max capacity of the queue
*
* @return The created queue_t object
*/
queue_t* create_queue(int capacity)
{
    queue_t* queue = (queue_t*)malloc(
        sizeof(queue_t));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (command_t*) malloc(
        queue->capacity * sizeof(command_t));
    return queue;
}

/*!
* @brief Helper whether queue is full
*
* @param[in] queue: pointer to the queue object
*
* @return Whether queue is full
*/
int is_full(queue_t* queue)
{
    return (queue->size == queue->capacity);
}

/*!
* @brief Helper whether queue is empty
*
* @param[in] queue: pointer to the queue object
*
* @return Whether queue is empty
*/
int is_empty(queue_t* queue)
{
    return (queue->size == 0);
}
 
/*!
* @brief enqueue object
*
* @param[in] queue: pointer to the queue object
* @param[in] k: command object to enqueue
*
* @return void
*/
void enqueue(queue_t* queue, command_t k)
{
    if (is_full(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = k;
    queue->size = queue->size + 1;
}
 
/*!
* @brief dequeue object
*
* @param[in] queue: pointer to the queue object
*
* @return void
*/
void dequeue(queue_t* queue)
{
    if (is_empty(queue))
        return;
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
}

/*!
* @brief get front of queue
*
* @param[in] queue: pointer to the queue object
*
* @return item at front of queue
*/
command_t* front(queue_t* queue){
    return &queue->array[queue->front];
}
