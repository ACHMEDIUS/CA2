#ifndef __CONTROL_SIGNAL_CONTROLLER_H__
#define __CONTROL_SIGNAL_CONTROLLER_H__

#include "alu.h"
#include "arch.h" 
#include "memory-control.h"

class ControlSignalController {
 public:
  ControlSignalController(MemAddress& PC, ALU& alu, RegisterFile& regfile,
                         DataMemory& dataMemory);

  void setOpCode(RegNumber opcode);
  void setFunct3(RegNumber funct3);
  void setFunct7(RegNumber funct7);
  void setRS1Value(RegValue rs1Value);
  void setRS2Value(RegValue rs2Value);
  void setImmediate(RegValue immediate);

  void generateControlSignals();

  ALUOp getALUOp() const;
  bool isMemRead() const;
  bool isMemWrite() const;
  bool isRegWrite() const;
  bool isBranch() const;
  MemAddress getBranchTarget() const;

 private:
  MemAddress& PC;
  ALU& alu;
  RegisterFile& regFile;
  DataMemory& dataMemory;

  RegNumber opcode;
  RegNumber funct3;
  RegNumber funct7;
  RegValue rs1Value;
  RegValue rs2Value;
  RegValue immediate;

  ALUOp aluOp;
  bool memRead;
  bool memWrite;
  bool regWrite;
  bool branch;
  MemAddress branchTarget;

  
};

#endif  // __CONTROL_SIGNAL_CONTROLLER_H_