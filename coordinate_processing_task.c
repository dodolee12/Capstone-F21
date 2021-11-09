#include <stdlib.h>

#include "coordinate_processing_task.h"
#include "queue.h"
#include "command.h"

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn      coordinateProcessing_createTask
 *
 * @brief   Task creation function for the processing buffered coordinates.
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void coordinate_processing_create_task(){
    coord_queue = create_queue(10000);
    pthread_mutex_init(&queue_lock,NULL);
    pthread_cond_init(&queue_not_empty,NULL);

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


    retc = pthread_create(&thread, &pAttrs, &process_queue_task_fxn, NULL);

    if (retc != 0)
    {
        while (1)
            ;
    }
}

/*******************************************************************************
 * @fn      processQueue_taskFxn
 *
 * @brief   Application task entry point for processing buffered coordinates.
 *
 ******************************************************************************/
void* process_queue_task_fxn(void* args){

    //application loop
    while(1){
        pthread_mutex_lock(&queue_lock);
        while(is_empty(coord_queue)){
            pthread_cond_wait(&queue_not_empty, &queue_lock);
        }
        command_t* next_coord = front(coord_queue);
        dequeue(coord_queue);
        pthread_mutex_unlock(&queue_lock);

        //Process coord
        volatile int a = 3;

    }
}
