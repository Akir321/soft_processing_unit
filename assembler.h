#ifndef  __ASSEMBLER_H__
#define  __ASSEMBLER_H__

enum assemErrors
{
    NO_OUT_NAME     = 1,
    INCORRECT_PUSH  = 2,
    UNKNOWN_COMMAND = 3,
    INCORRECT_POP   = 4
};

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runAssembler(FILE *fin, FILE *fout);

void pointToZero(char *str);

int getRegisterNumber(const char *reg);

#endif //__ASSEMBLER_H__
