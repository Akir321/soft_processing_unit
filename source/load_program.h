#ifndef  __LOAD_PROGRAM_H__
#define  __LOAD_PROGRAM_H__

#include <stdio.h>

int checkSignature (FILE *fin);
int checkComVersion(FILE *fin);

int loadProgramBin(int **bufIn, size_t *bufSize, FILE *fin);

#endif //__LOAD_PROGRAM_H__