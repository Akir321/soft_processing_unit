#include <stdio.h>
#include "spu.h"

int main()
{
    printf("#hi! this is a program that calculates different expressions\n");
    printf("#it is  a stack calculator\n");
    printf("#sooner it is going to be a primitive processor\n\n");

    printf("#to start calculating just type in the commands and values\n");
    printf("#use documentation to find out what commands do\n\n");

    FILE *fin  = stdin;
    FILE *fout = stdout;

    int error = runSPU(fin, fout);
    if (error)
    {
        printf("ERROR: %d\n", error);
        return error;
    }

    putchar('\n');
    printf("#we hope that our program helped you in your calculations\n");
    printf("#see you in the next one!\n");
}
