#ifndef  __CMD_ENUM_H__
#define  __CMD_ENUM_H__

const int MaxCommandLength = 16;
const int RegistersNumber  = 10;

const char * const Signature = "will";

const int PrecisionConst   = 100;
#define PrecisionFormat "%.2f"

#define DEF_CMD(name, num, hasArg, function) name = num,

#define DEF_CMD_JMP(name, num, hasArg, sign) DEF_CMD(name, num, hasArg, sign)

enum ArgTypes
{
    ARG_TYPE_NOTHING  = -1,
    ARG_TYPE_NUMBER   = 0,
    ARG_TYPE_REGISTER = 0x20,
    ARG_TYPE_STRING   = -2,
    ARG_TYPE_ADDRESS  =  0x40
};

enum Commands
{
    #include "commands.h"
};

#undef DEF_CMD_JMP
#undef DEF_CMD

#endif //__CMD_ENUM_H__