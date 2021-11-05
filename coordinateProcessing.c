#include <stdlib.h>

#include "coordinateProcessing.h"
#include "queue.h"
#include "command.h"

void coordinateProcessing_createTask(){
    coordQueue = createQueue();
    pthread_mutex_init(&queueLock,NULL);
    pthread_cond_init(&queueNotEmpty,NULL);

    //everything below is for OS thread logic

    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = COORDINATE_PROCESSING_PRIORITY;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);

    if (retc != 0)
    {
        while (1)
            ;
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, COORDINATE_PROCESSING_STACK_SIZE);

    if (retc != 0)
    {
        while (1)
            ;
    }


    retc = pthread_create(&thread, &pAttrs, &processQueue, NULL);

    if (retc != 0)
    {
        while (1)
            ;
    }
}


void* processQueue(void* args){

    //application loop
    while(1){
        pthread_mutex_lock(&queueLock);
        while(coordQueue->front == NULL){
            pthread_cond_wait(&queueNotEmpty, &queueLock);
        }
        struct Command* nextCoord = (struct Command*) coordQueue->front->val;
        deQueue(coordQueue);
        pthread_mutex_unlock(&queueLock);

        //Process coord
        volatile int a = 3;

//        if(nextCoord->special == 0){
//            printf("Processing coord (%d,%d)\n", nextCoord->x, nextCoord->y);
//        }
//        else{
//            switch(nextCoord->special){
//                case 1:
//                    printf("Starting\n");
//                    break;
//                case 2:
//                    printf("Lift Pen\n");
//                    break;
//                case 3:
//                    printf("Done.\n");
//                    break;
//            }
//        }
        free(nextCoord);
    }

    return NULL;
}
