#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "disassembler.h"
#include "cmd_enum.h"
#include "errors.h"

static const int   NameAddSymbolsLen = 8;
static const char *NameAddSymbols = ".src.txt";

#define DEF_CMD(name, num, hasArg, function)                                            \
            case name:                                                                  \
            {                                                                           \
                myFputsCommandName(#name, fout);                                        \
                fputc(' ', fout);                                                       \
                                                                                        \
                if (hasArg)                                                             \
                {                                                                       \
                    int arg = *(bufIn + 1);                                             \
                    if (printArgument(*bufIn++, arg, fout)) return INCORRECT_ARGUMENT;  \
                }                                                                       \
                fputc('\n', fout);                                                      \
                                                                                        \
                break;                                                                  \
            }

#define DEF_CMD_JMP(name, num, hasArg, sign) DEF_CMD(name, num, hasArg, sign)

int runDisassembler(int *bufIn, FILE *fout)
{
    assert(bufIn);
    assert(fout);

    while (*bufIn != HLC)
    {   
        switch (*bufIn)
        {
            #include "commands.h"
            default:
            {
                printf("ERROR: unknown command: %d\n", *bufIn);
                return UNKNOWN_COMMAND;
            }
        }
                
        bufIn++;
    }

    fprintf(fout, "%s", "hlc");
    return EXIT_SUCCESS;
}

#undef DEF_CMD_JMP
#undef DEF_CMD

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName)
{
    assert(argV);
    assert(fileInName);
    assert(fileOutName);

    if (argC == 1) return 1;

    *fileInName  = argV[1];

    *fileOutName = (char *)calloc(strlen(argV[1]) + NameAddSymbolsLen, sizeof(char));

    char *temp = strdup(*fileInName);
    pointToZero(temp);
    sprintf(*fileOutName, "%s%s", temp, NameAddSymbols);
    free(temp);

    printf("in  = %s\n", *fileInName);
    printf("out = %s\n", *fileOutName);

    return EXIT_SUCCESS;
}

void pointToZero(char *str)
{
    assert(str);

    size_t i = 0;
    while(str[i] != '.' && str[i] != '\0') { i++; }

    str[i] = '\0';
}

int checkSignature(FILE *fin)
{
    assert(fin);

    int fileSignature = 0;
    fread((void *)&fileSignature, sizeof(fileSignature), 1, fin);

    if (fileSignature != *(const int *)Signature)  
    {
        printf("file Signature = <%4s>, my Signature = <%s>\n", (char *)&fileSignature, Signature);
        return BAD_SIGNATURE;
    }

    return EXIT_SUCCESS;
}

int checkComVersion(FILE *fin)
{
    assert(fin);

    int fileComVersion = 0;
    fread((void *)&fileComVersion, sizeof(fileComVersion), 1, fin);

    if (fileComVersion != CommandVersion) 
    {
        printf("file ComVersion = %d, my CommandVersion = %d\n", fileComVersion, CommandVersion);
        return BAD_COM_VERSION;
    }

    return EXIT_SUCCESS;
}

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin)
{
    assert(bufIn);
    assert(fin);

    if (checkSignature(fin))  return BAD_SIGNATURE;

    if (checkComVersion(fin)) return BAD_COM_VERSION;


    fread((void *)bufSize, sizeof(bufSize), 1, fin);

    *bufIn = (int *)calloc(*bufSize, sizeof(int));
    if (!*bufIn) { perror("loadProgramBin"); return MEMORY_ERROR; } 

    fread((void *)(*bufIn), sizeof(int), *bufSize, fin);

    return EXIT_SUCCESS;
}

int printArgument(int command, int argument, FILE *fout)
{
    if (command & ARG_TYPE_REGISTER)
    {
        if (argument < 0 || argument >= RegistersNumber) return INCORRECT_ARGUMENT;
        fprintf(fout, "r%cx", argument + 'a');
    }
    else if (command & ARG_TYPE_ADDRESS)
    {
        fprintf(fout, "%d", argument);
    }
    else
    {
        fprintf(fout, PrecisionFormat, (double)(argument) / PrecisionConst);
    }

    return EXIT_SUCCESS;
}

int myFputsCommandName(const char *str, FILE *f)
{
    assert(str);

    for ( ; *str != '\0' && *str != '_'; str++)
    {
        fputc(tolower(*str), f);
    }

    return EXIT_SUCCESS;
}
