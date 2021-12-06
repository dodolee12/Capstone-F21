/** @file command.h
*
* @brief A struct defined for the command given to the micro-controller
*
*/

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

/**
 * Type of command sent
 */
typedef enum {
    COORDINATE,
    START,
    LIFT_PEN,
    STOP,
    TEST,
} command_type;

typedef struct {
    uint16_t x;
    uint16_t y;
    command_type type; //1 is start, 2 is lift pen, 3 is stop
} command_t;

command_t* create_command(uint16_t x, uint16_t y, command_type type);

#endif
