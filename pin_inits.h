/*
 * pin_inits.h
 *
 *  Created on: Nov 10, 2021
 *      Author: Do
 */

#ifndef PIN_INITS_H_
#define PIN_INITS_H_

#include <ti/drivers/Timer.h>

Timer_Handle timer_PWM;
Timer_Params timer_PWM_params;

/* Solenoid driver Methods */
void PWM_timer_callback(Timer_Handle myHandle, int_fast16_t status);
void initialize_PWM_timer();
void output_solenoid_duty_cycle_adjust();
void stop_output_solenoid_duty_cycle_adjust();

/* pin configuration methods */
void set_pin_config_input_pullup(uint_least8_t pin);
void set_pin_config_output_high(uint_least8_t pin);
void set_pin_config_output_low(uint_least8_t pin);


Timer_Handle timer_motor1;
Timer_Params timer_motor1_params;

/* Motor Driver 1 Methods */
void initialize_motor1_timer(int period);
void motor1_timer_callback(Timer_Handle myHandle, int_fast16_t status);
void start_motor1();
void stop_motor1();


/* Switch Initialization Methods */
void initialize_switch1_interrupt();
void switch1_interrupt_callback(uint_least8_t index);


#endif /* PIN_INITS_H_ */
