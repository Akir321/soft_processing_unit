#include <assert.h>
#include <stdio.h>

#include "stack.h"
#include "html_logfile.h"

#define STACK_VERIFY \
if (errorFieldToU(stackError(stk))) \
{ \
    printf("STACK CORRUPTED\n"); \
    STACK_DUMP(stk); \
    return stackError(stk); \
}

stackErrorField stackError(stack *stk)
{
    stackErrorField error = {};

    if (!stk)                                    { error.stack_null     = 1;  return error; }
    if (!stk->data)                                error.data_null      = 1;
    if (stk->capacity < stk->size)                 error.small_capacity = 1;

    CANARY_PROTECTION (
        if (stk->stackCanary1 != STK_CANARY)       error.changed_canary = 1;
        if (stk->stackCanary2 != STK_CANARY)       error.changed_canary = 1;
    )

    HASH_PROTECTION (
        if (stackHashCheck(stk).changed_hash) error.changed_hash   = 1;
    )

    if (error.data_null == 1)                      return error;

    CANARY_PROTECTION (
        unsigned long long buf_canary = *((unsigned long long *)stk->data - 1);
        if (buf_canary != BUF_CANARY)              error.changed_canary = 1;
        buf_canary = *(unsigned long long *)(stk->data + stk->capacity);
        if (buf_canary != BUF_CANARY)              error.changed_canary = 1;
    )

    return error;
}

unsigned errorFieldToU(stackErrorField error)
{
    return *(unsigned *)&error;
}

stackErrorField stackCtor(stack *stk, size_t capacity)
{
    assert(stk);
    stackErrorField error = {};

    stk->capacity = (capacity > 0) ? capacity : DEFAULT_CAPACITY;
    stk->size = 0;

    CANARY_PROTECTION (
        stk->stackCanary1 = STK_CANARY;
        stk->stackCanary2 = STK_CANARY;
    )

    stk->data = (elem_t *)myCalloc(stk->capacity, sizeof(elem_t));
    //printf("data[%p], capacity = %lld\n", stk->data, stk->capacity);
    if (!stk->data) 
    {
        error.data_null = 1;
        return error;
    }

    CANARY_PROTECTION (
        *(unsigned long long *) stk->data                  = BUF_CANARY;
        stk->data = (elem_t *)((unsigned long long *)stk->data + 1);
        *(unsigned long long *)(stk->data + stk->capacity) = BUF_CANARY;
    )

    HASH_PROTECTION (
        stk->hash = stackHashCalc(stk);
        LOG("stackHashCalc = %u\n", stackHashCalc(stk));
        LOG("stkHash = %u\n", stk->hash);
    )

    return error;
}

stackErrorField stackDtor(stack *stk)
{
    assert(stk);
    stackErrorField error = {};
    
    CANARY_PROTECTION (
        stk->data = (elem_t *)((unsigned long long *)stk->data - 1);
    )
    
    free(stk->data);
    stk->data = NULL;
    
    stk->capacity = 0;
    stk->size = -1;

    CANARY_PROTECTION (
        stk->stackCanary1 = 0;
        stk->stackCanary2 = 0;
    )

    HASH_PROTECTION (
        stk->hash = 0;
    )

    return error;
}

stackErrorField stackDump(stack *stk, const char *file, int line, const char *function)
{
    assert(stk);
    LOG("I'm stackDump called from %s (%d) %s\n", function, line, file);
    stackErrorField error = stackError(stk);
    printStackErrors(error);

    LOG(" capacity = %lld\n size = %lld\n", stk->capacity, stk->size);

    HASH_PROTECTION (
        LOG(" hash = %u\n", stk->hash);
    )

    CANARY_PROTECTION (
        LOG("stackCanary1  = 0x%llx\n", stk->stackCanary1);
        LOG("stackCanary2  = 0x%llx\n", stk->stackCanary2);
    )

    LOG(" data[%p] = ", stk->data);
    if (!stk->data)
    {
        LOG("NULL\n");
        return error;
    }
    putc('\n', LogFile);

    for (size_t i = 0; 
         i < stk->size + ELEM_PRINT_ADD 
         && i < stk->capacity
         && i < LAST_PRINTED; i++)
    {
        LOG("   ");
        if (i < stk->size) putc('*', LogFile);
        else               putc(' ', LogFile);
        LOG("[%lld] = " elemFormat "\n", i, stk->data[i]);
    }

    CANARY_PROTECTION (
        unsigned long long buf_canary = *((unsigned long long *)stk->data - 1);
        LOG("bufferCanary1 = 0x%llx\n", buf_canary);
        buf_canary = *(unsigned long long *)(stk->data + stk->capacity);
        LOG("bufferCanary2 = 0x%llx\n", buf_canary);
    )

    return error;
}

