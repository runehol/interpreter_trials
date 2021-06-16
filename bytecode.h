#ifndef BYTECODE_H
#define BYTECODE_H

#include <cstdint>

enum Opcode
{
    OP_RETURN = 0, /* no args */
    OP_ADD = 1,       /* 8-bit dest, src */
    OP_SUB = 2,       /* 8-bit dest, src */
    OP_MOV = 3,       /* 8-bit dest, src */
    OP_MOVI = 4,      /* 8-bit dest, 0. 32-bit immediate */
    OP_B = 5,         /* 8-bit relative */
    OP_BNZ = 6        /* 8-bit relative */
};


enum Register
{
    X0 = 0,
    X1 = 1,
    X2 = 2,
    X3 = 3,
    X4 = 4,
    X5 = 5,
    X6 = 6,
    X7 = 7,
    X8 = 8,
    X9 = 9,
    X10 = 10,
    X11 = 11,
    X12 = 12,
    X13 = 13,
    X14 = 14,
    X15 = 15

};

#define REGS(dest, src) ((((dest)&0xf)<<4)|((src)&0xf))
#define IMM(imm) (((imm)>>0)&0xff), (((imm)>>8)&0xff), (((imm)>>16)&0xff), (((imm)>>24)&0xff)


extern uint8_t cycle_table[];

extern uint8_t fibonacci[];

#endif //BYTECODE_H
