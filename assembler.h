#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

#include "io.h"

const int CommandVersion  = 5;
const int maxValuesOnLine = 2;

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(textArray *textIn, FILE *fout, int **bufOut, const char *fileName);

void pointToZero(char *str);

int getRegisterNumber(const char *reg);

void writeToArr(int *array, size_t *position, int value);

int getArgument(const char *str, int *arg, int *argType);
int getArgumentPrintError(int argType, const char *str, const char *fileName, size_t line);

int myStrCmpNoCase(const char *s1, const char *s2);

#endif //__ASSEMBLER_H__
