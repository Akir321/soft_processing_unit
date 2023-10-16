#ifndef  __CMD_ENUM_H__
#define  __CMD_ENUM_H__

const int MaxCommandLength = 16;
const int RegistersNumber  = 4;

const char * const Signature = "will";

const int PrecisionConst   = 100;
#define PrecisionFormat "%.2f"

#define DEF_CMD(name, num) name = num,

enum commands
{
    #include "commands.h"
};

#undef DEF_CMD

#endif //__CMD_ENUM_H__