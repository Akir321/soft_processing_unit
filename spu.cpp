#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "spu.h"
#include "stack.h"

int runSPU(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);
    assert(fin != fout);

    stack spuStack = {};
    stackCtor(&spuStack, DEFAULT_CAPACITY);

    char command[MaxCommandLength] = {};
    fscanf(fin, "%s", command);

    while (strcmp(command, "hlc") != 0)
    {
        int error = processCommand(command, &spuStack, fin, fout);
        if (error) return error;

        fscanf(fin, "%s", command);
    }

    return EXIT_SUCCESS;
}

int processCommand(char *command, stack *spuStack, FILE *fin, FILE *fout)
{
    assert(command);

    if      (strcmp(command, "push") == 0)
    {
        elem_t value = 0;
        if (fscanf(fin, elemFormat, &value) == 0) return EOF;

        stackPush(spuStack, value);
    }
    else if (strcmp(command, "out") == 0)
    {
        elem_t value = 0;
    
        stackErrorField error = stackPop(spuStack, &value);
        if (error.stack_underflow) return STACK_UNDERFLOW;

        fprintf(fout, elemFormat "\n", value);
        fflush(fout);
    }
    else if (strcmp(command, "add") == 0)
    {
        elem_t value1 = 0, value2 = 0;

        stackErrorField error1 = stackPop(spuStack, &value1);
        stackErrorField error2 = stackPop(spuStack, &value2);

        if (error1.stack_underflow or error2.stack_underflow) return STACK_UNDERFLOW;

        stackPush(spuStack, value1 + value2);
    }
    else
    {
        fprintf(fout, "ERROR: unknown command: %s\n", command);
        return UNKNOWN_COMMAND;
    }

    STACK_DUMP(spuStack);

    return EXIT_SUCCESS;
}
