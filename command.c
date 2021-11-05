#include "command.h"
#include <stdlib.h>

struct Command* createCommand(int x, int y, int special){
    struct Command* command = (struct Command*)malloc(sizeof(struct Command));
    command->x = x;
    command->y = y;
    command->special = special;
    
    return command;
}