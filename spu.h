#ifndef  __SPU_H__
#define  __SPU_H__

#include "stack.h"

const int MaxCommandLength = 10;
const int PrecisionConst   = 100;

enum errors
{
    STACK_UNDERFLOW  = 1,
    INCORECT_PUSH    = 2,
    UNKNOWN_COMMAND  = 3,
    DIVISION_BY_ZERO = 4
};

int runSPU(FILE *fin, FILE *fout);

int processCommand(char *command, stack *spuStack, FILE *fin, FILE *fout);

#endif //__SPU_H__
