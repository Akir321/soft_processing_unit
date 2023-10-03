#ifndef  __SPU_H__
#define  __SPU_H__

#include <stdio.h>
#include "stack.h"

const int MaxCommandLength = 10;
const int PrecisionConst   = 100;

enum spuErrors
{
    STACK_UNDERFLOW  = 1,
    INCORECT_PUSH    = 2,
    UNKNOWN_COMMAND  = 3,
    DIVISION_BY_ZERO = 4
};

enum commands
{
    HLC  = -1,
    PUSH =  0,
    IN   =  1,
    OUT  =  2,
    ADD  =  3,
    SUB  =  4,
    MUL  =  5,
    DIV  =  6,
    SQRT =  7
};

int runSPU(FILE *fin, FILE *fout);

int processCommand(char *command, stack *spuStack, FILE *fin, FILE *fout);

#endif //__SPU_H__
