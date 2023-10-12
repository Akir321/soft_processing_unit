#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "spu.h"
#include "commands.h"
#include "stack.h"

int spuCtor(Processor *spu, size_t stackCapacity)
{
    assert(spu);

    stackCtor(&spu->stk, stackCapacity);

    for (int i = 0; i < RegistersNumber; i++)
    {
        spu->registers[i] = 0;
    }

    return EXIT_SUCCESS;
}

int spuDtor(Processor *spu)
{
    assert(spu);

    stackDtor(&spu->stk);

    for (int i = 0; i < RegistersNumber; i++)
    {
        spu->registers[i] = 0;
    }

    return EXIT_SUCCESS;
}

int spuError(Processor *spu)
{
    assert(spu);

    int stackErr = errorFieldToU(stackError(&spu->stk));
    if (stackErr) return stackErr;
    
    return EXIT_SUCCESS;
}

int spuDump(Processor *spu, const char *file, int line, const char *function)
{
    assert(spu);

    printf("I'm spuDump called from %s (%d) %s\n", function, line, file);
    printf("Processor's stack:\n");
    stackDump(&spu->stk, file, line, function);

    printf("Register's:\n");
    for (int i = 0; i < RegistersNumber; i++)
    {
        printf("  r%cx = %d\n", i + 'a', spu->registers[i]);
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

int runSPU(Processor *spu, int *bufIn, FILE *fout)
{
    assert(spu);
    assert(bufIn);
    assert(fout);

    while (*bufIn != HLC)
    {
        int error = processCommand(spu, &bufIn, fout);
        if (error) return error;

        bufIn++;
    }

    return EXIT_SUCCESS;
}

int processCommand(Processor *spu, int **bufIn, FILE *fout)
{
    assert(spu);
    assert(fout);
    assert(bufIn);
    assert(*bufIn);

    switch(**bufIn)
    {
        case PUSH:
        {
            int error = commandPush(spu, bufIn);
            if (error) return error;
            break;
        }
        case PUSH_R:
        {
            int error = commandPushR(spu, bufIn);
            if (error) return error;
            break;
        }
        case OUT:
        {
            int error = commandOut(spu, fout);
            if (error) return error;
            break;
        }
        case ADD:
        {
            int error = commandAdd(spu);
            if (error) return error;
            break;
        }
        case SUB:
        {
            int error = commandSub(spu);
            if (error) return error;
            break;
        }
        case MUL:
        {
            int error = commandMul(spu);
            if (error) return error;
            break;
        }  
        case DIV:
        {
            int error = commandDiv(spu);
            if (error) return error;
            break;
        }
        case IN:
        {
            int error = commandIn(spu);
            if (error) return error;
            break;
        }
        case SQRT:
        {
            int error = commandSqrt(spu);
            if (error) return error;
            break;
        }
        case POP:
        {
            int error = commandPop(spu, bufIn);
            if (error) return error;
            break;
        }
        default:
        {
            fprintf(fout, "ERROR: unknown command: %d\n", **bufIn);
            return UNKNOWN_COMMAND;
            break;
        }
    }

    STACK_DUMP(&spu->stk);

    return EXIT_SUCCESS;
}

int commandPush(Processor *spu, int **bufIn)
{
    assert(spu);

    (*bufIn)++;
    stackPush(&spu->stk, (elem_t)(**bufIn));
    
    return EXIT_SUCCESS;
}

int commandOut(Processor *spu, FILE *fout)
{
    assert(spu);

    elem_t value = 0;
        
    stackErrorField error = stackPop(&spu->stk, &value);
    if (error.stack_underflow) return STACK_UNDERFLOW;

    fprintf(fout, PrecisionFormat "\n", (float)value / (float)PrecisionConst);

    return EXIT_SUCCESS;
}

int commandAdd(Processor *spu)
{
    assert(spu);

    elem_t value1 = 0, value2 = 0;

    stackErrorField error1 = stackPop(&spu->stk, &value1);
    stackErrorField error2 = stackPop(&spu->stk, &value2);

    if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

    elem_t addit = value1 + value2;
    stackPush(&spu->stk, addit);

    return EXIT_SUCCESS;
}

int commandSub(Processor *spu)
{
    assert(spu);

    elem_t subtrahend = 0, minuend = 0;

    stackErrorField error1 = stackPop(&spu->stk, &subtrahend);
    stackErrorField error2 = stackPop(&spu->stk, &minuend);

    if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

    elem_t subt = minuend - subtrahend;
    stackPush(&spu->stk, subt);

    return EXIT_SUCCESS;
}

int commandMul(Processor *spu)
{
    assert(spu);

    elem_t value1 = 0, value2 = 0;

    stackErrorField error1 = stackPop(&spu->stk, &value1);
    stackErrorField error2 = stackPop(&spu->stk, &value2);

    if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

    elem_t mult = value1 * value2 / PrecisionConst;
    stackPush(&spu->stk, mult);

    return EXIT_SUCCESS;
}

int commandDiv(Processor *spu)
{
    assert(spu);

    elem_t dividend = 0, divisor = 0;

    stackErrorField error1 = stackPop(&spu->stk, &divisor);
    stackErrorField error2 = stackPop(&spu->stk, &dividend);

    if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;
    if (divisor == 0)                                     return DIVISION_BY_ZERO;

    elem_t divis = (dividend * PrecisionConst) / divisor;
    stackPush(&spu->stk, divis);
    
    return EXIT_SUCCESS;
}

int commandIn(Processor *spu)
{
    assert(spu);

    float value = 0;
    static int valueNumber = 0;

    printf("type in value (number %d) from console: ", ++valueNumber);
    if (!scanf("%f", &value)) return INCORRECT_INPUT;

    stackPush(&spu->stk, (elem_t)(value * PrecisionConst));
    
    return EXIT_SUCCESS;
}

int commandSqrt(Processor *spu)
{
    assert(spu);

    elem_t value = 0;

    stackErrorField error = stackPop(&spu->stk, &value);
    if (error.stack_underflow) return STACK_UNDERFLOW;

    float sqRoot = sqrt((float)value / PrecisionConst);
    stackPush(&spu->stk, (elem_t)(sqRoot * PrecisionConst));
    
    return EXIT_SUCCESS;
}

int commandPop(Processor *spu, int **bufIn)
{
    assert(spu);

    elem_t value = 0;

    stackErrorField error = stackPop(&spu->stk, &value);
    if (error.stack_underflow) return STACK_UNDERFLOW;

    (*bufIn)++;
    int regNumber = **bufIn;

    if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_POP;
    spu->registers[regNumber] = value;

    return EXIT_SUCCESS;
}

int commandPushR(Processor *spu, int **bufIn)
{
    assert(spu);

    (*bufIn)++;
    int regNumber = **bufIn;

    if (regNumber < 0 || regNumber >= RegistersNumber) return INCORRECT_PUSH;
    stackPush(&spu->stk, spu->registers[regNumber]); 

    return EXIT_SUCCESS;
}

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
