/*
 * testTask.c
 *
 *  Created on: Nov 5, 2021
 *      Author: Do
 */




#include <stdlib.h>
#include <pthread.h>

#define TEST_PRIORITY 1
#define TEST_STACK_SIZE 32768

pthread_t thread;


void test_createTask(){
    //everything below is for OS thread logic

    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = TEST_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);

    if (retc != 0)
    {
        while (1)
            ;
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, TEST_STACK_SIZE);

    if (retc != 0)
    {
        while (1)
            ;
    }


    retc = pthread_create(&thread, &pAttrs, &testMethod, NULL);

    if (retc != 0)
    {
        while (1)
            ;
    }
}

//DO ALL TESTING HERE
void* testMethod(void* args){

}
}
