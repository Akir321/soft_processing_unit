#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

enum assemErrors
{
    NO_OUT_NAME = 1
};

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(FILE *fin, FILE *fout);

void pointToZero(char *str);

#endif //__ASSEMBLER_H__
