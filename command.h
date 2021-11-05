#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    COORDINATE,
    START,
    LIFT_PEN,
    STOP,
} command_type;

struct Command{
    int x;
    int y;
    command_type type; //1 is start, 2 is lift pen, 3 is stop
};


struct Command* createCommand(int x, int y, command_type type);

#endif
