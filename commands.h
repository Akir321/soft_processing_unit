#ifndef  __COMMANDS_H__
#define  __COMMANDS_H__

const int MaxCommandLength = 16;
const int RegistersNumber  = 4;

const char * const Signature = "will";

enum commands
{
    HLC    = -1,
    PUSH   =  0,
    IN     =  1,
    OUT    =  2,
    ADD    =  3,
    SUB    =  4,
    MUL    =  5,
    DIV    =  6,
    SQRT   =  7,
    POP    =  8,
    PUSH_R = 33
};

#endif //__COMMANDS_H__