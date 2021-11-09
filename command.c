/** @file command.c
*
* @brief Implementation of methods defined in command.h
*
*/

#include "command.h"
#include <stdlib.h>

/*!
* @brief Initialization function for command_t object
*
* @param[in] x The x coordinate in command
* @param[in] y The y coordinate in command
* @param[in] type The type of command
*
* @return The created command_t object
*/
command_t* create_command(uint16_t x, uint16_t y, command_type type){
    command_t* command = (command_t*) malloc(sizeof(command_t));
    command->x = x;
    command->y = y;
    command->type = type;
    
    return command;
}
