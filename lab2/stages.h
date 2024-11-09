/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.h - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __STAGES_H__
#define __STAGES_H__

static const bool PRINT_DEBUG = 0;

#include "Adder.h"
#include "alu.h"
#include "ControlSignal.h"
#include "mux.h"
#include "inst-decoder.h"
#include "memory-control.h"

/* Pipeline registers may be read during propagate and may only be
 * written during clockPulse. Note that you cannot read the incoming
 * pipeline registers in clockPulse (e.g. in clockPulse of EX, you cannot
 * read ID_EX) because that register will already have been overwritten.
 * In case you need to propagate values from one pipeline register to
 * the next, these need to be buffered explicitly within the stage.
 */
struct IF_IDRegisters
{
  MemAddress PC{};
  // Set to NOP.
  uint32_t InstructionWord_ = 0b000010101000000000000000000000000;
  ControlSignal ControlSignal_{};
};

struct ID_EXRegisters
{
  MemAddress PC{};
  RegValue RS1_{};
  RegValue RS2_{};
  RegValue Immediate_{};
  ControlSignal ControlSignal_{};
};

struct EX_MRegisters
{
  MemAddress PC{};
  RegValue ALUOutput_{};
  RegValue Immediate_{};
  RegValue RS2_{};
  ControlSignal ControlSignal_{};
};

struct M_WBRegisters
{
  MemAddress PC{};
  RegValue ALUOutput_{};
  RegValue Immediate_{};
  RegValue DataMemoryOutput_{};
  ControlSignal ControlSignal_{};
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

/*
 * Instruction fetch
 */

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
                        IF_IDRegisters& if_id,
                        InstructionMemory instructionMemory,
                        MemAddress& PC,
                        Mux<MemAddress, PipelineProperties::EPcSelect> &pcSelect,
                        bool& isDelaySlot,
                        ControlSignal controlSignal)
    : Stage(pipelining),
      if_id(if_id),
      instructionMemory(instructionMemory),
      instructionWord(0),
      PC(PC),
      MuxPc_(pcSelect),
      IsDelaySlot_(isDelaySlot),
      IsAfterDelaySlot_(false),
      ControlSignal_(controlSignal)
  {
    // Set correct mux value for start of the program.
    MuxPc_.setSelector(PipelineProperties::EPcSelect::PC_Jump);
  }

  void propagate() override;
  void clockPulse() override;

private:
  IF_IDRegisters& if_id;

  InstructionMemory instructionMemory;
  uint32_t instructionWord;
  MemAddress& PC;
  Mux<MemAddress, PipelineProperties::EPcSelect>& MuxPc_;
  bool& IsDelaySlot_;
  bool IsAfterDelaySlot_;
  ControlSignal ControlSignal_;
};

/*
 * Instruction decode
 */

class InstructionDecodeStage : public Stage
{
public:
  InstructionDecodeStage(const bool pipelining,
                         const IF_IDRegisters& if_id,
                         ID_EXRegisters& id_ex,
                         RegisterFile& regfile,
                         InstructionDecoder& decoder,
                         ControlSignal controlSignal,
                         uint64_t& nInstrIssued,
                         uint64_t& nStalls,
                         Mux<MemAddress, PipelineProperties::EPcSelect> &pcSelect,
                         bool& flag,
                         bool& isDelaySlot,
                         bool debugMode = false)
    : Stage(pipelining),
      if_id(if_id), id_ex(id_ex),
      regfile(regfile), decoder(decoder),
      ControlSignal_(controlSignal),
      nInstrIssued(nInstrIssued), nStalls(nStalls),
      MuxPc_(pcSelect),
      Immediate_(0),
      Flag_(flag),
      IsDelaySlot_(isDelaySlot),
      debugMode(debugMode),
      AdderBranch_(), Adder_(),
      PC_(0)
  {
  }

  void propagate() override;
  void clockPulse() override;

private:
  const IF_IDRegisters& if_id;
  ID_EXRegisters& id_ex;

  RegisterFile& regfile;
  InstructionDecoder& decoder;
  ControlSignal ControlSignal_;

  uint64_t& nInstrIssued;
  uint64_t& nStalls;

  Mux<MemAddress, PipelineProperties::EPcSelect>& MuxPc_;
  RegValue Immediate_;
  bool& Flag_;
  bool& IsDelaySlot_;
  bool debugMode;
  // To compute branch target address.
  Adder AdderBranch_;
  Adder Adder_;
  MemAddress PC_;
};

/*
 * Execute
 */

class ExecuteStage : public Stage
{
public:
  ExecuteStage(bool pipelining,
               const ID_EXRegisters& id_ex,
               EX_MRegisters& ex_m,
               ControlSignal controlSignal,
               ALU& alu)
    : Stage(pipelining),
      id_ex(id_ex), ex_m(ex_m),
      ControlSignal_(controlSignal),
      Alu_(alu),
      Immediate_(0),
      RS2_(0),
      PC_(0)
  {
  }

  void propagate() override;
  void clockPulse() override;

private:
  const ID_EXRegisters& id_ex;
  EX_MRegisters& ex_m;
  ControlSignal ControlSignal_;
  ALU Alu_;
  RegValue Immediate_;
  RegValue RS2_;
  MemAddress PC_;
};

/*
 * Memory
 */

class MemoryStage : public Stage
{
public:
  MemoryStage(bool pipelining,
              const EX_MRegisters& ex_m,
              M_WBRegisters& m_wb,
              DataMemory dataMemory,
              ControlSignal controlSignal)
    : Stage(pipelining),
      ex_m(ex_m), m_wb(m_wb),
      dataMemory(dataMemory),
      ControlSignal_(controlSignal),
      ALUOutput_(0),
      Immediate_(0),
      RS2_(0),
      PC_(0)
  {  }

  void propagate() override;
  void clockPulse() override;

private:
  const EX_MRegisters& ex_m;
  M_WBRegisters& m_wb;

  DataMemory dataMemory;
  ControlSignal ControlSignal_;
  RegValue ALUOutput_;
  RegValue Immediate_;
  RegValue RS2_;
  MemAddress PC_;
};

/*
 * Write back
 */

class WriteBackStage : public Stage
{
public:
  WriteBackStage(bool pipelining,
                 const M_WBRegisters& m_wb,
                 RegisterFile& regfile,
                 uint64_t& nInstrCompleted,
                 ControlSignal controlSignal)
    : Stage(pipelining),
      m_wb(m_wb), regfile(regfile),
      RD_(0),
      nInstrCompleted(nInstrCompleted),
      ControlSignal_(controlSignal)
  {
  }

  void propagate() override;
  void clockPulse() override;

private:
  const M_WBRegisters& m_wb;

  RegisterFile& regfile;

  RegNumber RD_;

  uint64_t& nInstrCompleted;
  ControlSignal ControlSignal_;
};

#endif /* __STAGES_H__ */
