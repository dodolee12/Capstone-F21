/** @file coordinate_processing_task.h
*
* @brief The coordinate processing thread
*
*/
#ifndef COORDINATE_PROCESSING_TASK_H
#define COORDINATE_PROCESSING_TASK_H

#include <pthread.h>
#include "queue.h"

#define COORDINATE_PROCESSING_PRIORITY 2
#define COORDINATE_PROCESSING_STACK_SIZE 4096

pthread_t thread;

//bufferqueue and mutexes
queue_t* coord_queue;
pthread_mutex_t queue_lock;
pthread_cond_t queue_not_empty;

void coordinate_processing_create_task();

void* process_queue_task_fxn(void* args);


#endif
