DEF_CMD(HLC,     -1, 0, 0)

DEF_CMD(PUSH,     0, 1, commandPush(spu, bufIn))

DEF_CMD(IN,       1, 0, commandIn(spu))

DEF_CMD(OUT,      2, 0, commandOut(spu, fout))

DEF_CMD(ADD,      3, 0, commandArithm(spu, **bufIn))

DEF_CMD(SUB,      4, 0, commandArithm(spu, **bufIn))

DEF_CMD(MUL,      5, 0, commandArithm(spu, **bufIn))

DEF_CMD(DIV,      6, 0, commandArithm(spu, **bufIn))

DEF_CMD(SQRT,     7, 0, commandSqrt(spu))

DEF_CMD(POP,     33, 1, commandPop(spu, bufIn))

DEF_CMD(PUSH_R,  32, 1, commandPushR(spu, bufIn))

DEF_CMD(SIN,      9, 0, commandTrigonom(spu, **bufIn))

DEF_CMD(COS,     10, 0, commandTrigonom(spu, **bufIn))

DEF_CMD(TAN,     11, 0, commandTrigonom(spu, **bufIn))

DEF_CMD(COT,     12, 0, commandTrigonom(spu, **bufIn))

DEF_CMD(MEOW,    13, 0, commandMeow(fout))

DEF_CMD(WMEOW,   14, 0, commandWeirdMeow(fout))
