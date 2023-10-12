#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "disassembler.h"
#include "commands.h"

static const int   NameAddSymbolsLen = 8;
static const char *NameAddSymbols = ".src.txt";

int runDisassembler(int *bufIn, FILE *fout)
{
    assert(bufIn);
    assert(fout);

    while (*bufIn != HLC)
    {   
        switch (*bufIn)
        {
            case PUSH:
            {
                bufIn++;
                fprintf(fout, "%s " PrecisionFormat "\n", "push", (double)(*bufIn) / PrecisionConst);

                break;
            }
            case PUSH_R:
            {
                bufIn++;
                int regNumber = *bufIn;

                if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_PUSH;
                fprintf(fout, "%s r%cx\n", "push", regNumber + 'a');

                break;
            }
            case IN:
            {
                fprintf(fout, "%s\n", "in");
                break;
            }
            case OUT:
            {
                fprintf(fout, "%s\n", "out");
                break;
            }
            case ADD:
            {
                fprintf(fout, "%s\n", "add");
                break;
            }
            case SUB:
            {
                fprintf(fout, "%s\n", "sub");
                break;
            }
            case MUL:
            {
                fprintf(fout, "%s\n", "mul");
                break;
            }
            case DIV:
            {
                fprintf(fout, "%s\n", "div");
                break;
            }
            case SQRT:
            {
                fprintf(fout, "%s\n", "sqrt");
                break;
            }
            case POP:
            {
                bufIn++;
                int regNumber  = *bufIn;

                if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_POP;
                fprintf(fout, "%s r%cx\n", "pop", regNumber + 'a');

                break;
            }
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
