//
// CSC Implementation
//

#include "helper.h"
#include "alu.h"
#include "arch.h"

#include <iostream>

void CSC::getControlSignalsForInstr() {
  // ...
  // TODO: impl opcodes

  // l.nop
  if (opCode == 0x05) {
    oper = ALUOp::NOP;
    A = 0;
    B = 0;
    return;
  }

  // l.add, sub, etc
  // 111000DDDDDAAAAABBBBB-00----0000
  if (opCode == 0x38) {
    switch (func) {
    case 0:
      oper = ALUOp::ADD;
      A = RA;
      B = RB;
      break;
    case 2:
      oper = ALUOp::SUB;
      A = RA;
      B = RB;
      break;
    case 3:
      oper = ALUOp::AND;
      A = RA;
      B = RB;
      break;
    case 4:
      oper = ALUOp::OR;
      A = RA;
      B = RB;
      break;
    case 5:
      oper = ALUOp::XOR;
      A = RA;
      B = RB;
      break;
    case 8:
      oper = ALUOp::LS; // l.sll
      A = RA;
      // we strip the 6th bit from RB, since we are only interested in 32 bit
      // shifts
      B = RB & 0x1F;
      break;
    case 0b11000: // l.srl
      oper = ALUOp::RS;
      A = RA;
      B = RB & 0x1F;
      break;
    case 0b101000: // l.sra
      oper = ALUOp::RSA;
      A = RA;
      B = RB & 0x1F;
      break;
    }
    return;
  }

  // l.addi
  // 100111DDDDDAAAAAIIIIIIIIIIIIIIII
  if (opCode == 0x27) {
    oper = ALUOp::ADD;
    A = RA;
    B = immediate;
    return;
  }

  // l.lwz rD,I(rA)
  // 100001 | DDDDD | AAAAA | IIIIIIIIIIIIIIII
  if (opCode == 0x21) {
    oper = ALUOp::LW;
    A = RA;
    B = immediate;
    return;
  }

  // l.lbz rD,I(rA)
  // 100011 | DDDDD | AAAAA | IIIIIIIIIIIIIIII
  if (opCode == 0x23) {
    oper = ALUOp::LB;
    A = RA;
    B = immediate;
    return;
  }

  // l.sw
  // 110101 | IIIII | AAAAA| BBBBB | IIIIIIIIIII
  if (opCode == 0x35) {
    oper = ALUOp::SW;
    A = RA;
    B = immediate;
    return;
  }

  // l.sb
  if (opCode == 0x36) {
    oper = ALUOp::SB;
    A = RA;
    B = immediate;
    return;
  }

  // l.sfi
  // 101111 | func(5) | AAAAA | IIIIIIIIIIIIIIII
  if (opCode == 0x2F) {
    switch (func) {
    case 0: // l.sfeq
      oper = ALUOp::EQ;
      A = RA;
      B = immediate;
      break;
    case 1: // l.sfne
      oper = ALUOp::NE;
      A = RA;
      B = immediate;
      break;
    // case 2:
    case 10: // l.sfgts
      oper = ALUOp::GT;
      A = RA;
      B = immediate;
      break;

    // case 4:
    case 12: // l.sflts
      oper = ALUOp::LT;
      A = RA;
      B = immediate;
      break;

    // case 3:
    case 11: // l.sfges
      oper = ALUOp::GE;
      A = RA;
      B = immediate;
      break;
    // case 5:
    case 13: // l.sfles
      oper = ALUOp::LE;
      A = RA;
      B = immediate;
    default:
      break;
    }
    return;
  }

  // l.sf
  if (opCode == 0x39) {
    switch (func) {
    case 0: // l.sfeq
      oper = ALUOp::EQ;
      A = RA;
      B = RB;
      break;
    case 1: // l.sfne
      oper = ALUOp::NE;
      A = RA;
      B = RB;
      break;
    case 10: // l.sfgts
      oper = ALUOp::GT;
      A = RA;
      B = RB;
      break;
    case 11: // l.sfges
      oper = ALUOp::GE;
      A = RA;
      B = RB;
      break;
    case 12: // l.sflts
      oper = ALUOp::LT;
      A = RA;
      B = RB;
      break;
    case 13: // l.sfles
      oper = ALUOp::LE;
      A = RA;
      B = RB;
      break;
    }
    return;
  }

  // l.ori
  // 101010 | DDDDD | AAAAA | IIIIIIIIIIIIIIII
  if (opCode == 0x2A) {
    oper = ALUOp::OR;
    A = RA;
    B = immediate;
    return;
  }

  // l.andi
  // 101001 | DDDDD | AAAAA | IIIIIIIIIIIIIIII
  if (opCode == 0x29) {
    oper = ALUOp::AND;
    A = RA;
    B = immediate;
    return;
  }

  // l.movhi
  //
  if (opCode == 0x06 && func == 0) {
    oper = ALUOp::NOP;
    A = 0;
    B = immediate << 16;
    return;
  }

  // l.jal
  // 000001 | IIIIIIIIIIIIIIIIIIIII
  if (opCode == 0x01) {
    oper = ALUOp::JUMP;
    A = PC;
    B = immediate;
    return;
  }

  // l.jr
  if (opCode == 0x11) {
    oper = ALUOp::JUMP;
    A = 0;
    B = RB;
    return;
  }

  // l.j
  if (opCode == 0x00) {
    oper = ALUOp::JUMP;
    A = PC;
    B = immediate;
    return;
  }

  // l.bf
  if (opCode == 0x04) {
    oper = ALUOp::JUMP;
    A = PC;
    B = immediate;
    return;
  }

  std::cout << "Error: control signals not implemented for: " << opCode
            << std::endl;

  std::cout << "PC: " << std::hex << PC << std::endl;

  throw std::runtime_error("control signals not implemented");
}

void CSC::setOpCode(RegNumber opc) { this->opCode = opc; }

void CSC::setFunc(RegNumber func) { this->func = func; }

void CSC::setA(RegValue a) { this->RA = a; }

void CSC::setB(RegValue b) { this->RB = b; }

void CSC::setImmediate(RegValue imm) { this->immediate = imm; }

RegValue CSC::getRS1() const { return this->A; }

RegValue CSC::getRS2() const { return this->B; }

ALUOp CSC::getALUOp() const { return this->oper; }

void CSC::setPC(MemAddress PC) { this->PC = PC; }
