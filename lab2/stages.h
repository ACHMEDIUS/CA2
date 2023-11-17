/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.h - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __STAGES_H__
#define __STAGES_H__

#include "alu.h"
#include "mux.h"
#include "inst-decoder.h"
#include "memory-control.h"

struct IF_IDRegisters
{
  MemAddress PC = 0;
  uint32_t instruction = 0; 
};

struct ID_EXRegisters
{
  MemAddress PC{};
  uint32_t instruction = 0; 
  uint32_t rs1 = 0; 
  uint32_t rs2 = 0; 
  uint32_t imm = 0; 
  uint32_t rd = 0; 
  bool branch = false; 
  bool jump = false; 
  bool memRead = false; 
  bool memWrite = false; 
  bool regWrite = false; 
};

struct EX_MRegisters
{
  MemAddress PC{};
  uint32_t aluResult = 0; 
  uint32_t memAddress = 0; 
  uint32_t memData = 0; 
  bool branch = false; 
  bool jump = false; 
  bool memRead = false; 
  bool memWrite = false; 
  bool regWrite = false; 
};

struct M_WBRegisters
{
  MemAddress PC{};
  uint32_t memData = 0; 
  uint32_t aluResult = 0; 
  uint32_t rd = 0; 
  bool regWrite = false; 
};

class Stage
{
public:
    Stage(bool pipelining)
        : pipelining(pipelining)
    {
    }

    virtual ~Stage()
    {
    }

    virtual void propagate() = 0;
    virtual void clockPulse() = 0;

protected:
    bool pipelining;
};

 class InstructionFetchFailure : public std::exception
 {
   public:
     explicit InstructionFetchFailure(const MemAddress addr)
     {
       std::stringstream ss;
       ss << "Instruction fetch failed at address " << std::hex << addr;
       message = ss.str();
     }

     const char* what() const noexcept override
     {
       return message.c_str();
     }

   private:
     std::string message{};
 };

 class TestEndMarkerEncountered : public std::exception
 {
   public:
     explicit TestEndMarkerEncountered(const MemAddress addr)
     {
       std::stringstream ss;
       ss << "Test end marker encountered at address " << std::hex << addr;
       message = ss.str();
     }

     const char* what() const noexcept override
     {
       return message.c_str();
     }

   private:
     std::string message{};
 };


class InstructionFetchStage : public Stage
{
public:
    InstructionFetchStage(bool pipelining,
                          IF_IDRegisters &if_id,
                          InstructionMemory instructionMemory,
                          MemAddress &PC)
        : Stage(pipelining),
          if_id(if_id),
          instructionMemory(instructionMemory),
          PC(PC)
    {
    }

    void propagate() override;
    void clockPulse() override;

private:
    IF_IDRegisters &if_id;

    InstructionMemory instructionMemory;
    MemAddress &PC;
};

class InstructionDecodeStage : public Stage
{
public:
    InstructionDecodeStage(bool pipelining,
                           const IF_IDRegisters &if_id,
                           ID_EXRegisters &id_ex,
                           RegisterFile &regfile,
                           InstructionDecoder &decoder,
                           uint64_t &nInstrIssued,
                           uint64_t &nStalls,
                           bool debugMode = false)
        : Stage(pipelining),
          if_id(if_id), id_ex(id_ex),
          regfile(regfile), decoder(decoder),
          nInstrIssued(nInstrIssued), nStalls(nStalls),
          debugMode(debugMode)
    {
    }

    void propagate() override;
    void clockPulse() override;

private:
    const IF_IDRegisters &if_id;
    ID_EXRegisters &id_ex;

    RegisterFile &regfile;
    InstructionDecoder &decoder;

    uint64_t &nInstrIssued;
    uint64_t &nStalls;

    bool debugMode;

    MemAddress PC{};

    // Fields 
    uint32_t instruction{};
    uint32_t opcode{};
    uint32_t rs{};
    uint32_t rt{};
    uint32_t rd{};
    uint32_t shamt{};
    uint32_t funct{};
    uint32_t immediate{};
    uint32_t address{};
    bool isBranch{};
    bool isJump{};
    bool isLoad{};
    bool isStore{};
    bool isRType{};
    bool isIType{};
    bool isJType{};
    bool isSyscall{};
    bool isNop{};
    bool isHalt{};
    bool isTestEndMarker{};
    bool isInstructionFetchFailure{};
};

class ExecuteStage : public Stage
{
public:
    ExecuteStage(bool pipelining,
                 const ID_EXRegisters &id_ex,
                 EX_MRegisters &ex_m)
        : Stage(pipelining),
          id_ex(id_ex), ex_m(ex_m)
    {
    }

    void propagate() override;
    void clockPulse() override;

private:
    const ID_EXRegisters &id_ex;
    EX_MRegisters &ex_m;

    MemAddress PC{};

    // Fields 
    uint32_t aluOp{};
    uint32_t aluSrc1{};
    uint32_t aluSrc2{};
    uint32_t aluResult{};
    bool zeroFlag{};

    // ALU 
    ALU alu;
};

class MemoryStage : public Stage
{
public:
    MemoryStage(bool pipelining,
                const EX_MRegisters &ex_m,
                M_WBRegisters &m_wb,
                DataMemory dataMemory)
        : Stage(pipelining),
          ex_m(ex_m), m_wb(m_wb), dataMemory(dataMemory)
    {
    }

    void propagate() override;
    void clockPulse() override;

private:
    const EX_MRegisters &ex_m;
    M_WBRegisters &m_wb;

    DataMemory dataMemory;

    MemAddress PC{};

    // Fields 
    uint32_t aluResult{};
    uint32_t rt{};
    uint32_t rd{};
    uint32_t memData{};
    bool isLoad{};
    bool isStore{};

    // Buffers 
    uint32_t memAddr{};
    uint32_t writeData{};
    bool memWrite{};
    bool memRead{};
};

class WriteBackStage : public Stage
{
public:
    WriteBackStage(bool pipelining,
                   const M_WBRegisters &m_wb,
                   RegisterFile &regfile,
                   bool &flag,
                   uint64_t &nInstrCompleted)
        : Stage(pipelining),
          m_wb(m_wb), regfile(regfile), flag(flag),
          nInstrCompleted(nInstrCompleted)
    {
    }

    void propagate() override;
    void clockPulse() override;

private:
  const M_WBRegisters &m_wb;

  RegisterFile &regfile;
  bool &flag;

  // Fields 
  uint32_t writeData{};
  uint32_t rd{};
  bool isLoad{};
  bool isStore{};

  // Buffers
  bool memToReg{};
  bool regWrite{};
  uint32_t memData{};
  uint32_t aluResult{};
  uint64_t &nInstrCompleted;
};

#endif /* __STAGES_H__ */
