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

int labelTableCtor(LabelTable *labels)
{
    assert(labels);

    for (size_t i = 0; i < LabelsNumber; i++)
    {
        labels->table[i].address = -1;
        labels->table[i].name    = NULL;
    }

    return EXIT_SUCCESS;
}

int labelTableDtor(LabelTable *labels)
{
    assert(labels);

    for (size_t i = 0; i < LabelsNumber; i++)
    {
        labels->table[i].address = -1;

        if (labels->table[i].name) 
        {
            free(labels->table[i].name);
            labels->table[i].name = NULL;
        }
    }

    return EXIT_SUCCESS;
}

int labelTableAdd(LabelTable *labels, const char *name, size_t address)
{
    assert(labels);
    assert(name);

    if (labels->count >= LabelsNumber) return -1;

    labels->table[labels->count].name = strdup(name);
    if (!labels->table[labels->count].name) return -1;

    labels->table[labels->count].address = address;
    labels->count++;

    return (int)(labels->count - 1);
}

int labelTableFind(LabelTable *labels, const char *name)
{
    assert(labels);
    assert(name);

    for (size_t i = 0; i < labels->count; i++)
    {
        if (strcmp(name, labels->table[i].name) == 0)
        {
            return (int)(labels->table[i].address);
        }
    }

    return -1;
}

int labelTableDump(LabelTable *labels)
{
    assert(labels);
    
    printf("I'm LabelTable\n");

    for (size_t i = 0; i < labels->count; i++)
    {
        printf("  [%lld] <%s> = %lld\n", i, labels->table[i].name, labels->table[i].address);
    }

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, hasArg, function)                                                           \
                                                                                                       \
        if (myStrCmpNoCase(command, #name) == 0)                                                       \
        {                                                                                              \
            int arg = 0, argType = 0;                                                                  \
            if (hasArg)                                                                                \
            {                                                                                          \
                int error = getArgument(num, textIn->strings[line].str, &arg, &argType, labels);               \
                if (error)                                                                             \
                {                                                                                      \
                    getArgumentPrintError(argType, textIn->strings[line].str,   fileName, line + 1);   \
                    return INCORRECT_ARGUMENT;                                                         \
                }                                                                                      \
            }                                                                                          \
                                                                                                       \
            writeToArr(*bufOut, position, num | argType);                                             \
            if (hasArg) writeToArr(*bufOut, position, arg);                                           \
        }                                                                                              \
        else   

#define DEF_CMD_JMP(name, num, hasArg, sign) DEF_CMD(name, num, hasArg, sign)                                          

int runAssembler(textArray *textIn, LabelTable *labels, FILE *foutbin, int **bufOut, size_t *position,
                const char *fileName)
{
    assert(textIn);
    assert(textIn->buffer);
    assert(textIn->strings);
    assert(foutbin);

    *bufOut = (int *)calloc(textIn->nLines * maxValuesOnLine, sizeof(int));

    size_t line = 0;
    
    char command[MaxCommandLength] = {};
    sscanf(textIn->strings[line].str, "%16s", command);

    while (line < textIn->nLines)
    {
        printf("%s\n", textIn->strings[line].str);
        //printf("%s\n", command);
       
       if (command[0] == ':')
       {
            if (labelTableFind(labels, command + 1) == -1)
            {
                if (labelTableAdd(labels,  command + 1, *position) == -1) return LABEL_ADD_ERROR;
            }
       }
       else
       #include "commands.h"
        { //else
            printf("%s(%lld): error: unknown command: %s\n", fileName, line + 1, command);
            printf("|    <%s>\n", textIn->strings[line].str);
            return UNKNOWN_COMMAND;
        }
       
        sscanf(textIn->strings[++line].str, "%16s", command);
    }

    writeToArr(*bufOut, position, HLT);

    return EXIT_SUCCESS;
}

#undef DEF_CMD_JMP
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

int getArgument(int comNum, const char *str, int *arg, int *argType, LabelTable *labels)
{
    assert(str);
    assert(arg);
    assert(argType);

    char command[MaxCommandLength] = {};

    float fArgument                   =  0;
    int   dArgument                   =  0;
    char  sArgument[MaxCommandLength] = {};

    if (comNum & ARG_TYPE_ADDRESS)
    {
        if (sscanf(str, "%s %d", command, &dArgument) == 2)
        {
            *argType = ARG_TYPE_ADDRESS;
            *arg     = dArgument;
        }
        else if (sscanf(str, "%s :%s", command, sArgument) == 2)
        {
            *argType = ARG_TYPE_ADDRESS;
            *arg     = labelTableFind(labels, sArgument);
        }
        else return EXIT_FAILURE;
    }
    else if (sscanf(str, "%s %f", command, &fArgument) == 2)
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

int writeCommandsToFile(const int *bufOut, const size_t *position, FILE *foutbin)
{
    assert(bufOut);
    assert(position);
    assert(foutbin);
    
    /*
    fprintf(fout, "%d\n", *(const int *)Signature);
    fprintf(fout, "%d\n", CommandVersion);
    fprintf(fout, "%lld\n", position);

    for (size_t i = 0; i < position; i++)
    {
        fprintf(fout, "%d\n", (*bufOut)[i]);
    }
    */

    printf("%lld\n", fwrite((const void *) Signature,      sizeof(char),    4,        foutbin));
    printf("%lld\n", fwrite((const void *)&CommandVersion, sizeof(int),     1,        foutbin));
    printf("%lld\n", fwrite((const void *) position,       sizeof(size_t),  1,        foutbin));
    
    printf("%lld\n", fwrite((const void *) bufOut,         sizeof(int),     *position, foutbin));

    return EXIT_SUCCESS;
}

int myStrCmpNoCase(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    for ( ; tolower(*s1) == tolower(*s2) && *s1 != '\0'; s1++, s2++) {}
    
    return tolower(*s1) - tolower(*s2);
}
