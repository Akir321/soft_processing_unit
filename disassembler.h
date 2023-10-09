#ifndef  __DISASSEMBLER_H__
#define  __DISASSEMBLER_H__

const int CommandVersion = 2;

enum assemErrors
{
    NO_OUT_NAME     = 1,
    INCORRECT_PUSH  = 2,
    UNKNOWN_COMMAND = 3,
    INCORRECT_POP   = 4,
    BAD_SIGNATURE   = 5,
    BAD_COM_VERSION = 6
};

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int runDisassembler(FILE *fin, FILE *fout);

void pointToZero(char *str);

int checkSignature(FILE *fin);
int checkComVersion(FILE *fin);

#endif //__DISASSEMBLER_H__
