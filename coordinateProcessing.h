#ifndef COORDINATEPROCESSING_H
#define COORDINATEPROCESSING_H

#include <pthread.h>

#define COORDINATE_PROCESSING_PRIORITY 1
#define COORDINATE_PROCESSING_STACK_SIZE 32768

pthread_t thread;

//bufferqueue and mutexes
struct Queue* coordQueue;
pthread_mutex_t queueLock;
pthread_cond_t queueNotEmpty;

void coordinateProcessing_createTask();

void* processQueue();


#endif
