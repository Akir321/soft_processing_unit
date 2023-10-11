#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

#include "io.h"

const int CommandVersion  = 2;
const int maxValuesOnLine = 2;

enum assemErrors
{
    NO_OUT_NAME     = 1,
    INCORRECT_PUSH  = 2,
    UNKNOWN_COMMAND = 3,
    INCORRECT_POP   = 4
};

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(textArray *textIn, FILE *fout, int **bufOut);

void pointToZero(char *str);

int getRegisterNumber(const char *reg);

void writeToArr(int *array, size_t *position, int value);

#endif //__ASSEMBLER_H__
