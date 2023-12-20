#ifndef  __SPU_H__
#define  __SPU_H__

#include <stdio.h>

#include "cmd_enum.h"
#include "stack.h"

const int    CommandVersion = 5;
const double ComparisonPrecision = 0.000001;

struct Processor
{
    stack stk;
    stack addresses;
    int registers[RegistersNumber];
};

int spuCtor (Processor *spu, size_t stackCapacity);
int spuDtor (Processor *spu);
int spuError(Processor *spu);
int spuDump (Processor *spu, FILE *f, const char *file, int line, const char *function);

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin);

int runSPU(Processor *spu, int *bufIn, size_t bufSize, FILE *fout);

int processCommand(Processor *spu, int *bufIn, size_t *instructionPointer, FILE *fout);

int checkSignature(FILE *fin);
int checkComVersion(FILE *fin);

int commandPush    (Processor *spu, int *bufIn, size_t *instructionPointer);
int commandOut     (Processor *spu, FILE *fout);
int commandArithm  (Processor *spu, int command);
int commandTrigonom(Processor *spu, int command);
int commandIn      (Processor *spu);
int commandSqrt    (Processor *spu);
int commandPop     (Processor *spu, int *bufIn, size_t *instructionPointer);
int commandPushR   (Processor *spu, int *bufIn, size_t *instructionPointer);
int commandCall    (Processor *spu, int *bufIn, size_t *instructionPointer);
int commandRet(Processor *spu, int *bufIn, size_t *instructionPointer);

int commandJmp(int *bufIn, size_t *instructionPointer);

int commandMeow     (FILE *fout);
int commandWeirdMeow(FILE *fout);

bool isZero(float a);

#endif //__SPU_H__
