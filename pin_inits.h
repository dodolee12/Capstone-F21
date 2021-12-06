/*
 * pin_inits.h
 *
 * @brief: Initialization for certain pins for motor/solenoid/switches
 *
 *  Created on: Nov 10, 2021
 *      Author: Do
 */

#ifndef PIN_INITS_H_
#define PIN_INITS_H_

#include <ti/drivers/Timer.h>
#include <stdbool.h>

Timer_Handle timer_PWM;
Timer_Params timer_PWM_params;

/* Solenoid driver Methods */
void enable_solenoid();
void disable_solenoid();

/* pin configuration methods */
void set_pin_config_input_pullup(uint_least8_t pin);
void set_pin_config_output_high(uint_least8_t pin);
void set_pin_config_output_low(uint_least8_t pin);

//This motor timer is the timer that controls how both motors are on for
Timer_Handle timer_motor;
Timer_Params timer_motor_params;

void initialize_motor_timer();
void motor_callback_dummy(Timer_Handle myHandle, int_fast16_t status);
void intialize_and_start_motor_timer(int period,int state);
void motor_init_callback_state0(Timer_Handle myHandle, int_fast16_t status);
void motor_init_callback_state1(Timer_Handle myHandle, int_fast16_t status);
void motor_init_callback_state2(Timer_Handle myHandle, int_fast16_t status);

/*
 *  Microstepping of motor
 */
typedef enum{
    FULL,
    HALF,
    QUARTER,
    EIGHTH,
    SIXTEENTH,
} microstep_t;

/*
 * Direction of Motor
 */
typedef enum{
    LEFT,
    RIGHT,
} direction_t;

Timer_Handle timer_motor1;
Timer_Params timer_motor1_params;

/* Motor Driver 1 Methods */
void initialize_motor1_timer(int frequency);
void motor1_timer_callback(Timer_Handle myHandle, int_fast16_t status);
void start_motor1_timer();
void start_motor1(int frequency, direction_t direction, microstep_t microstep);
void stop_motor1();
void set_motor1_microstepping(microstep_t microstep);

Timer_Handle timer_motor2;
Timer_Params timer_motor2_params;

/* Motor Driver 2 Methods */
void initialize_motor2_timer(int frequency);
void motor2_timer_callback(Timer_Handle myHandle, int_fast16_t status);
void start_motor2_timer();
void start_motor2(int frequency, direction_t direction, microstep_t microstep);
void stop_motor2();
void set_motor2_microstepping(microstep_t microstep);

/* Switch Initialization Methods
 * Fix these when setting up switches
 */
void initialize_switch1_interrupt();
void initialize_switch2_interrupt();
void initialize_switch3_interrupt();
void initialize_switch4_interrupt();
void stop_motor1_interrupt_callback(uint_least8_t index);
void stop_motor2_interrupt_callback(uint_least8_t index);


#endif /* PIN_INITS_H_ */
