#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "assembler.h"
#include "commands.h"

const int          NameAddSymbolsLen = 9;
const char * const NameAddSymbols = ".byte.txt";

int runAssembler(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);

    fprintf(fout, "%s\n", Signature);
    fprintf(fout, "%d\n", CommandVersion);

    char command[MaxCommandLength] = {};
    fscanf(fin, "%s", command);

    while (strcmp(command, "hlc"))
    {
        if (strcmp(command, "push") == 0)
        {
            float value = 0;
            char reg[MaxCommandLength] = {};
            if (fscanf(fin, "%f", &value))
            {
                fprintf(fout, "%d %f\n", PUSH, value);
            }
            else if (fscanf(fin, "%s", reg))
            {
                int regNumber = getRegisterNumber(reg);
                if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_PUSH;

                fprintf(fout, "%d %d\n", PUSH_R, regNumber);
            }
            else 
            {
                return INCORRECT_PUSH;
            }
        }
        else if (strcmp(command, "in")   == 0)
        {
            fprintf(fout, "%d\n", IN);
        }
        else if (strcmp(command, "out")  == 0)
        {
            fprintf(fout, "%d\n", OUT);
        }
        else if (strcmp(command, "add")  == 0)
        {
            fprintf(fout, "%d\n", ADD);
        }
        else if (strcmp(command, "sub")  == 0)
        {
            fprintf(fout, "%d\n", SUB);
        }
        else if (strcmp(command, "mul")  == 0)
        {
            fprintf(fout, "%d\n", MUL);
        }
        else if (strcmp(command, "div")  == 0)
        {
            fprintf(fout, "%d\n", DIV);
        }
        else if (strcmp(command, "sqrt") == 0)
        {
            fprintf(fout, "%d\n", SQRT);
        }
        else if (strcmp(command, "pop")  == 0)
        {
            char reg[MaxCommandLength] = {};
            if (!fscanf(fin, "%16s", reg)) return INCORRECT_POP;

            int registerNumber = getRegisterNumber(reg);
            if (registerNumber == -1)      return INCORRECT_POP;

            fprintf(fout, "%d %d\n", POP, registerNumber);
        }
        else
        {
            fprintf(fout, "ERROR: unknown command: %s\n", command);
            return UNKNOWN_COMMAND;
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
