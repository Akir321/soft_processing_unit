#include <stdio.h>
#include <stdlib.h>
#include "spu.h"

int main(int argc, const char *argv[])
{
    if (argc == 1) { printf("File name not given\n"); return 0; }
    const char *fileInName = argv[1];

    FILE *fin  = fopen(fileInName, "rb");
    FILE *fout = stdout;

    if (!fin) { printf("ERROR: couldn't open file %s\n", fileInName); return 1; }

    Processor spu = {};
    spuCtor(&spu, 4);

    int *bufIn = NULL;
    size_t bufSize = 0;
    
    int error = loadProgramBin(&bufIn, &bufSize, fin);
    if (error) { printf("ERROR: %d\n", error); return error; }

    error = runSPU(&spu, bufIn, bufSize, fout);
    if (error) { printf("ERROR: %d\n", error); return error; }

    spuDump(&spu, stdout, __FILE__, __LINE__, __func__);

    free(bufIn);
    spuDtor(&spu);
    fclose(fin);
    fclose(fout);
}
