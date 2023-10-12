#ifndef  __SPU_H__
#define  __SPU_H__

#include <stdio.h>

#include "commands.h"
#include "stack.h"

const int CommandVersion = 3;

enum spuErrors
{
    STACK_UNDERFLOW   = 1,
    INCORRECT_PUSH    = 2,
    UNKNOWN_COMMAND   = 3,
    DIVISION_BY_ZERO  = 4,
    FILE_ERROR        = 5,
    INCORRECT_INPUT   = 6,
    INCORRECT_POP     = 7,
    BAD_SIGNATURE     = 8,
    BAD_COM_VERSION   = 9,
    MEMORY_ERROR      = 10
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

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin);

int runSPU(Processor *spu, int *bufIn, FILE *fout);

int processCommand(Processor *spu, int **bufIn, FILE *fout);

int checkSignature(FILE *fin);
int checkComVersion(FILE *fin);

int commandPush (Processor *spu, int **bufIn);
int commandOut  (Processor *spu, FILE *fout);
int commandAdd  (Processor *spu);
int commandSub  (Processor *spu);
int commandMul  (Processor *spu);
int commandDiv  (Processor *spu);
int commandIn   (Processor *spu);
int commandSqrt (Processor *spu);
int commandPop(Processor *spu, int **bufIn);
int commandPushR(Processor *spu, int **bufIn);

#endif //__SPU_H__
