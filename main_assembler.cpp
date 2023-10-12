#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
#include "io.h"

int main(int argc, const char *argv[])
{
    const char *fileInName  = NULL;
    char       *fileOutName = NULL;

    if (argc == 1)
    {
        printf("ERROR: file name not given\n");
        return 0;
    }

    processArgv(argc, argv, &fileInName, &fileOutName);

    textArray textIn = {};
    if (readTextFromFile(fileInName, &textIn)) { perror(fileInName); return 0; }

    FILE *fout = fopen (fileOutName, "w");
    if (!fout) { perror(fileInName); return 0; }

    int *bufOut = NULL;

    int error = runAssembler(&textIn, fout, &bufOut, fileInName);
    if (bufOut) free(bufOut); 

    destroyTextArray(&textIn);
    fclose(fout);

    if (error) { printf("ERROR: %d\n", error); return error; }
}
