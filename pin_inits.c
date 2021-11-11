/*
 * pin_inits.c
 *
 *  Created on: Nov 10, 2021
 *      Author: Do
 */


#include "pin_inits.h"
#include "Board.h"
#include <ti/drivers/GPIO.h>


/*
 * This callback is called every 3000 microseconds.
 */
void PWM_timer_callback(Timer_Handle myHandle, int_fast16_t status)
{
    GPIO_toggle(Board_PK2);
}

/***
* Simulate PWM with timer interrupts on PK2 with 50% duty cycle
*/

void initialize_PWM_timer(){
    Timer_Params_init(&timer_PWM_params);
    timer_PWM_params.period = 1500;
    timer_PWM_params.periodUnits = Timer_PERIOD_US;
    timer_PWM_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_PWM_params.timerCallback = PWM_timer_callback;

    timer_PWM = Timer_open(MSP_EXP432E401Y_TIMER3, &timer_PWM_params);

    if (timer_PWM == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
}

/***
 * Start the PWM on PK2
 *
 */

void output_solenoid_duty_cycle_adjust(){
    if (Timer_start(timer_PWM) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}

/***
 * Stop timer aka stop PWM
 */
void stop_output_solenoid_duty_cycle_adjust(){
    Timer_stop(timer_PWM);
    GPIO_write(Board_PK2,0);
}

/* Pin configurations methods */

void set_pin_config_input_pullup(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_IN_PU);
}

void set_pin_config_output_high(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

}
void set_pin_config_output_low(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

/* motor driver 1 methods */
void initialize_motor1_timer(int period){
    Timer_Params_init(&timer_motor1_params);
    timer_motor1_params.period = period;
    timer_motor1_params.periodUnits = Timer_PERIOD_US;
    timer_motor1_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_motor1_params.timerCallback = motor1_timer_callback;

    timer_motor1 = Timer_open(MSP_EXP432E401Y_TIMER4, &timer_motor1_params);

    if (timer_motor1 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
}

void start_motor1(){
    if (Timer_start(timer_motor1) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}

void stop_motor1(){
    Timer_stop(timer_motor1);
    GPIO_write(Board_PK2,0);
    Timer_close(timer_motor1);
}

/***
 * Called every 1000 microseconds or 1000 Hz
 */
void motor1_timer_callback(Timer_Handle myHandle, int_fast16_t status){
    GPIO_toggle(Board_PD5);
    GPIO_toggle(Board_PD5);
}

/* Switch methods */

void initialize_switch1_interrupt(){
    GPIO_setCallback(Board_PM2, switch1_interrupt_callback);

    GPIO_enableInt(Board_PM2);
}

void switch1_interrupt_callback(uint_least8_t index){
    GPIO_write(Board_PP1,0);
}
