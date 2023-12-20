#ifndef STACK_H
#define STACK_H

typedef int elem_t;
#define elemFormat "%d"

#undef  CANARY_PROTECTION
#undef    HASH_PROTECTION

#define CANARY_PROTECTION(...) //__VA_ARGS__
#define   HASH_PROTECTION(...) //__VA_ARGS__

const size_t DEFAULT_CAPACITY = 8;
const size_t LAST_PRINTED     = 16;
const size_t ELEM_PRINT_ADD   = 4;
const size_t REALLOC_RATE     = 2;

const unsigned int HASH_BASE  = 256;
const unsigned int HASH_MOD   = 11113;

const unsigned long long STK_CANARY = 0xDEADBEEF;
const unsigned long long BUF_CANARY = 0xFACEFEED;

struct stack
{
    CANARY_PROTECTION( long long stackCanary1; )
    elem_t *data;

    size_t capacity;
    size_t size;

    HASH_PROTECTION  ( unsigned hash; )
    CANARY_PROTECTION( long long stackCanary2; )
};

struct stackErrorField
{
    unsigned stack_null      : 1;
    unsigned data_null       : 1;
    unsigned small_capacity  : 1;
    unsigned stack_underflow : 1;
    unsigned realloc_failed  : 1;
    unsigned changed_canary  : 1;
    unsigned changed_hash    : 1;
};

/**
 * @brief determines if the stack is corrupted or not
 * 
 * @param [in] stk 
 * @return stackErrorField 
 */
stackErrorField stackError(stack *stk);

/**
 * @brief creator for struct stack
 * 
 * @param [in, out] stk 
 * @param [in] capacity 
 * @return stackErrorField 
 */
stackErrorField stackCtor(stack *stk, size_t capacity);

/**
 * @brief destructor for struct stack
 * 
 * @param [in, out] stk 
 * @return stackErrorField 
 */
stackErrorField stackDtor(stack *stk);

/**
 * @brief prints out all the info about the stack
 * 
 * @param [in, out] stk 
 * @param [in] file 
 * @param [in] line 
 * @param [in] function 
 * @return stackErrorField 
 */
stackErrorField stackDump(stack *stk, FILE *f, const char *file, int line, const char *function);

/**
 * @brief prints out the errors of the stack (if they are present)
 * 
 * @param [in] error 
 */
void printStackErrors(stackErrorField error);

/**
 * @brief an easier way of printing the info
 * 
 */
#define STACK_DUMP(stk, f) stackDump((stk), f, __FILE__, __LINE__, __func__);

/**
 * @brief adds the value to the top of the stack
 * 
 * @param [in, out] stk 
 * @param [in] value 
 * @return stackErrorField 
 */
stackErrorField stackPush(stack *stk, elem_t value);

/**
 * @brief returns (to the variable) the top element of the stack and deletes it
 * 
 * @param [in, out] stk 
 * @param [out] returnValue 
 * @return stackErrorField 
 */
stackErrorField stackPop(stack *stk, elem_t *returnValue);

/**
 * @brief changes the capacity of the stack
 * 
 * @param [out] stk 
 * @return stackErrorField 
 */
stackErrorField stackRealloc(stack *stk);

/**
 * @brief a calloc function that adds to the size of buffer 2 times the canary size if needed
 * 
 * @param [in] elementNum 
 * @param [in] elementSize 
 * @return void* pointer to the buffer
 */
void *myCalloc(size_t elementNum, size_t elementSize);

/**
 * @brief calculates the hash of stack and its data
 * 
 * @param [in] stk 
 * @return (unsigned int) the hash value
 */
unsigned int stackHashCalc(stack *stk);

/**
 * @brief checks if the saved hash of stack equals to the newly calculated
 * 
 * @param stk 
 * @return stackErrorField 
 */
stackErrorField stackHashCheck(stack *stk);

/**
 * @brief returns the int value of struct stackErrorField (which is a bit-field)
 * 
 * @param error 
 * @return unsigned 
 */
unsigned errorFieldToU(stackErrorField error);

#endif //STACK_H
