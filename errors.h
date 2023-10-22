#ifndef  __ERRORS_H__
#define  __ERRORS_H__

enum Errors
{
    STACK_UNDERFLOW    = 1,
    INCORRECT_PUSH     = 2,
    UNKNOWN_COMMAND    = 3,
    DIVISION_BY_ZERO   = 4,
    FILE_ERROR         = 5,
    INCORRECT_INPUT    = 6,
    INCORRECT_POP      = 7,
    BAD_SIGNATURE      = 8,
    BAD_COM_VERSION    = 9,
    MEMORY_ERROR       = 10,
    INCORRECT_ARGUMENT = 11,
    INCORRECT_ADDRESS  = 12,
    LABEL_ADD_ERROR    = 13
};

#endif //__ERRORS_H__
