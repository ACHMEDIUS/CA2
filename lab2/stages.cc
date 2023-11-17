/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"
#include <iostream>

void InstructionFetchStage::propagate()
{

    try
    {
        instructionWord = instructionMemory->readWord(PC);
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
    if_id.npc = PC + 4;
}
/*
 * Instruction decode
 */

void dump_instruction(std::ostream &os, const uint32_t instructionWord,
                      const InstructionDecoder &decoder);

void InstructionDecodeStage::propagate()
{
    decoder.setInstruction(if_id.instructionWord);

    // Control signals
    ControlSignals controlSignals = ControlSignals::fromInstruction(if_id.instructionWord);

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

    /* Register fetch and other matters */
    id_ex.PC = PC;
    id_ex.readReg1 = registerFile->read(controlSignals.rs1, PC);
    id_ex.readReg2 = registerFile->read(controlSignals.rs2, PC);
    id_ex.imm = decoder.getImmediate();
    id_ex.rd = decoder.getRD();

    // PC
    if (decoder.getOpcode() == Opcode::JALR)
    {
        PC = id_ex.readReg1 + id_ex.imm;
    }
    else if (decoder.getOpcode() == Opcode::JAL)
    {
        PC += id_ex.imm;
    }
    else
    {
        PC += 4;
    }
}

void InstructionDecodeStage::clockPulse()
{
    /* ignore the "instruction" in the first cycle. */
    if (!pipelining || (pipelining && PC != 0x0))
        ++nInstrIssued;

    // Pipeline Register
    id_ex.PC = PC;
    id_ex.readReg1 = readReg1;
    id_ex.readReg2 = readReg2;
    id_ex.imm = imm;
    id_ex.rd = rd;
}

/*
 * Execute
 */

void ExecuteStage::propagate()
{

    // input based on control signals
    uint32_t input1 = id_ex.readReg1;
    uint32_t input2 = id_ex.readReg2;

    if (ControlSignals::ALUSrc::fromInstruction(id_ex.instructionWord) == ControlSignals::ALUSrc::IMM)
    {
        input2 = id_ex.imm;
    }

    // ALU operation
    uint32_t result = 0;

    switch (ControlSignals::ALUOp::fromInstruction(id_ex.instructionWord))
    {
        case ControlSignals::ALUOp::ADD:
            result = input1 + input2;
            break;
        case ControlSignals::ALUOp::SUB:
            result = input1 - input2;
            break;
        case ControlSignals::ALUOp::AND:
            result = input1 & input2;
            break;
        case ControlSignals::ALUOp::OR:
            result = input1 | input2;
            break;
        case ControlSignals::ALUOp::XOR:
            result = input1 ^ input2;
            break;
        case ControlSignals::ALUOp::SLT:
            result = ((int32_t)input1 < (int32_t)input2) ? 1 : 0;
            break;
        case ControlSignals::ALUOp::SLL:
            result = input1 << (input2 & 0x1F);
            break;
        case ControlSignals::ALUOp::SRL:
            result = input1 >> (input2 & 0x1F);
            break;
        case ControlSignals::ALUOp::SRA:
            result = (int32_t)input1 >> (input2 & 0x1F);
            break;
        default:
            break;
    }

    // Result
    ex_m.PC = PC;
    ex_m.result = result;
}

void ExecuteStage::clockPulse()
{
    // input based on control signals
    uint32_t input1 = id_ex.readReg1;
    uint32_t input2 = id_ex.readReg2;

    if (ControlSignals::ALUSrc::fromInstruction(id_ex.instructionWord) == ControlSignals::ALUSrc::IMM)
    {
        input2 = id_ex.imm;
    }

    // ALU operation
    uint32_t result = 0;

    switch (ControlSignals::ALUOp::fromInstruction(id_ex.instructionWord))
    {
        case ControlSignals::ALUOp::ADD:
            result = input1 + input2;
            break;
        case ControlSignals::ALUOp::SUB:
            result = input1 - input2;
            break;
        case ControlSignals::ALUOp::AND:
            result = input1 & input2;
            break;
        case ControlSignals::ALUOp::OR:
            result = input1 | input2;
            break;
        case ControlSignals::ALUOp::XOR:
            result = input1 ^ input2;
            break;
        case ControlSignals::ALUOp::SLT:
            result = ((int32_t)input1 < (int32_t)input2) ? 1 : 0;
            break;
        case ControlSignals::ALUOp::SLL:
            result = input1 << (input2 & 0x1F);
            break;
        case ControlSignals::ALUOp::SRL:
            result = input1 >> (input2 & 0x1F);
            break;
        case ControlSignals::ALUOp::SRA:
            result = (int32_t)input1 >> (input2 & 0x1F);
            break;
        default:
            break;
    }

    // Result
    ex_m.PC = PC;
    ex_m.result = result;

    // Memory operations
    if (ControlSignals::MemWrite::fromInstruction(id_ex.instructionWord) == ControlSignals::MemWrite::WRITE)
    {
        ex_m.effectiveAddress = result;
    }
    else if (ControlSignals::MemRead::fromInstruction(id_ex.instructionWord) == ControlSignals::MemRead::READ)
    {
        ex_m.effectiveAddress = result;
    }
}

/*
 * Memory
 */

void MemoryStage::propagate()
{
    // Data memory based on control signals
    if (ControlSignals::MemWrite::fromInstruction(ex_m.instructionWord) == ControlSignals::MemWrite::WRITE)
    {
        dataMemory.write(ex_m.effectiveAddress, ex_m.result);
    }
    else if (ControlSignals::MemRead::fromInstruction(ex_m.instructionWord) == ControlSignals::MemRead::READ)
    {
        m_wb.result = dataMemory.read(ex_m.effectiveAddress);
    }

    PC = ex_m.PC;
}

void MemoryStage::clockPulse()
{
    dataMemory.clockPulse();

    m_wb.PC = PC;
    m_wb.instructionWord = ex_m.instructionWord;
    m_wb.result = ex_m.result;

    // Data memory based on control signals
    if (ControlSignals::MemWrite::fromInstruction(ex_m.instructionWord) == ControlSignals::MemWrite::WRITE)
    {
        dataMemory.write(ex_m.effectiveAddress, ex_m.result);
    }
    else if (ControlSignals::MemRead::fromInstruction(ex_m.instructionWord) == ControlSignals::MemRead::READ)
    {
        m_wb.result = dataMemory.read(ex_m.effectiveAddress);
    }
}

/*
 * Write back
 */

void WriteBackStage::propagate()
{
    if (!pipelining || (pipelining && m_wb.PC != 0x0))
        ++nInstrCompleted;

    // Extract control signals from instruction word
    ControlSignals::RegWrite regWrite = ControlSignals::RegWrite::fromInstruction(m_wb.instructionWord);
    uint8_t writeReg = ControlSignals::WriteReg::fromInstruction(m_wb.instructionWord);
    uint32_t writeData = m_wb.result;

    // Write to register file if necessary
    if (regWrite == ControlSignals::RegWrite::WRITE)
    {
        regfile.write(writeReg, writeData);
    }
}

void WriteBackStage::clockPulse()
{
    regfile.clockPulse();
}
