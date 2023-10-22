#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

#include "io.h"

const int CommandVersion  = 5;
const int maxValuesOnLine = 2;
const int LabelsNumber    = 10;

struct Label 
{
    char *name;
    size_t address;
};

struct LabelTable
{
    Label table[LabelsNumber];
    size_t count;
};

int labelTableCtor(LabelTable *labels);
int labelTableDtor(LabelTable *labels);
int labelTableAdd (LabelTable *labels, const char *name, size_t address);
int labelTableFind(LabelTable *labels, const char *name);
int labelTableDump(LabelTable *labels);

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(textArray *textIn, LabelTable *labels, FILE *fout, int **bufOut, size_t *position,
                 const char *fileName);

int writeCommandsToFile(const int *bufOut, const size_t *position, FILE *foutbin);

void pointToZero(char *str);

int getRegisterNumber(const char *reg);

void writeToArr(int *array, size_t *position, int value);

int getArgument(int comNum, const char *str, int *arg, int *argType, LabelTable *labels);
int getArgumentPrintError(int argType, const char *str, const char *fileName, size_t line);

int myStrCmpNoCase(const char *s1, const char *s2);

#endif //__ASSEMBLER_H__
