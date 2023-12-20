DEF_CMD(HLT,     -1, 0, 0)

DEF_CMD(PUSH,     0, 1, commandPush(spu, bufIn, instructionPointer))

DEF_CMD(IN,       1, 0, commandIn(spu))

DEF_CMD(OUT,      2, 0, commandOut(spu, fout))

DEF_CMD(ADD,      3, 0, commandArithm(spu, bufIn[*instructionPointer]))

DEF_CMD(SUB,      4, 0, commandArithm(spu, bufIn[*instructionPointer]))

DEF_CMD(MUL,      5, 0, commandArithm(spu, bufIn[*instructionPointer]))

DEF_CMD(DIV,      6, 0, commandArithm(spu, bufIn[*instructionPointer]))

DEF_CMD(SQRT,     7, 0, commandSqrt(spu))

DEF_CMD(POP,     33, 1, commandPop(spu, bufIn, instructionPointer))

DEF_CMD(PUSH_R,  32, 1, commandPushR(spu, bufIn, instructionPointer))

DEF_CMD(SIN,      9, 0, commandTrigonom(spu, bufIn[*instructionPointer]))

DEF_CMD(COS,     10, 0, commandTrigonom(spu, bufIn[*instructionPointer]))

DEF_CMD(TAN,     11, 0, commandTrigonom(spu, bufIn[*instructionPointer]))

DEF_CMD(COT,     12, 0, commandTrigonom(spu, bufIn[*instructionPointer]))

DEF_CMD(MEOW,    13, 0, commandMeow(fout))

DEF_CMD(WMEOW,   14, 0, commandWeirdMeow(fout))

DEF_CMD(JMP,     1 | ARG_TYPE_ADDRESS, 1, commandJmp(bufIn, instructionPointer))

DEF_CMD_JMP(JA,  2 | ARG_TYPE_ADDRESS, 1, >)

DEF_CMD_JMP(JAE, 3 | ARG_TYPE_ADDRESS, 1, >=)

DEF_CMD_JMP(JB,  4 | ARG_TYPE_ADDRESS, 1, <)

DEF_CMD_JMP(JBE, 5 | ARG_TYPE_ADDRESS, 1, <=)

DEF_CMD_JMP(JE,  6 | ARG_TYPE_ADDRESS, 1, ==)

DEF_CMD_JMP(JN,  7 | ARG_TYPE_ADDRESS, 1, !=)

DEF_CMD(CALL,    20 | ARG_TYPE_ADDRESS, 1, commandCall(spu, bufIn, instructionPointer))

DEF_CMD(RET,     21,                    0, commandRet (spu, bufIn, instructionPointer))
