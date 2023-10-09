#ifndef  __SPU_H__
#define  __SPU_H__

#include <stdio.h>

#include "commands.h"
#include "stack.h"

const int PrecisionConst   = 100;
#define PrecisionFormat "%.2f"

const int CommandVersion = 2;

enum spuErrors
{
    STACK_UNDERFLOW  = 1,
    INCORECT_PUSH    = 2,
    UNKNOWN_COMMAND  = 3,
    DIVISION_BY_ZERO = 4,
    FILE_ERROR       = 5,
    INCORRECT_INPUT  = 6,
    INCORREST_POP    = 7,
    BAD_SIGNATURE    = 8,
    BAD_COM_VERSION  = 9
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

int processCommand(int command, Processor *spu, FILE *fin, FILE *fout);

int checkSignature(FILE *fin);
int checkComVersion(FILE *fin);

int commandPush (Processor *spu, FILE *fin);
int commandOut  (Processor *spu, FILE *fout);
int commandAdd  (Processor *spu);
int commandSub  (Processor *spu);
int commandMul  (Processor *spu);
int commandDiv  (Processor *spu);
int commandIn   (Processor *spu);
int commandSqrt (Processor *spu);
int commandPop  (Processor *spu, FILE *fin);
int commandPushR(Processor *spu, FILE *fin);

#endif //__SPU_H__
