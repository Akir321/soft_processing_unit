#include <stdio.h>
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

    if (checkSignature (fin)) return BAD_SIGNATURE;
    if (checkComVersion(fin)) return BAD_COM_VERSION;

    int error = runSPU(&spu, fin, fout);
    if (error) { printf("ERROR: %d\n", error); return error; }

    spuDump(&spu, __FILE__, __LINE__, __func__);

    spuDtor(&spu);
    fclose(fin);
    fclose(fout);
}
