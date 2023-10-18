#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "assembler.h"
#include "cmd_enum.h"
#include "errors.h"
#include "io.h"

const int          NameAddSymbolsLen = 4;
const char * const NameAddSymbols = ".bin";

#define DEF_CMD(name, num, hasArg, function)                                                                     \
                                                                                                       \
        if (myStrCmpNoCase(command, #name) == 0)                                                               \
        {                                                                                              \
            int arg = 0, argType = 0;                                                                  \
            if (hasArg)                                                                                \
            {                                                                                          \
                int error = getArgument(textIn->strings[line].str, &arg, &argType);                    \
                if (error)                                                                             \
                {                                                                                      \
                    getArgumentPrintError(argType, textIn->strings[line].str,   fileName, line + 1);   \
                    return INCORRECT_ARGUMENT;                                                         \
                }                                                                                      \
            }                                                                                          \
                                                                                                       \
            writeToArr(*bufOut, &position, num | argType);                                             \
            if (hasArg) writeToArr(*bufOut, &position, arg);                                           \
        }                                                                                              \
        else                                             

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
       
       #include "commands.h"
        { //else
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

#undef DEF_CMD

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

int myStrCmpNoCase(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    for ( ; tolower(*s1) == tolower(*s2) && *s1 != '\0'; s1++, s2++) {}
    
    return tolower(*s1) - tolower(*s2);
}
