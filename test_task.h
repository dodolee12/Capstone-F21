/*
 * testTask.h
 *
 *  Created on: Nov 5, 2021
 *      Author: Do
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_

#include <pthread.h>

#define TEST_PRIORITY 1
#define TEST_STACK_SIZE 2048

pthread_t thread;

void* test_method(void* args);
void test_create_task();



#endif /* TESTTASK_H_ */
