#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "assembler.h"
#include "commands.h"
#include "io.h"

const int          NameAddSymbolsLen = 9;
const char * const NameAddSymbols = ".byte.txt";

int runAssembler(textArray *textIn, FILE *fout, int **bufOut)
{
    assert(textIn);
    assert(textIn->buffer);
    assert(textIn->strings);
    assert(fout);

    *bufOut = (int *)calloc(textIn->nLines * maxValuesOnLine, sizeof(int));

    size_t line = 0;
    size_t position = 0;
    
    char command[MaxCommandLength] = {};
    sscanf(textIn->strings[line].str, "%16s", command);

    while (strcmp(command, "hlc") != 0)
    {
        printf("%s\n", textIn->strings[line].str);
        //printf("%s\n", command);
        if (strcmp(command, "push") == 0)
        {
            float value = 0;
            char reg[MaxCommandLength] = {};

            if (sscanf(textIn->strings[line].str, "%16s %f", command, &value) == 2)
            {
                writeToArr(*bufOut, &position, PUSH);
                writeToArr(*bufOut, &position, (int)(value * PrecisionConst));
            }
            else if (sscanf(textIn->strings[line].str, "%16s %16s", command, reg) == 2)
            {
                int regNumber = getRegisterNumber(reg);
                if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_PUSH;

                writeToArr(*bufOut, &position, PUSH_R);
                writeToArr(*bufOut, &position, regNumber);
            }
            else 
            {
                return INCORRECT_PUSH;
            }
        }
        else if (strcmp(command, "in")   == 0)
        {
            writeToArr(*bufOut, &position, IN);
        }
        else if (strcmp(command, "out")  == 0)
        {
            writeToArr(*bufOut, &position, OUT);
        }
        else if (strcmp(command, "add")  == 0)
        {
            writeToArr(*bufOut, &position, ADD);
        }
        else if (strcmp(command, "sub")  == 0)
        {
            writeToArr(*bufOut, &position, SUB);
        }
        else if (strcmp(command, "mul")  == 0)
        {
            writeToArr(*bufOut, &position, MUL);
        }
        else if (strcmp(command, "div")  == 0)
        {
            writeToArr(*bufOut, &position, DIV);
        }
        else if (strcmp(command, "sqrt") == 0)
        {
            writeToArr(*bufOut, &position, SQRT);
        }
        else if (strcmp(command, "pop")  == 0)
        {
            char reg[MaxCommandLength] = {};
            if (!sscanf(textIn->strings[line].str, "%16s %16s", command, reg)) return INCORRECT_POP;

            int registerNumber = getRegisterNumber(reg);
            if (registerNumber == -1)      return INCORRECT_POP;

            writeToArr(*bufOut, &position, POP);
            writeToArr(*bufOut, &position, registerNumber);
        }
        else
        {
            printf("ERROR: line %lld: unknown command: %s\n", line, textIn->strings[line].str);
            return UNKNOWN_COMMAND;
        }
       
        sscanf(textIn->strings[++line].str, "%16s", command);
    }

    writeToArr(*bufOut, &position, HLC);

    fprintf(fout, "%d\n", *(const int *)Signature);
    fprintf(fout, "%d\n", CommandVersion);
    fprintf(fout, "%lld\n", position);

    for (size_t i = 0; i < position; i++)
    {
        fprintf(fout, "%d\n", (*bufOut)[i]);
    }

    FILE *foutbin = fopen("file.bin", "wb");

    printf("%lld\n", fwrite((const void *) Signature,      sizeof(char),    4,        foutbin));
    printf("%lld\n", fwrite((const void *)&CommandVersion, sizeof(int),     1,        foutbin));
    printf("%lld\n", fwrite((const void *)&position,       sizeof(size_t),  1,        foutbin));
    
    printf("%lld\n", fwrite((const void *)*bufOut,         sizeof(int),  position, foutbin));

    fclose(foutbin);

    return EXIT_SUCCESS;
}

int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName)
{
    assert(argV);
    assert(fileInName);
    assert(fileOutName);

    if (argC == 1) return EXIT_FAILURE;

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

int getRegisterNumber(const char *reg)
{
    assert(reg);

    if (reg[0] != 'r')  return -1;
    if (reg[2] != 'x')  return -1;
    if (reg[3] != '\0') return -1;

    int number = reg[1] - 'a';
    if (number < 0 || number >= RegistersNumber) return -1;

    return number;
}

void writeToArr(int *array, size_t *position, int value)
{
    assert(array);
    assert(position);
    
    array[(*position)++] = value;
}
