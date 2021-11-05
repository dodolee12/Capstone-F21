#include "command.h"
#include <stdlib.h>

struct Command* createCommand(int x, int y, command_type type){
    struct Command* command = (struct Command*)malloc(sizeof(struct Command));
    command->x = x;
    command->y = y;
    command->type = type;
    
    return command;
}
