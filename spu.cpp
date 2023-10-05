#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "spu.h"
#include "stack.h"

int runSPU(FILE *fin, FILE *fout)
{
    assert(fin);
    assert(fout);
    assert(fin != fout);

    stack spuStack = {};
    stackCtor(&spuStack, DEFAULT_CAPACITY);

    int command = 0;
    fscanf(fin, "%d", &command);

    while (command != HLC)
    {
        int error = processCommand(command, &spuStack, fin, fout);
        if (error) return error;

        fscanf(fin, "%d", &command);
    }

    stackDtor(&spuStack);

    return EXIT_SUCCESS;
}

#if 1
int processCommand(int command, stack *spuStack, FILE *fin, FILE *fout)
{   
    if      (command == PUSH)
    {
        float value = 0;
        if (fscanf(fin, "%f", &value) == 0) return INCORECT_PUSH;

        stackPush(spuStack, (elem_t)(value * PrecisionConst));
    }
    else if (command == OUT)
    {
        elem_t value = 0;
    
        stackErrorField error = stackPop(spuStack, &value);
        if (error.stack_underflow) return STACK_UNDERFLOW;

        fprintf(fout, "%f\n", (float)value / (float)PrecisionConst);
    }
    else if (command == IN)
    {
        elem_t value1 = 0, value2 = 0;

        stackErrorField error1 = stackPop(spuStack, &value1);
        stackErrorField error2 = stackPop(spuStack, &value2);

        if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

        elem_t addit = value1 + value2;
        stackPush(spuStack, addit);
    }
    else if (command == SUB)
    {
        elem_t subtrahend = 0, minuend = 0;

        stackErrorField error1 = stackPop(spuStack, &subtrahend);
        stackErrorField error2 = stackPop(spuStack, &minuend);

        if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

        elem_t subt = minuend - subtrahend;
        stackPush(spuStack, subt);
    }
    else if (command == MUL)
    {
        elem_t value1 = 0, value2 = 0;

        stackErrorField error1 = stackPop(spuStack, &value1);
        stackErrorField error2 = stackPop(spuStack, &value2);

        if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

        elem_t mult = value1 * value2 / PrecisionConst;
        stackPush(spuStack, mult);
    }
    else if (command == DIV)
    {
        elem_t dividend = 0, divisor = 0;

        stackErrorField error1 = stackPop(spuStack, &divisor);
        stackErrorField error2 = stackPop(spuStack, &dividend);

        if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;
        if (divisor == 0)                                     return DIVISION_BY_ZERO;

        elem_t divis = (dividend * PrecisionConst) / divisor;
        stackPush(spuStack, divis);
    }
    else if (command == IN)
    {
        float value = 0;
        static int valueNumber = 0;

        printf("type in value (number %d) from console: ", ++valueNumber);
        scanf("%f", &value);

        stackPush(spuStack, (elem_t)(value * PrecisionConst));
    }
    else if (command == SQRT)
    {
        elem_t value = 0;

        stackErrorField error = stackPop(spuStack, &value);
        if (error.stack_underflow) return STACK_UNDERFLOW;

        float sqRoot = sqrt((float)value / PrecisionConst);
        stackPush(spuStack, (elem_t)(sqRoot * PrecisionConst));
    }
    else
    {
        fprintf(fout, "ERROR: unknown command: %d\n", command);
        return UNKNOWN_COMMAND;
    }

    STACK_DUMP(spuStack);

    return EXIT_SUCCESS;
}
#endif

#if 0
int processCommand(int command, stack *spuStack, FILE *fin, FILE *fout)
{
    switch(command)
    {
        case PUSH:
            float value = 0;
            if (fscanf(fin, "%f", &value) == 0) return INCORECT_PUSH;

            stackPush(spuStack, (elem_t)(value * PrecisionConst));
            break;

        case OUT:
            elem_t value = 0;
        
            stackErrorField error = stackPop(spuStack, &value);
            if (error.stack_underflow) return STACK_UNDERFLOW;

            fprintf(fout, "%f\n", (float)value / (float)PrecisionConst);
            break;

        case ADD:
            elem_t value1 = 0, value2 = 0;

            stackErrorField error1 = stackPop(spuStack, &value1);
            stackErrorField error2 = stackPop(spuStack, &value2);

            if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

            elem_t addit = value1 + value2;
            stackPush(spuStack, addit);
            break;

        case SUB:
            elem_t subtrahend = 0, minuend = 0;

            stackErrorField error1 = stackPop(spuStack, &subtrahend);
            stackErrorField error2 = stackPop(spuStack, &minuend);

            if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

            elem_t subt = minuend - subtrahend;
            stackPush(spuStack, subt);
            break;

        case MUL:
            elem_t value1 = 0, value2 = 0;

            stackErrorField error1 = stackPop(spuStack, &value1);
            stackErrorField error2 = stackPop(spuStack, &value2);

            if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;

            elem_t mult = value1 * value2 / PrecisionConst;
            stackPush(spuStack, mult);
            break;
            
        case DIV:
            elem_t dividend = 0, divisor = 0;

            stackErrorField error1 = stackPop(spuStack, &divisor);
            stackErrorField error2 = stackPop(spuStack, &dividend);

            if (error1.stack_underflow || error2.stack_underflow) return STACK_UNDERFLOW;
            if (divisor == 0)                                     return DIVISION_BY_ZERO;

            elem_t divis = (dividend * PrecisionConst) / divisor;
            stackPush(spuStack, divis);
            break;

        case IN:
            float value = 0;
            static int valueNumber = 0;

            printf("type in value (number %d) from console: ", ++valueNumber);
            scanf("%f", &value);

            stackPush(spuStack, (elem_t)(value * PrecisionConst));
            break;

        case SQRT:
            elem_t value = 0;

            stackErrorField error = stackPop(spuStack, &value);
            if (error.stack_underflow) return STACK_UNDERFLOW;

            float sqRoot = sqrt((float)value / PrecisionConst);
            stackPush(spuStack, (elem_t)(sqRoot * PrecisionConst));
            break;

        default:
            fprintf(fout, "ERROR: unknown command: %d\n", command);
            return UNKNOWN_COMMAND;
            break;
    }

    STACK_DUMP(spuStack);

    return EXIT_SUCCESS;
}
#endif
