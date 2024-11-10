/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"
#include "alu.h"
#include "arch.h"
#include "inst-decoder.h"

#include <cstdint>
#include <exception>
#include <iostream>

/*
 * Instruction fetch
 */

void InstructionFetchStage::propagate() {
  try {
    /* TODO: implement instruction fetch from instruction memory. */
    if (pcMux.getOutput() != 0) {
      PC = pcMux.getOutput();
    }

    // Get instruction from memory
    instructionMemory.setAddress(PC);
    instructionMemory.setSize(4);
    instructionWord = instructionMemory.getValue();

    // std::cout << "Instruction: " << std::hex << instructionWord << std::endl;

    /* Enable this once you have implemented instruction fetch. */
    if (instructionWord == TestEndMarker) {
      throw TestEndMarkerEncountered(PC);
    }

  } catch (TestEndMarkerEncountered &e) {
    throw;
  } catch (std::exception &e) {
    throw InstructionFetchFailure(PC);
  }
}

void InstructionFetchStage::clockPulse() {
  /* TODO: write necessary fields in pipeline register */
  if_id.PC = PC;
  if_id.instructionWord = instructionWord;
}

/*
 * Instruction decode
 */

void dump_instruction(std::ostream &os, const uint32_t instructionWord,
                      const InstructionDecoder &decoder);

void InstructionDecodeStage::propagate() {
  /* TODO: set instruction word on the instruction decoder */
  PC = if_id.PC;
  decoder.setInstructionWord(if_id.instructionWord);
  decoder.setPC(PC);

  /* TODO: need a control signals class that generates control
   * signals from a given opcode and function code.
   */

  csc.setOpCode(decoder.getOpcode());
  csc.setFunc(decoder.getFunc());
  csc.setImmediate(decoder.getImmediate());
  csc.setPC(PC);

  /* debug mode: dump decoded instructions to cerr.
   * In case of no pipelining: always dump.
   * In case of pipelining: special case, if the PC == 0x0 (so on the
   * first cycle), don't dump an instruction. This avoids dumping a
   * dummy instruction on the first cycle when ID is effectively running
   * uninitialized.
   */
  if (debugMode && (!pipelining || (pipelining && PC != 0x0))) {
    /* Dump program counter & decoded instruction in debug mode */
    auto storeFlags(std::cerr.flags());

    std::cerr << std::hex << std::showbase << PC << "\t";
    std::cerr.setf(storeFlags);

    std::cerr << decoder << std::endl;
  }

  /* TODO: register fetch and other matters */
  regfile.setRS1(decoder.getA());
  regfile.setRS2(decoder.getB());
  regfile.setRD(decoder.getD());

  pcMux.setInput(Input::FIRST, PC + 4);
}

void InstructionDecodeStage::clockPulse() {
  /* ignore the "instruction" in the first cycle. */
  if (!pipelining || (pipelining && PC != 0x0))
    ++nInstrIssued;

  csc.setA(regfile.getReadData1());
  csc.setB(regfile.getReadData2());
  csc.getControlSignalsForInstr();

  switch (decoder.getOpcode()) {
  case 0x05:
    id_ex.aluOp = ALUOp::NOP;
    nStalls++;
    break;
  case 0x03: // l.bnf
    if (flag == 0) {
      id_ex.aluOp = csc.getALUOp();
    } else {
      id_ex.aluOp = ALUOp::NOP;
    }
    break;
  case 0x04: // l.bf
    if (flag == 1) {
      id_ex.aluOp = csc.getALUOp();
    } else {
      id_ex.aluOp = ALUOp::NOP;
    }
    break;
  default:
    id_ex.aluOp = csc.getALUOp();
    break;
  }

  /* TODO: write necessary fields in pipeline register */
  id_ex.PC = PC;
  id_ex.A = csc.getA();
  id_ex.B = csc.getB();
  id_ex.imm = decoder.getImmediate();
  id_ex.D = decoder.getD();
  id_ex.valueToWrite = regfile.getReadData2();
}

/*
 * Execute
 */

