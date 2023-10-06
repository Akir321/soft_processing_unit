#ifndef  __SPU_H__
#define  __SPU_H__

#include <stdio.h>
#include "stack.h"

const int MaxCommandLength = 10;
const int PrecisionConst   = 100;

const int RegistersNumber  = 4;

enum spuErrors
{
    STACK_UNDERFLOW  = 1,
    INCORECT_PUSH    = 2,
    UNKNOWN_COMMAND  = 3,
    DIVISION_BY_ZERO = 4,
    FILE_ERROR       = 5,
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

struct Processor
{
    stack stk;
    int registers[RegistersNumber];
};

int spuCtor (Processor *spu, size_t stackCapacity);
int spuDtor (Processor *spu);
int spuError(Processor *spu);
int spuDump (Processor *spu, const char *file, int line, const char *function);

int runSPU(Processor *spu, FILE *fin, FILE *fout);

int processCommand(int command, stack *spuStack, FILE *fin, FILE *fout);

#endif //__SPU_H__
