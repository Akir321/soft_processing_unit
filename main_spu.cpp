#include <stdio.h>
#include "spu.h"

int main(int argc, const char *argv[])
{
    printf("#hi! this is a program that calculates different expressions\n");
    printf("#it is  a stack calculator\n");
    printf("#sooner it is going to be a primitive processor\n\n");

    printf("#to start calculating just type in the commands and values\n");
    printf("#use documentation to find out what commands do\n\n");

    const char *fileInName = "file.src.txt";

    if (argc > 1)
    {
        fileInName = argv[1];
    }

    FILE *fin  = fopen(fileInName, "rb");
    FILE *fout = stdout;

    if (!fin) 
    {
        printf("ERROR: couldn't open file %s\n", fileInName);
        return 1;
    }

    int error = runSPU(fin, fout);
    if (error)
    {
        printf("ERROR: %d\n", error);
        return error;
    }

    fclose(fin);
    fclose(fout);

    putchar('\n');
    printf("#we hope that our program helped you in your calculations\n");
    printf("#see you in the next one!\n");
}
