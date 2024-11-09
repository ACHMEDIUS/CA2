/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"
#include <iostream>

/*
 * Instruction fetch
 */

void InstructionFetchStage::propagate()
{
  try
  {
    instructionMemory.setAddress(PC);
    instructionMemory.setSize(4);
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
  uint32_t instructionWord = instructionMemory.getValue();

  if (instructionWord == TestEndMarker)
    throw TestEndMarkerEncountered(PC);

  if_id.PC = PC;
  if_id.instruction = instructionWord;

  // Update PC to point to the next instruction
  PC += 4;
}

/*
 * Instruction decode
 */

void dump_instruction(std::ostream &os, const uint32_t instructionWord,
                      const InstructionDecoder &decoder);

void InstructionDecodeStage::propagate()
{
  // Set instruction word on the instruction decoder
  instructionWord = if_id.instruction;
  decoder.setInstructionWord(instructionWord);

  // Generate control signals
  Opcode opcode = decoder.getOpcode();

  if (opcode == Opcode::REG) // R-type instruction
  {
    uint32_t funct3 = decoder.getFunct3();
    uint32_t funct7 = decoder.getFunct7();

    if (funct3 == 0x0)
    {
      if (funct7 == 0x00)
      {
        // ADD instruction
        aluOp = ALUOp::ADD;
      }
      else if (funct7 == 0x20)
      {
        // SUB instruction
        aluOp = ALUOp::SUB;
      }
      else
      {
        throw IllegalInstruction("Unsupported funct7 in R-type instruction");
      }
    }
    else
    {
      throw IllegalInstruction("Unsupported funct3 in R-type instruction");
    }
  }
  else
  {
    throw IllegalInstruction("Unsupported opcode");
  }

  // Get register numbers
  rs1 = decoder.getRS1();
  rs2 = decoder.getRS2();
  rd = decoder.getRD();

  // Register fetch
  regfile.setRS1(rs1);
  regfile.setRS2(rs2);

  // Get read data immediately
  regReadData1 = regfile.getReadData1();
  regReadData2 = regfile.getReadData2();

  // For R-type instructions, immediate value is not used
  immValue = 0;

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

  /* TODO: register fetch */
}

void InstructionDecodeStage::clockPulse()
{
  /* ignore the "instruction" in the first cycle. */
  if (!pipelining || (pipelining && PC != 0x0))
    ++nInstrIssued;

  id_ex.PC = PC;
  id_ex.regReadData1 = regReadData1;
  id_ex.regReadData2 = regReadData2;
  id_ex.instructionWord = instructionWord;
  id_ex.rs1 = rs1;
  id_ex.rs2 = rs2;
  id_ex.rd = rd;
  id_ex.aluOp = aluOp;
  id_ex.immValue = immValue;
}

/*
 * Execute
 */

void ExecuteStage::propagate()
{
  alu.setOp(id_ex.aluOp);
  alu.setA(id_ex.regReadData1);
  alu.setB(id_ex.regReadData2);

  PC = id_ex.PC;
}

void ExecuteStage::clockPulse()
{
  RegValue aluResult = alu.getResult();

  ex_m.PC = PC;
  ex_m.aluResult = aluResult;
  ex_m.rd = id_ex.rd;
  ex_m.memRead = false;
  ex_m.memWrite = false;
  ex_m.regReadData2 = id_ex.regReadData2;
}

/*
 * Memory
 */

void MemoryStage::propagate()
{
  PC = ex_m.PC;
}

void MemoryStage::clockPulse()
{
  m_wb.PC = PC;
  m_wb.result = ex_m.aluResult;
  m_wb.rd = ex_m.rd;
  m_wb.regWrite = true;
}

/*
 * Write back
 */

void WriteBackStage::propagate()
{
  if (!pipelining || (pipelining && m_wb.PC != 0x0))
    ++nInstrCompleted;

  if (m_wb.regWrite)
  {
    regfile.setRD(m_wb.rd);
    regfile.setWriteData(m_wb.result);
    regfile.setWriteEnable(true);
  }
  else
  {
    regfile.setWriteEnable(false);
  }
}

void WriteBackStage::clockPulse()
{
  regfile.clockPulse();
}
