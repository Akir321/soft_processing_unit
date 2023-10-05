#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "disassembler.h"
#include "spu.h"

static const int   NameAddSymbolsLen = 8;
static const char *NameAddSymbols = ".src.txt";

int runDisassembler(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);

    int command = 0;
    fscanf(fin, "%d", &command);

    while (command != HLC)
    {
        if (command == PUSH)
        {
            float value = 0;
            if (fscanf(fin, "%f", &value) == 0) return INCORECT_PUSH;

            fprintf(fout, "%s %f\n", "push", value);
        }
        else if (command == IN)
        {
            fprintf(fout, "%s\n", "in");
        }
        else if(command == OUT)
        {
            fprintf(fout, "%s\n", "out");
        }
        else if(command == ADD)
        {
            fprintf(fout, "%s\n", "add");
        }
        else if(command == SUB)
        {
            fprintf(fout, "%s\n", "sub");
        }
        else if(command == MUL)
        {
            fprintf(fout, "%s\n", "mul");
        }
        else if(command == DIV)
        {
            fprintf(fout, "%s\n", "div");
        }
        else if(command == SQRT)
        {
            fprintf(fout, "%s\n", "sqrt");
        }
        else
        {
            fprintf(fout, "ERROR: unknown command: %d\n", command);
            return UNKNOWN_COMMAND;
        }
                
        if(!fscanf(fin, "%d", &command)) break;
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
