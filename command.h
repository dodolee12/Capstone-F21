#ifndef COMMAND_H
#define COMMAND_H

struct Command{
    short x;
    short y;
    char special; //1 is start, 2 is lift pen, 3 is stop  (change to ENUM)
};

struct Command* createCommand(int x, int y, int liftPen);

#endif
