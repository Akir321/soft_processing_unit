#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const int         NameAddSymbolsLen;
extern const char const *NameAddSymbols;

#if 1
int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName);
void pointToZero(char *str);



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
#endif


#if 0
int processArgv(int argC, const char *argV[], const char **fileInName, char **fileOutName)
{
    assert(argV);
    assert(fileInName);
    assert(fileOutName);

    int com = 0;
    while (strcmp(argV[com], "-o") != 0) 
    {
        com++;
        if (com == argC - 1) break;
    }

    if (strcmp(argV[com], "-o") == 0) 
    {
        if (argC <= com + 1) return 1; // it renurns only 1 for an error

        com++;
        *fileOutName = (char *)calloc(strlen(argV[com]) + NameAddSymbolsLen, sizeof(char));
        sprintf(*fileOutName, "%s%s", argV[com], NameAddSymbols);
    }

    if (!fileOutName) com = 2;
    else com++;

    *fileInName = argV[com];

    if (!fileOutName)
    {
        *fileOutName = (char *)calloc(strlen(argV[com]) + NameAddSymbolsLen, sizeof(char));
        sprintf(*fileOutName, "%s%s", argV[com], NameAddSymbols);
    }

    return EXIT_SUCCESS;
}
#endif
