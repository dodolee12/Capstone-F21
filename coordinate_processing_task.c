/** @file coordinate_processing_task.c
*
* @brief The coordinate processing thread implementation
*
*/

#include <stdlib.h>
#include <stdbool.h>
#include <ti/drivers/GPIO.h>
#include <unistd.h>

#include "coordinate_processing_task.h"
#include "queue.h"
#include "command.h"
#include "pin_inits.h"
#include "Board.h"

extern bool printer_free;

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

    //Previous x and y coordinates for calculating slope
    int prevx = 0;
    int prevy = 1000;

    //application loop
    while(1){
        pthread_mutex_lock(&queue_lock);
        while(is_empty(coord_queue)){
            pthread_cond_wait(&queue_not_empty, &queue_lock);
        }
        command_t* next_coord = front(coord_queue);
        dequeue(coord_queue);
        pthread_mutex_unlock(&queue_lock);

        //process coord


        //check lift pen command
        switch(next_coord->type){
            case START:
                //lift pen before calibrating

                //Then move to 0,0 by triggering interrupts
                prevx = 0;
                prevy = 0;
            break;
            case COORDINATE:

                //Step 1: calculate the slope
                //if slope is < 1/32 or > 32, then stall certain motor
                //if less than 1/32, we draw horizontal line, if greater than 32, then draw vertical line
                //Reference: took 16 sec to move 1920 pixels on 1khz, this is 120pixels/sec
                //find the smaller change in x or y, this will be the base which is 2khz aka 240 pixels/s
                //find time it takes to travel x coordinates so deltax/240
                //then find period
                //direction is set by sign
                //microstepping is set by magnitude


                //lets make motor1 x axis and motor2 y axis

                //calculate configurations on both x and y motors
                ;
                int newx = next_coord->x;
                int newy = next_coord->y;
                if(newy == prevy && newx == prevx){
                    printer_free = true;
                    break;
                }
                double slope;
                if(newy == prevy){
                    slope = (newx - prevx < 0 ? -1 : 1) * 1.0/100;
                }
                else if(newx == prevx){
                    slope = (newy - prevy < 0 ? -1 : 1) * 100;
                }
                else{
                    slope = (double) (newy - prevy) / (newx - prevx);
                }

                //SINCE MOTOR FULL STEPPING DOSENT WORK FOR NOW LETS LIMIT TO 1/16 and 16

                //if between -1 and 1 then delta y is smaller
                if(slope <= 1 && slope >= -1){

                    //if the slope is too small for noticeable change, we can draw horizontal line otherwise set y motor speed
                    if(slope < 1.0/16 && slope > -1.0/16){
                        //dont turn on y motor
                        //set x motor as base motor
                        int deltax = newx - prevx;
                        double time = (double) deltax/BASE_MOTOR_SPEED*1000000; //time to keep motor on at 2khz in us (240 pixels/sec)
                        start_motor1(BASE_MOTOR_FREQUENCY,deltax > 0 ? RIGHT : LEFT,SIXTEENTH); // base motor settings
                        intialize_and_start_motor_timer(time,1); // only motor 1 is on
                        //usleep(abs(time));
                    }
                    else{
                        //base motor is y motor
                        int deltay = newy - prevy;
                        int deltax = newx - prevx;
                        double time = (double) deltay/BASE_MOTOR_SPEED*1000000; //time to keep motor on at 2khz in us (240 pixels/sec)
                        //now determine freq and microstepping in motor1
                        double motor1_freq = (double) BASE_MOTOR_FREQUENCY/slope;
                        microstep_t microstep = SIXTEENTH;
                        while(motor1_freq > 4000){
                            motor1_freq /= 2;
                            --microstep;
                        }
                        start_motor2(BASE_MOTOR_FREQUENCY*1,deltay > 0 ? RIGHT : LEFT,SIXTEENTH); // base motor settings
                        start_motor1(motor1_freq,deltax > 0 ? RIGHT : LEFT,microstep); // base motor settings
                        intialize_and_start_motor_timer(time,0); // both motors on
                    }

                }
                else{
                    //check if vertical line
                    if(slope < -16 || slope > 16){
                        //only y motor
                        int deltay = newy - prevy;
                        double time = (double) deltay/BASE_MOTOR_SPEED*1000000; //time to keep motor on at 2khz in us (240 pixels/sec)
                        start_motor2(BASE_MOTOR_FREQUENCY*1,deltay > 0 ? RIGHT : LEFT,SIXTEENTH); // base motor settings
                        intialize_and_start_motor_timer(time,2); // only motor 2 is on
                    }
                    else{
                        //base motor is x motor
                        int deltax = newx - prevx;
                        int deltay = newy - prevy;
                        double time = (double) deltax/BASE_MOTOR_SPEED*1000000; //time to keep motor on at 2khz in us (240 pixels/sec)
                        //now determine freq and microstepping in motor1
                        double motor2_freq = (double) BASE_MOTOR_FREQUENCY*slope*1;
                        microstep_t microstep = SIXTEENTH;
                        while(motor2_freq > 4000){
                            motor2_freq /= 2;
                            --microstep;
                        }
                        start_motor1(BASE_MOTOR_FREQUENCY,deltax > 0 ? RIGHT : LEFT,SIXTEENTH); // base motor settings
                        start_motor2(motor2_freq,deltay > 0 ? RIGHT : LEFT,microstep); // base motor settings
                        intialize_and_start_motor_timer(time,0); // both motors on
                    }
                }

                prevx = newx;
                prevy = newy;

                enable_solenoid();
            break;
            case LIFT_PEN:
                disable_solenoid();
                usleep(100000);
            break;
            case STOP:
                //do the same as start: lift pen, move to 0,0
                prevx = 0;
                prevy = 0;
            break;
            case TEST:
                //random stuff for testing
                start_motor1(BASE_MOTOR_FREQUENCY,RIGHT,HALF);
                intialize_and_start_motor_timer(1000000,1);
            break;
        }

    }
}