void printStackErrors(stackErrorField error)
{
    if (error.stack_null)       LOG("ERROR: stack_null     = 1\n");
    if (error.data_null)        LOG("ERROR: data_null      = 1\n");
    if (error.small_capacity)   LOG("ERROR: small_capacity = 1\n");
    if (error.anti_overflow)    LOG("ERROR: anti_overflow  = 1\n");
    if (error.realloc_failed)   LOG("ERROR: realloc_failed = 1\n");

    CANARY_PROTECTION (
        if (error.changed_canary)   LOG("changed_canary = 1\n");
    )

    HASH_PROTECTION (
        if (error.changed_hash)     LOG("changed_hash   = 1\n");
    )
}

stackErrorField stackPush(stack *stk, elem_t value)
{
    STACK_VERIFY;
    stackErrorField error = {};

    if (stk->size >= stk->capacity) error = stackRealloc(stk);
    if (error.realloc_failed) return error;

    stk->data[stk->size++] = value;

    HASH_PROTECTION (
        stk->hash = stackHashCalc(stk);
    )

    return error;
}

stackErrorField stackPop(stack *stk, elem_t *returnValue)
{
    STACK_VERIFY;
    assert(returnValue);

    stackErrorField error = {};

    if (!stk->size) 
    {
        error.anti_overflow = 1;
        return error;
    }
    if (stk->size * REALLOC_RATE * REALLOC_RATE <= stk->capacity) error =  stackRealloc(stk);
    if (error.realloc_failed) return error;

    *returnValue = stk->data[--stk->size];
    //stk->data[stk->size] = 0;

    HASH_PROTECTION (
        stk->hash = stackHashCalc(stk);
    )

    return error;
}

stackErrorField stackRealloc(stack *stk)
{
    STACK_VERIFY;

    LOG("i'm stackRealloc\n");
    LOG("capacity = %lld\nsize = %lld\n", stk->capacity, stk->size);
    LOG("start reallocing\n");

    stackErrorField error = {};

    CANARY_PROTECTION (
        stk->data = (elem_t *)((long long *)stk->data - 1);
        size_t prevCapacity = stk->capacity;
    )

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= REALLOC_RATE;
        stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t) + 2 * sizeof(long long));
    }
    else if (stk->size * REALLOC_RATE  * REALLOC_RATE <= stk->capacity)
    {
        stk->capacity /= REALLOC_RATE;
        stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t) + 2 * sizeof(long long));
    }
    if (!stk->data) 
    {
        error.realloc_failed = 1;
        return error;
    }

    CANARY_PROTECTION (
        stk->data = (elem_t *)((long long *)stk->data + 1);
        *(unsigned long long *)(stk->data + prevCapacity)  = 0;
        *(unsigned long long *)(stk->data + stk->capacity) = BUF_CANARY;
    )

    LOG("realloc finished\n");
    LOG("new capacity = %lld\nnew size = %lld\n", stk->capacity, stk->size);
    return error;
}

void *myCalloc(size_t elementNum, size_t elementSize)
{
    size_t bufSize = elementSize * elementNum;
    
    CANARY_PROTECTION(
        bufSize += 2 * sizeof(long long);
    )

    void *buffer = malloc(bufSize);
    if (!buffer) return NULL;

    char *first = (char *)buffer;
    char *last  = (char *)buffer + bufSize;
    
    for ( ; first != last; first++)
    {
        *first = 0;
    }
    
    return buffer;
}

HASH_PROTECTION (
unsigned int stackHashCalc(stack *stk)
{
    assert(stk);

    unsigned int tempHash = stk->hash;
    stk->hash = 0;

    unsigned int hash = 0;
    for (size_t i = 0; i < sizeof(stack); i++)
    {
        hash = (hash * HASH_BASE + *((unsigned char *)stk + i)) % HASH_MOD;
    }

    if (!stk->data) return hash;

    size_t bufSize = stk->capacity * sizeof(elem_t);
    for (size_t i = 0; i < bufSize; i++)
    {
        hash = (hash * HASH_BASE + *((unsigned char *)stk->data + i)) % HASH_MOD;
    }

    stk->hash = tempHash;
    return hash;
}

stackErrorField stackHashCheck(stack *stk)
{
    assert(stk);
    stackErrorField error = {};

    unsigned int hashValue = stackHashCalc(stk);
    if (hashValue != stk->hash) error.changed_hash = 1;

    return error;
}
)
