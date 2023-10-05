#include <stdio.h>
#include "assembler.h"

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

    FILE *fin  = fopen (fileInName,  "r");
    if (!fin)  { perror(fileInName); return 0; }

    FILE *fout = fopen (fileOutName, "w");
    if (!fout) { perror(fileInName); return 0; }

    int error = runAssembler(fin, fout);
    if (error)
    {
        printf("ERROR: %d\n", error);
        return error;
    }
}