void ExecuteStage::propagate() {
  /* TODO configure ALU based on control signals and using inputs
   * from pipeline register.
   * Consider using the Mux class.
   */
  PC = id_ex.PC;
  alu.setA(id_ex.A);
  alu.setB(id_ex.B);
  alu.setOp(id_ex.aluOp);
  rd = id_ex.D;
  aluOp = id_ex.aluOp;
  valueToWrite = id_ex.valueToWrite;
}

void ExecuteStage::clockPulse() {
  /* TODO: write necessary fields in pipeline register. This
   * includes the result (output) of the ALU. For memory-operations
   * the ALU computes the effective memory address.
   */

  ex_m.PC = PC;

  ex_m.aluRes = alu.getResult();
  ex_m.rd = rd;
  ex_m.aluOp = aluOp;
  ex_m.valueToWrite = valueToWrite;
}

/*
 * Memory
 */

void MemoryStage::propagate() {
  /* TODO: configure data memory based on control signals and using
   * inputs from pipeline register.
   */
  PC = ex_m.PC;
  aluRes = ex_m.aluRes;
  rd = ex_m.rd;
  aluOp = ex_m.aluOp;

  pcMux.setInput(Input::SECOND, aluRes);

  pcMux.setSelector(aluOp == ALUOp::JUMP ? Input::SECOND : Input::FIRST);

  dataMemory.setAddress(aluRes);
  dataMemory.setDataIn(ex_m.valueToWrite);

  bool const read = aluOp == ALUOp::LB || aluOp == ALUOp::LW ||
                    aluOp == ALUOp::LDW;

  dataMemory.setReadEnable(read);

  bool const write = aluOp == ALUOp::SB || aluOp == ALUOp::SW ||
                     aluOp == ALUOp::SDW;

  dataMemory.setWriteEnable(write);

  switch (aluOp) {
  case ALUOp::LB:
  case ALUOp::SB:
    dataMemory.setSize(1);
    break;
  case ALUOp::LW:
  case ALUOp::SW:
    dataMemory.setSize(4);
    break;
  case ALUOp::LDW:
  case ALUOp::SDW:
    dataMemory.setSize(8);
    break;
  case ALUOp::JUMP:
    if (/*flag no delay*/ false) {
      aluRes = PC + 4;
    } else {
      aluRes = PC + 8;
    }
    break;
  default:
    // Do nothing
    break;
  }
}

void MemoryStage::clockPulse() {
  /* TODO: pulse the data memory */
  dataMemory.clockPulse();
  /* TODO: write necessary fields in pipeline register */

  m_wb.PC = PC;
  m_wb.aluRes = aluRes;
  m_wb.rd = rd;
  m_wb.aluOp = aluOp;
  m_wb.dataFromMemory = dataMemory.getDataOut(false);
}

/*
 * Write back
 */

void WriteBackStage::propagate() {
  if (!pipelining || (pipelining && m_wb.PC != 0x0))
    ++nInstrCompleted;

  /* TODO: configure write lines of register file based on control
   * signals
   */

  rd = m_wb.rd;
  aluOp = m_wb.aluOp;

  wbMux.setInput(Input::FIRST, m_wb.aluRes);
  wbMux.setInput(Input::SECOND, m_wb.dataFromMemory);

  switch (aluOp) {
  case ALUOp::LB:
  case ALUOp::LW:
  case ALUOp::LDW:
  case ALUOp::SB:
  case ALUOp::SW:
  case ALUOp::SDW:
    wbMux.setSelector(Input::SECOND);
    break;
  default:
    wbMux.setSelector(Input::FIRST);
    break;
  }

  switch (aluOp) {
  case ALUOp::EQ:
  case ALUOp::GE:
  case ALUOp::GT:
  case ALUOp::LE:
  case ALUOp::LT:
  case ALUOp::NE:
    flag = m_wb.aluRes;
    break;
  default:
    break;
  }

  regfile.setWriteData(wbMux.getOutput());
}

void WriteBackStage::clockPulse() {
  /* TODO: pulse the register file */
  regfile.setRD(rd);
  regfile.setWriteEnable(aluOp != ALUOp::GE && aluOp != ALUOp::NE &&
                         aluOp != ALUOp::LT && aluOp != ALUOp::LE &&
                         aluOp != ALUOp::EQ);

  regfile.clockPulse();
  regfile.setWriteEnable(false);
}
