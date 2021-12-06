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
#define BASE_MOTOR_SPEED 120 // pixels per sec
#define BASE_MOTOR_FREQUENCY 1000
#define TIME_PER_COORDINATE 0.001  // time per coordinate in sec (1ms)

pthread_t thread;

//bufferqueue and mutexes
queue_t* coord_queue;
pthread_mutex_t queue_lock;
pthread_cond_t queue_not_empty;

void coordinate_processing_create_task();

void* process_queue_task_fxn(void* args);


#endif
