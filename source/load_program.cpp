#include <stdio.h>
#include <assert.h>

#include "errors.h"
#include "load_program.h"

extern const int          CommandVersion;
extern const char * const Signature;

int checkSignature(FILE *fin)
{
    assert(fin);

    int fileSignature = 0;
    fread((void *)&fileSignature, sizeof(fileSignature), 1, fin);

    if (fileSignature != *(const int *)Signature)  
    {
        printf("file Signature = <%4s>, my Signature = <%s>\n", (char *)&fileSignature, Signature);
        return BAD_SIGNATURE;
    }

    return EXIT_SUCCESS;
}

int checkComVersion(FILE *fin)
{
    assert(fin);

    int fileComVersion = 0;
    fread((void *)&fileComVersion, sizeof(fileComVersion), 1, fin);

    if (fileComVersion != CommandVersion) 
    {
        printf("file ComVersion = %d, my CommandVersion = %d\n", fileComVersion, CommandVersion);
        return BAD_COM_VERSION;
    }

    return EXIT_SUCCESS;
}

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin)
{
    assert(bufIn);
    assert(fin);

    if (checkSignature(fin))  return BAD_SIGNATURE;

    if (checkComVersion(fin)) return BAD_COM_VERSION;


    fread((void *)bufSize, sizeof(bufSize), 1, fin);

    *bufIn = (int *)calloc(*bufSize, sizeof(int));
    if (!*bufIn) { perror("loadProgramBin"); return MEMORY_ERROR; } 

    fread((void *)(*bufIn), sizeof(int), *bufSize, fin);

    return EXIT_SUCCESS;
}
