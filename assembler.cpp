#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "assembler.h"
#include "cmd_enum.h"
#include "errors.h"
#include "io.h"

const int          NameAddSymbolsLen = 4;
const char * const NameAddSymbols = ".bin";

int runAssembler(textArray *textIn, FILE *foutbin, int **bufOut, const char *fileName)
{
    assert(textIn);
    assert(textIn->buffer);
    assert(textIn->strings);
    assert(foutbin);

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
            int arg = 0, argType = 0;
            int error = getArgument(textIn->strings[line].str, &arg, &argType);
            if (error) 
            {
                getArgumentPrintError(argType, textIn->strings[line].str,   fileName, line + 1);
                return INCORRECT_PUSH;
            }

            writeToArr(*bufOut, &position, PUSH | argType);
            writeToArr(*bufOut, &position, arg);
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
        else if (strcmp(command, "sin") == 0)
        {
            writeToArr(*bufOut, &position, SIN);
        }
        else if (strcmp(command, "cos") == 0)
        {
            writeToArr(*bufOut, &position, COS);
        }
        else if (strcmp(command, "tan") == 0)
        {
            writeToArr(*bufOut, &position, TAN);
        }
        else if (strcmp(command, "cot") == 0)
        {
            writeToArr(*bufOut, &position, COT);
        }
        else if (strcmp(command, "meow") == 0)
        {
            writeToArr(*bufOut, &position, MEOW);
        }
        else if (strcmp(command, "wmeow") == 0)
        {
            writeToArr(*bufOut, &position, WMEOW);
        }
        else if (strcmp(command, "pop")  == 0)
        {
            int arg = 0, argType = 0;
            int error = getArgument(textIn->strings[line].str, &arg, &argType);
            if (error)
            {
                getArgumentPrintError(argType, textIn->strings[line].str, fileName, line + 1);
                return INCORRECT_POP;
            }

            writeToArr(*bufOut, &position, POP);
            writeToArr(*bufOut, &position, arg);
        }
        else
        {
            printf("%s(%lld): error: unknown command: %s\n", fileName, line + 1, command);
            printf("|    <%s>\n", textIn->strings[line].str);
            return UNKNOWN_COMMAND;
        }
       
        sscanf(textIn->strings[++line].str, "%16s", command);
    }

    writeToArr(*bufOut, &position, HLC);

    /*
    fprintf(fout, "%d\n", *(const int *)Signature);
    fprintf(fout, "%d\n", CommandVersion);
    fprintf(fout, "%lld\n", position);

    for (size_t i = 0; i < position; i++)
    {
        fprintf(fout, "%d\n", (*bufOut)[i]);
    }

    FILE *foutbin = fopen("file.bin", "wb");
    */

    printf("%lld\n", fwrite((const void *) Signature,      sizeof(char),    4,        foutbin));
    printf("%lld\n", fwrite((const void *)&CommandVersion, sizeof(int),     1,        foutbin));
    printf("%lld\n", fwrite((const void *)&position,       sizeof(size_t),  1,        foutbin));
    
    printf("%lld\n", fwrite((const void *)*bufOut,         sizeof(int),     position, foutbin));

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

int getArgument(const char *str, int *arg, int *argType)
{
    assert(str);
    assert(arg);
    assert(argType);

    char command[MaxCommandLength] = {};

    float fArgument                   =  0;
    char  sArgument[MaxCommandLength] = {};

    if (sscanf(str, "%s %f", command, &fArgument) == 2)
    {
        *argType = ARG_TYPE_NUMBER;
        if (strcmp(command, "push") != 0) return EXIT_FAILURE;

        *arg = (int)(fArgument * PrecisionConst);
    }
    else if (sscanf(str, "%s %s", command, sArgument) == 2)
    {
        int regNumber = getRegisterNumber(sArgument);
        if (regNumber == -1) 
        {
            *argType = ARG_TYPE_STRING;
            return EXIT_FAILURE;
        }

        *argType = ARG_TYPE_REGISTER;
        *arg = regNumber;
    }
    else
    {
        *argType = ARG_TYPE_NOTHING;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int getArgumentPrintError(int argType, const char *str, const char *fileName, size_t line)
{
    assert(str);
    assert(fileName);

    char command[MaxCommandLength] = {};
    char arg[MaxCommandLength]     = {};
    sscanf(str, "%s %s", command, arg);

    switch(argType)
    {
        case ARG_TYPE_NOTHING:
            printf("%s(%lld): error: no argument for %s given\n", fileName, line, command);
            printf("|    <%s>\n", str);
            break;
        case ARG_TYPE_STRING:
            printf("%s(%lld): error: incorrect register name: %s\n", fileName, line, arg);
            printf("|    <%s>\n", str);
            break;
        case ARG_TYPE_NUMBER:
            printf("%s(%lld): error: incorrect argument for %s given\n", fileName, line, command);
            printf("|    <%s>\n", str);
            break;
        default:
            printf("%d - no case for this error in getArgumentPrintError()\n", argType);
            break;
    }

    return EXIT_SUCCESS;
}
