/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "arch.h"
#include "inst-decoder.h"

#include <map>

enum class EAluOp {
    NOP = 0,
    ADD, // 1
    SUB, // 2
    SHIFTL, // 3
    SHIFTR, // 4
    SEXTSHIFTR,//5
    OR, // 6
    AND, // 7
    XOR, // 8
    LEQ, // 9
    LEQS, // 10
    GEQ, // 11
    GEQS, // 12
    R9ADD // 13
};

// ALU component responsible for hardcoded values needed to shift with.
struct FShiftValues
{
  const RegValue shiftValue16_ = 16;
};

/* The ALU component performs the specified operation on operands A and B
 * when asked to propagate the result. The operation is specified through
 * the ALUOp.
 */
class ALU
{
  public:
    ALU();

    void setA(RegValue A) { this->A = A; }
    void setB(RegValue B) { this->B = B; }

    RegValue getResult();

    void setOp(EAluOp op) { this->Op_ = op; }

  private:
    RegValue A;
    RegValue B;
    
    EAluOp Op_;
    bool Carry_;
    bool SignedOverflow_;

  FShiftValues ShiftValues_;
};

#endif /* __ALU_H__ */
