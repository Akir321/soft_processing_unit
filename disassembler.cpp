#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "disassembler.h"
#include "spu.h"

static const int   NameAddSymbolsLen = 8;
static const char *NameAddSymbols = ".src.txt";

const int nCommands = 8;

static const char *stringCommands[nCommands] = { "push",
                                                 "in",
                                                 "out",
                                                 "add",
                                                 "sub",
                                                 "mul",
                                                 "div",
                                                 "sqrt" };

int runDisassembler(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);

    int c = 0;
    fscanf(fin, "%d", &c);

    while (c != HLC)
    {
        if (c == PUSH)
        {
            float value = 0;
            if (fscanf(fin, "%f", &value) == 0) return INCORECT_PUSH;

            fprintf(fout, "%s %f\n", stringCommands[PUSH], value);
        }
        else if (c == IN)
        {
            fprintf(fout, "%s\n", stringCommands[IN]);
        }
        else if(c == OUT)
        {
            fprintf(fout, "%s\n", stringCommands[OUT]);
        }
        else if(c == ADD)
        {
            fprintf(fout, "%s\n", stringCommands[ADD]);
        }
        else if(c == SUB)
        {
            fprintf(fout, "%s\n", stringCommands[SUB]);
        }
        else if(c == MUL)
        {
            fprintf(fout, "%s\n", stringCommands[MUL]);
        }
        else if(c == DIV)
        {
            fprintf(fout, "%s\n", stringCommands[DIV]);
        }
        else if(c == SQRT)
        {
            fprintf(fout, "%s\n", stringCommands[SQRT]);
        }
                
        if(!fscanf(fin, "%d", &c)) break;
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
