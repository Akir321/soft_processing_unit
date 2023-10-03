#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "assembler.h"
#include "spu.h"

static const int   NameAddSymbolsLen = 9;
static const char *NameAddSymbols = ".byte.txt";

int runAssembler(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);

    char command[MaxCommandLength] = {};
    fscanf(fin, "%s", command);

    while (strcmp(command, "hlc"))
    {
        if (strcmp(command, "push") == 0)
        {
            float value = 0;
            if (fscanf(fin, "%f", &value) == 0) return INCORECT_PUSH;

            fprintf(fout, "%d %f\n", PUSH, value);
        }
        else if (strcmp(command, "in")   == 0)
        {
            fprintf(fout, "%d\n", IN);
        }
        else if(strcmp(command, "out")  == 0)
        {
            fprintf(fout, "%d\n", OUT);
        }
        else if(strcmp(command, "add")  == 0)
        {
            fprintf(fout, "%d\n", ADD);
        }
        else if(strcmp(command, "sub")  == 0)
        {
            fprintf(fout, "%d\n", SUB);
        }
        else if(strcmp(command, "mul")  == 0)
        {
            fprintf(fout, "%d\n", MUL);
        }
        else if(strcmp(command, "div")  == 0)
        {
            fprintf(fout, "%d\n", DIV);
        }
        else if(strcmp(command, "sqrt")  == 0)
        {
            fprintf(fout, "%d\n", SQRT);
        }
                
        if(!fscanf(fin, "%s", command)) break;
    }

    fprintf(fout, "%d", HLC);
    return EXIT_SUCCESS;
}

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName)
{
    assert(argV);
    assert(fileInName);
    assert(fileOutName);

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
