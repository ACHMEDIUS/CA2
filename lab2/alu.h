/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016, 2018 Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "arch.h"
#include "inst-decoder.h"

enum class ALUOp {
    NOP,
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SLL,
    SRL,
    SRA,
    SLT,
    SLTU,
    // TODO: Add other operations as necessary
};

class ALU
{
public:
    ALU();

    void setA(RegValue A) { this->A = A; }
    void setB(RegValue B) { this->B = B; }

    RegValue getResult();

    void setOp(ALUOp op) { this->op = op; }

private:
    RegValue A;
    RegValue B;
    ALUOp op;

    // Private helper functions
    RegValue add();
    RegValue sub();
    RegValue bitwiseAnd();
    RegValue bitwiseOr();
    RegValue bitwiseXor();
    RegValue shiftLeftLogical();
    RegValue shiftRightLogical();
    RegValue shiftRightArithmetic();
    RegValue setLessThan();
    RegValue setLessThanUnsigned();
    // TODO: Add other helper functions for different operations
};

#endif /* __ALU_H__ */
