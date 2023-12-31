#ifndef  __DISASSEMBLER_H__
#define  __DISASSEMBLER_H__

const int CommandVersion = 5;

/*enum assemErrors
{
    NO_OUT_NAME     = 1,
    INCORRECT_PUSH  = 2,
    UNKNOWN_COMMAND = 3,
    INCORRECT_POP   = 4,
    BAD_SIGNATURE   = 5,
    BAD_COM_VERSION = 6,
    MEMORY_ERROR    = 7
};*/

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin);

int runDisassembler(int *bufIn, FILE *fout);

void pointToZero(char *str);

int checkSignature(FILE *fin);
int checkComVersion(FILE *fin);

int printArgument(int command, int argument, FILE *fout);

int myFputsCommandName(const char *str, FILE *f);

#endif //__DISASSEMBLER_H__
