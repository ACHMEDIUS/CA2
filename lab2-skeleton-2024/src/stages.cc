/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"
#include "control-signal-controller.h"

#include <iostream>

/*
 * Instruction fetch
 */

void InstructionFetchStage::propagate()
{
  try
  {
    // 1. Set PC to instructionMemory
    instructionMemory.setAddress(PC);
    instructionMemory.setSize(4); // Assuming 32-bit instructions

    // 2. Fetch instruction
    instructionWord = instructionMemory.getValue();

    // 3. Check for test end marker
    if (instructionWord == TestEndMarker)
      throw TestEndMarkerEncountered(PC);

    // 4. Update PC for next instruction
    PC = PC + 4;
  }
  catch (TestEndMarkerEncountered &e)
  {
    throw;
  }
  catch (std::exception &e)
  {
    throw InstructionFetchFailure(PC);
  }
}
void InstructionFetchStage::clockPulse()
{
  if_id.PC = PC;
  if_id.instructionWord = instructionWord;
}

/*
 * Instruction decode
 */

void dump_instruction(std::ostream &os, const uint32_t instructionWord,
                      const InstructionDecoder &decoder);

void InstructionDecodeStage::propagate()
{
  ControlSignalController controlSignalController(PC, alu, regfile, dataMemory);

  instructionWord = if_id.instructionWord;

  decoder.setInstructionWord(instructionWord);

  rs1 = decoder.getRS1();
  rs2 = decoder.getRS2();
  rd = decoder.getRD();
  immediate = decoder.getImmediate();

  controlSignalController.setOpCode(decoder.getOpcode());
  controlSignalController.setFunct3(decoder.getFunct3());
  controlSignalController.setFunct7(decoder.getFunct7());
  controlSignalController.setRS1Value(regfile.getReadData1());
  controlSignalController.setRS2Value(regfile.getReadData2());
  controlSignalController.setImmediate(immediate);

  controlSignalController.generateControlSignals();

  PC = if_id.PC;

  /* debug mode: dump decoded instructions to cerr.
   * In case of no pipelining: always dump.
   * In case of pipelining: special case, if the PC == 0x0 (so on the
   * first cycle), don't dump an instruction. This avoids dumping a
   * dummy instruction on the first cycle when ID is effectively running
   * uninitialized.
   */
  if (debugMode && (!pipelining || (pipelining && PC != 0x0)))
  {
    /* Dump program counter & decoded instruction in debug mode */
    auto storeFlags(std::cerr.flags());

    std::cerr << std::hex << std::showbase << PC << "\t";
    std::cerr.setf(storeFlags);

    std::cerr << decoder << std::endl;
  }
}

void InstructionDecodeStage::clockPulse()
{
  ControlSignalController controlSignalController(PC, alu, regfile, dataMemory);
  /* ignore the "instruction" in the first cycle. */
  if (!pipelining || (pipelining && PC != 0x0))
    ++nInstrIssued;

  id_ex.PC = PC;
  id_ex.rs1Value = regfile.getReadData1();
  id_ex.rs2Value = regfile.getReadData2();
  id_ex.rd = rd;
  id_ex.immediate = immediate;
  // id_ex.controlSignals = controlSignalController;
}

/*
 * Execute
 */

void ExecuteStage::propagate()
{
  alu.setA(id_ex.rs1Value);

  PC = id_ex.PC;

  if (id_ex.controlSignals.getALUOp() == ALUOp::ADD_I ||
      id_ex.controlSignals.getALUOp() == ALUOp::SLT_I ||
      id_ex.controlSignals.getALUOp() == ALUOp::SLTU_I)
  {
    alu.setB(id_ex.immediate);
  }
  else
  {
    alu.setB(id_ex.rs2Value);
  }

  alu.setOp(id_ex.controlSignals.getALUOp());
  // Perform ALU operation
  aluResult = alu.getResult();
  rs2Value = id_ex.rs2Value;
  rd = id_ex.rd;
}

void ExecuteStage::clockPulse()
{
  ex_m.PC = PC;
  ex_m.aluResult = aluResult;
  ex_m.rs2Value = rs2Value;
  ex_m.rd = rd;
  // ex_m.controlSignals = id_ex.controlSignals;
}

/*
 * Memory
 */

void MemoryStage::propagate()
{
  if (ex_m.controlSignals.isMemRead())
  {
    dataMemory.setAddress(ex_m.aluResult);
    dataMemory.setSize(4);
    dataMemory.setReadEnable(true);
  }
  else if (ex_m.controlSignals.isMemWrite())
  {
    dataMemory.setAddress(ex_m.aluResult);
    dataMemory.setSize(4);
    dataMemory.setWriteEnable(true);
    dataMemory.setDataIn(ex_m.rs2Value);
  }

  PC = ex_m.PC;

  memData = ex_m.aluResult;
  rd = ex_m.rd;
}

void MemoryStage::clockPulse()
{
  dataMemory.clockPulse();

  m_wb.PC = ex_m.PC;
  m_wb.aluResult = memData;
  m_wb.memData = dataMemory.getDataOut();
  m_wb.rd = rd;
  // m_wb.controlSignals = ex_m.controlSignals;
  m_wb.PC = PC;
}

/*
 * Write back
 */

void WriteBackStage::propagate()
{
  if (!pipelining || (pipelining && m_wb.PC != 0x0))
    ++nInstrCompleted;

  if (m_wb.controlSignals.isRegWrite())
  {
    if (m_wb.controlSignals.isMemRead())
    {
      regfile.setWriteData(m_wb.memData);
    }
    else
    {
      regfile.setWriteData(m_wb.aluResult);
    }
    regfile.setWriteEnable(true);
    regfile.setRD(m_wb.rd);
  }
}

void WriteBackStage::clockPulse()
{
  regfile.clockPulse();
}
