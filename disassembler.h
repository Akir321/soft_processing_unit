#ifndef  __DISASSEMBLER_H__
#define  __DISASSEMBLER_H__

enum assemErrors
{
    NO_OUT_NAME = 1
};

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runDisassembler(FILE *fin, FILE *fout);

void pointToZero(char *str);

#endif //__DISASSEMBLER_H__
