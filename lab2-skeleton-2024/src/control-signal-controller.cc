#include "control-signal-controller.h"

#include <iostream>

ControlSignalController::ControlSignalController(MemAddress& PC, ALU& alu,
                                                 RegisterFile& regfile,
                                                 DataMemory& dataMemory)
    : PC(PC),
      alu(alu),
      regFile(regfile),
      dataMemory(dataMemory),
      opcode(0),
      funct3(0),
      funct7(0),
      rs1Value(0),
      rs2Value(0),
      immediate(0),
      aluOp(ALUOp::ADD),  // Default ALU operation
      memRead(false),
      memWrite(false),
      regWrite(false),
      branch(false),
      branchTarget(0) {}

void ControlSignalController::setOpCode(RegNumber opcode) {
  this->opcode = opcode;
}

void ControlSignalController::setFunct3(RegNumber funct3) {
  this->funct3 = funct3;
}

void ControlSignalController::setFunct7(RegNumber funct7) {
  this->funct7 = funct7;
}

void ControlSignalController::setRS1Value(RegValue rs1Value) {
  this->rs1Value = rs1Value;
}

void ControlSignalController::setRS2Value(RegValue rs2Value) {
  this->rs2Value = rs2Value;
}

void ControlSignalController::setImmediate(RegValue immediate) {
  this->immediate = immediate;
}

ALUOp ControlSignalController::getALUOp() const { return aluOp; }

bool ControlSignalController::isMemRead() const { return memRead; }

bool ControlSignalController::isMemWrite() const { return memWrite; }

bool ControlSignalController::isRegWrite() const { return regWrite; }

bool ControlSignalController::isBranch() const { return branch; }

MemAddress ControlSignalController::getBranchTarget() const {
  return branchTarget;
}

void ControlSignalController::generateControlSignals() {
  // Reset control signals
  aluOp = ALUOp::ADD;  // Default
  memRead = false;
  memWrite = false;
  regWrite = false;
  branch = false;
  branchTarget = 0;

  switch (opcode) {
    case 0b0110011:  // R-type
      regWrite = true;
      switch (funct3) {
        case 0x0:
          if (funct7 == 0x00) {
            aluOp = ALUOp::ADD;
          } else if (funct7 == 0x20) {
            aluOp = ALUOp::SUB;
          }
          break;
        case 0x1:
          aluOp = ALUOp::SLL;
          break;
        case 0x2:
          aluOp = ALUOp::SLT;
          break;
        case 0x3:
          aluOp = ALUOp::SLTU;
          break;
        case 0x4:
          aluOp = ALUOp::XOR;
          break;
        case 0x5:
          if (funct7 == 0x00) {
            aluOp = ALUOp::SRL;
          } else if (funct7 == 0x20) {
            aluOp = ALUOp::SRA;
          }
          break;
        case 0x6:
          aluOp = ALUOp::OR;
          break;
        case 0x7:
          aluOp = ALUOp::AND;
          break;
      }
      break;
    case 0b0010011:  // I-type arithmetic
      regWrite = true;
      switch (funct3) {
        case 0x0:
          aluOp = ALUOp::ADD_I;
          break;
        case 0x1:
          aluOp = ALUOp::SLL;  
          break;
        case 0x2:
          aluOp = ALUOp::SLT_I;
          break;
        case 0x3:
          aluOp = ALUOp::SLTU_I;
          break;
        case 0x4:
          aluOp = ALUOp::XOR;
          break;
        case 0x5:
          if (funct7 == 0x00) {
            aluOp = ALUOp::SRL;  
          } else if (funct7 == 0x20) {
            aluOp = ALUOp::SRA;  
          }
          break;
        case 0x6:
          aluOp = ALUOp::OR;
          break;
        case 0x7:
          aluOp = ALUOp::AND;
          break;
      }
      break;
    case 0b0000011:  // I-type load
      regWrite = true;
      memRead = true;
      aluOp = ALUOp::ADD_I;  
      break;
    case 0b0100011:  // S-type
      memWrite = true;
      aluOp = ALUOp::ADD_I;  
      break;
    case 0b1100011:  // B-type
      branch = true;
      switch (funct3) {
        case 0x0:
          aluOp = ALUOp::EQ;
          break;
        case 0x1:
          aluOp = ALUOp::NE;
          break;
        case 0x4:
          aluOp = ALUOp::SLT;
          break;
        case 0x5:
        case 0x7:
          aluOp = ALUOp::GE;
          break;
        case 0x6:
          aluOp = ALUOp::SLTU;
          break;
      }
      branchTarget = PC + immediate;
      break;
    case 0b0110111:  
      regWrite = true;
      aluOp = ALUOp::ADD_I;  
      break;
    case 0b0010111:  
      regWrite = true;
      aluOp = ALUOp::ADD_I;  
      break;
    case 0b1101111:  
      regWrite = true;
      aluOp = ALUOp::ADD_I; 
      branchTarget = PC + immediate;
      break;
  }
}