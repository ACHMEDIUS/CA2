//
// CSC
//

#ifndef _CSC_H_
#define _CSC_H_

#include "alu.h"
#include "arch.h"
#include "reg-file.h"

class CSC {
public:
  void setPC(MemAddress PC);

  void getControlSignalsForInstr();

  void setOpCode(RegNumber opc);

  void setFunc(RegNumber func);

  void setA(RegValue a);

  void setB(RegValue b);

  void setImmediate(RegValue imm);

  RegValue getA() const;

  RegValue getB() const;

  ALUOp getALUOp() const;

private:
  RegValue A = 0;
  RegValue B = 0;

  ALUOp oper = ALUOp::NOP;

  RegValue immediate{};
  RegNumber opCode{};
  RegNumber func{};
  MemAddress PC{};

  RegValue RA{};
  RegValue RB{};
};

#endif // _CSC_H_