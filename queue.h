/*
 * @file queue.h
 *
 *
 * @brief: queue implementation in c
 *
 *  Created on: Nov 5, 2021
 *      Author: Do
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "command.h"

/* Struct for queue implementation to be used in the buffer */
typedef struct {
    int front, rear, size;
    unsigned capacity;
    command_t* array;
} queue_t;

queue_t* create_queue(int capacity);
void enqueue(queue_t* q, command_t k);
void dequeue(queue_t* q);
int is_empty(queue_t* queue);
int is_full(queue_t* queue);
command_t* front(queue_t* queue);



#endif
