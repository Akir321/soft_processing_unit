#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

#include "io.h"

const int CommandVersion  = 4;
const int maxValuesOnLine = 2;

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(textArray *textIn, FILE *fout, int **bufOut, const char *fileName);

void pointToZero(char *str);

int getRegisterNumber(const char *reg);

void writeToArr(int *array, size_t *position, int value);

#endif //__ASSEMBLER_H__
