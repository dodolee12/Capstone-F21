/*
 * pin_inits.c
 *
 * @brief: Initialization implementations for the pin inits
 *
 *  Created on: Nov 10, 2021
 *      Author: Do
 */


#include "pin_inits.h"
#include "Board.h"
#include <ti/drivers/GPIO.h>
#include <stdlib.h>
#include <stdbool.h>

bool printer_free = true;

/*
 * Turn on Solenoid
 */
void enable_solenoid(){
    GPIO_write(Board_SOLENOID_INP,1);
}

/*
 * Turn off Solenoid
 */
void disable_solenoid(){
    GPIO_write(Board_SOLENOID_INP,0);
}

/* Pin configurations methods */

/***
 * Set pin as an input pullup resistor
 */
void set_pin_config_input_pullup(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_IN_PU);
}

/***
 * Set pin as output with write value 1
 */
void set_pin_config_output_high(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

}


/***
 * Set pin as output with write value 0
 */
void set_pin_config_output_low(uint_least8_t pin){
    GPIO_setConfig(pin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

void initialize_motor_timer(){
    Timer_Params_init(&timer_motor_params);
    timer_motor_params.period = 1000;
    timer_motor_params.periodUnits = Timer_PERIOD_US;
    timer_motor_params.timerMode = Timer_ONESHOT_CALLBACK;
    timer_motor_params.timerCallback = motor_callback_dummy;

    timer_motor = Timer_open(MSP_EXP432E401Y_TIMER3, &timer_motor_params);

    if (timer_motor == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
}

void motor_callback_dummy(Timer_Handle myHandle, int_fast16_t status){

}

/* motor length timer methods */
//State defines how many timers are active. 0 is both, 1 is only motor1, 2 is only motor2
void intialize_and_start_motor_timer(int period, int state){
    Timer_stop(timer_motor);
    Timer_close(timer_motor);
    Timer_Params_init(&timer_motor_params);
    timer_motor_params.period = abs(period);
    timer_motor_params.periodUnits = Timer_PERIOD_US;
    timer_motor_params.timerMode = Timer_ONESHOT_CALLBACK;
    switch(state){
        case 0:
            timer_motor_params.timerCallback = motor_init_callback_state0;
        break;
        case 1:
            timer_motor_params.timerCallback = motor_init_callback_state1;
        break;
        case 2:
            timer_motor_params.timerCallback = motor_init_callback_state2;
        break;
    }

    timer_motor = Timer_open(MSP_EXP432E401Y_TIMER3, &timer_motor_params);

    if (timer_motor == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
    if (Timer_start(timer_motor) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    printer_free = false;
    while(!printer_free);
}

void motor_init_callback_state0(Timer_Handle myHandle, int_fast16_t status){
    stop_motor1();
    stop_motor2();
    printer_free = true;
}

void motor_init_callback_state1(Timer_Handle myHandle, int_fast16_t status){
    stop_motor1();
    printer_free = true;
}

void motor_init_callback_state2(Timer_Handle myHandle, int_fast16_t status){
    stop_motor2();
    printer_free = true;
}

/* motor driver 1 methods */
/***
 * Initiazlize the timer for pulses sent to stepper motor 1
 */
void initialize_motor1_timer(int frequency){
    Timer_Params_init(&timer_motor1_params);
    timer_motor1_params.period = abs(frequency);
    timer_motor1_params.periodUnits = Timer_PERIOD_HZ;
    timer_motor1_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_motor1_params.timerCallback = motor1_timer_callback;

    timer_motor1 = Timer_open(MSP_EXP432E401Y_TIMER4, &timer_motor1_params);

    if (timer_motor1 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
}

void start_motor1_timer(){
    if (Timer_start(timer_motor1) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}

/***
 * Start pulses on motor 1
 * To start the motor, we enable, set direction, microstepping and start timer
 */
void start_motor1(int frequency, direction_t direction, microstep_t microstep){
    GPIO_write(Board_MOTOR1_EN,0); //enable motor1
    GPIO_write(Board_MOTOR1_DIR,direction); //set direction
    set_motor1_microstepping(microstep); //set microstep
    initialize_motor1_timer(frequency);
    start_motor1_timer();
}

/***
 * Stop pulses on motor 1, make enable pin low
 */
void stop_motor1(){
    Timer_stop(timer_motor1);
    GPIO_write(Board_MOTOR1_STEP,0);
    GPIO_write(Board_MOTOR1_EN,1); //disable motor1
    Timer_close(timer_motor1);
}

/***
 * Called every 1000 microseconds or 1000 Hz. Callback to pulse on motor 1
 */
void motor1_timer_callback(Timer_Handle myHandle, int_fast16_t status){
    GPIO_toggle(Board_MOTOR1_STEP);
    GPIO_toggle(Board_MOTOR1_STEP);
}

/*
 * Set microstepping of motor1
 */
void set_motor1_microstepping(microstep_t microstep){
    switch(microstep){
        case FULL:
            GPIO_write(Board_MOTOR1_M1,0);
            GPIO_write(Board_MOTOR1_M2,0);
            GPIO_write(Board_MOTOR1_M3,0);
        break;
        case HALF:
            GPIO_write(Board_MOTOR1_M1,1);
            GPIO_write(Board_MOTOR1_M2,0);
            GPIO_write(Board_MOTOR1_M3,0);
        break;
        case QUARTER:
            GPIO_write(Board_MOTOR1_M1,0);
            GPIO_write(Board_MOTOR1_M2,1);
            GPIO_write(Board_MOTOR1_M3,0);
        break;
        case EIGHTH:
            GPIO_write(Board_MOTOR1_M1,1);
            GPIO_write(Board_MOTOR1_M2,1);
            GPIO_write(Board_MOTOR1_M3,0);
        break;
        case SIXTEENTH:
            GPIO_write(Board_MOTOR1_M1,1);
            GPIO_write(Board_MOTOR1_M2,1);
            GPIO_write(Board_MOTOR1_M3,1);
        break;
    }
}


/* motor driver 2 methods */

/***
 * Initiazlize the timer for pulses sent to stepper motor 2
 */
void initialize_motor2_timer(int frequency){
    Timer_Params_init(&timer_motor2_params);
    timer_motor2_params.period = abs(frequency);
    timer_motor2_params.periodUnits = Timer_PERIOD_HZ;
    timer_motor2_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_motor2_params.timerCallback = motor2_timer_callback;

    timer_motor2 = Timer_open(MSP_EXP432E401Y_TIMER5, &timer_motor2_params);

    if (timer_motor2 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
}

void start_motor2_timer(){
    if (Timer_start(timer_motor2) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}

/***
 * Start pulses on motor 2
 */
void start_motor2(int frequency, direction_t direction, microstep_t microstep){
    GPIO_write(Board_MOTOR2_EN,0); //enable motor1
    GPIO_write(Board_MOTOR2_DIR,direction); //set direction
    set_motor2_microstepping(microstep); //set microstep
    initialize_motor2_timer(frequency);
    start_motor2_timer();
}

/***
 * Stop pulses on motor 2
 */
void stop_motor2(){
    Timer_stop(timer_motor2);
    GPIO_write(Board_MOTOR2_STEP,0);
    GPIO_write(Board_MOTOR2_EN,1); //disable motor1
    Timer_close(timer_motor2);
}

/***
 * Called every 1000 microseconds or 1000 Hz. Callback to pulse on motor 1
 */
void motor2_timer_callback(Timer_Handle myHandle, int_fast16_t status){
    GPIO_toggle(Board_MOTOR2_STEP);
    GPIO_toggle(Board_MOTOR2_STEP);
}

/*
 * Set microstepping of motor1
 */
void set_motor2_microstepping(microstep_t microstep){
    switch(microstep){
        case FULL:
            GPIO_write(Board_MOTOR2_M1,0);
            GPIO_write(Board_MOTOR2_M2,0);
            GPIO_write(Board_MOTOR2_M3,0);
        break;
        case HALF:
            GPIO_write(Board_MOTOR2_M1,1);
            GPIO_write(Board_MOTOR2_M2,0);
            GPIO_write(Board_MOTOR2_M3,0);
        break;
        case QUARTER:
            GPIO_write(Board_MOTOR2_M1,0);
            GPIO_write(Board_MOTOR2_M2,1);
            GPIO_write(Board_MOTOR2_M3,0);
        break;
        case EIGHTH:
            GPIO_write(Board_MOTOR2_M1,1);
            GPIO_write(Board_MOTOR2_M2,1);
            GPIO_write(Board_MOTOR2_M3,0);
        break;
        case SIXTEENTH:
            GPIO_write(Board_MOTOR2_M1,1);
            GPIO_write(Board_MOTOR2_M2,1);
            GPIO_write(Board_MOTOR2_M3,1);
        break;
    }
}

/* Switch methods */

/**
 * Initialize switch1's interrupt
 */
void initialize_switch1_interrupt(){
    GPIO_setCallback(Board_SWITCH1_INT, stop_motor1_interrupt_callback);

    GPIO_enableInt(Board_SWITCH1_INT);
}

/**
 * Initialize switch2's interrupt
 */
void initialize_switch2_interrupt(){
    GPIO_setCallback(Board_SWITCH2_INT, stop_motor1_interrupt_callback);

    GPIO_enableInt(Board_SWITCH2_INT);
}

/**
 * Initialize switch3's interrupt
 */
void initialize_switch3_interrupt(){
    GPIO_setCallback(Board_SWITCH3_INT, stop_motor2_interrupt_callback);

    GPIO_enableInt(Board_SWITCH3_INT);
}

/**
 * Initialize switch4's interrupt
 */
void initialize_switch4_interrupt(){
    GPIO_setCallback(Board_SWITCH4_INT, stop_motor2_interrupt_callback);

    GPIO_enableInt(Board_SWITCH4_INT);
}

/**
 * Callback
 */
void stop_motor1_interrupt_callback(uint_least8_t index){
    stop_motor1();
}

/**
 * Callback
 */
void stop_motor2_interrupt_callback(uint_least8_t index){
    stop_motor2();
}
