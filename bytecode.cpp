#include "bytecode.h"


uint8_t cycle_table[8] = {
    1,
    2,
    2,
    2,
    6,
    2,
    2,
};


uint8_t fibonacci[] = {

    // initializer

    OP_MOV, REGS(X3, X0),
    OP_MOVI, REGS(X0, 0), IMM(0),
    OP_MOVI, REGS(X1, 0), IMM(1),
    OP_MOV, REGS(X4, X1),

    OP_SUB, REGS(X3, X0),
    OP_BNZ, 1,
    OP_RETURN, // early-out for fibonacci(0)


    // loop start
    OP_MOV, REGS(X2, X0),
    OP_MOV, REGS(X0, X1),
    OP_ADD, REGS(X1, X2),
    OP_SUB, REGS(X3, X4),
    OP_BNZ, uint8_t(-10),

    OP_RETURN,
};
