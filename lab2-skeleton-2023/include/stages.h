/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.h - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __STAGES_H__
#define __STAGES_H__

#include "alu.h"
#include "helper.h"
#include "inst-decoder.h"
#include "memory-control.h"
#include "mux.h"

class Pipeline;

enum class Input { FIRST, SECOND, LAST };

/* Pipeline registers may be read during propagate and may only be
 * written during clockPulse. Note that you cannot read the incoming
 * pipeline registers in clockPulse (e.g. in clockPulse of EX, you cannot
 * read ID_EX) because that register will already have been overwritten.
 * In case you need to propagate values from one pipeline register to
 * the next, these need to be buffered explicitly within the stage.
 */
struct IF_IDRegisters {
  MemAddress PC = 0;

  /* TODO: add necessary fields */
  RegValue instructionWord{};
};

struct ID_EXRegisters {
  MemAddress PC{};

  /* TODO: add necessary fields */
  ALUOp aluOp{};
  RegValue A{};
  RegValue B{};
  RegValue imm{};
  RegNumber D{};
  RegValue valueToWrite;
};

struct EX_MRegisters {
  MemAddress PC{};

  /* TODO: add necessary fields */
  RegValue aluRes{};
  ALUOp aluOp{};
  RegNumber rd{};
  RegValue valueToWrite{};
};

struct M_WBRegisters {
  MemAddress PC{};

  /* TODO: add necessary fields */
  RegValue aluRes{};
  ALUOp aluOp{};
  RegNumber rd{};
  RegValue dataFromMemory{};
};

/*
 * Abstract base class for pipeline stage
 */

class Stage {
public:
  Stage(bool pipelining) : pipelining(pipelining) {}

  virtual ~Stage() {}

  virtual void propagate() = 0;
  virtual void clockPulse() = 0;

protected:
  bool pipelining;
};

/*
 * Instruction fetch
 */

class InstructionFetchFailure : public std::exception {
public:
  explicit InstructionFetchFailure(const MemAddress addr) {
    std::stringstream ss;
    ss << "Instruction fetch failed at address " << std::hex << addr;
    message = ss.str();
  }

  const char *what() const noexcept override { return message.c_str(); }

private:
  std::string message{};
};

class TestEndMarkerEncountered : public std::exception {
public:
  explicit TestEndMarkerEncountered(const MemAddress addr) {
    std::stringstream ss;
    ss << "Test end marker encountered at address " << std::hex << addr;
    message = ss.str();
  }

  const char *what() const noexcept override { return message.c_str(); }

private:
  std::string message{};
};

class InstructionFetchStage : public Stage {
public:
  InstructionFetchStage(bool pipelining, IF_IDRegisters &if_id,
                        InstructionMemory instructionMemory, MemAddress &PC,
                        Mux<MemAddress, Input> &pcMux)
      : Stage(pipelining), if_id(if_id), instructionMemory(instructionMemory),
        PC(PC), pcMux(pcMux) {}

  void propagate() override;
  void clockPulse() override;

private:
  IF_IDRegisters &if_id;

  InstructionMemory instructionMemory;
  MemAddress &PC;

  //
  RegValue instructionWord{};
  Mux<MemAddress, Input> &pcMux;
};

/*
 * Instruction decode
 */

class InstructionDecodeStage : public Stage {
public:
  InstructionDecodeStage(bool pipelining, const IF_IDRegisters &if_id,
                         ID_EXRegisters &id_ex, RegisterFile &regfile,
                         InstructionDecoder &decoder, uint64_t &nInstrIssued,
                         uint64_t &nStalls, CSC csc,
                         Mux<MemAddress, Input> &pcMux, bool &flag,
                         bool debugMode = false)
      : Stage(pipelining), if_id(if_id), id_ex(id_ex), regfile(regfile),
        decoder(decoder), nInstrIssued(nInstrIssued), nStalls(nStalls),
        debugMode(debugMode), csc(csc), pcMux(pcMux), flag(flag) {}

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

  /* TODO: add other necessary fields/buffers. */
  CSC csc;
  Mux<MemAddress, Input> &pcMux;
  bool &flag;
};

/*
 * Execute
 */

class ExecuteStage : public Stage {
public:
  ExecuteStage(bool pipelining, const ID_EXRegisters &id_ex,
               EX_MRegisters &ex_m)
      : Stage(pipelining), id_ex(id_ex), ex_m(ex_m) {}

  void propagate() override;
  void clockPulse() override;

private:
  const ID_EXRegisters &id_ex;
  EX_MRegisters &ex_m;

  MemAddress PC{};
  /* TODO: add other necessary fields/buffers and components (ALU anyone?) */
  ALU alu{};
  ALUOp aluOp{};
  RegNumber rd{};
  RegValue valueToWrite{};
};

/*
 * Memory
 */

class MemoryStage : public Stage {
public:
  MemoryStage(bool pipelining, const EX_MRegisters &ex_m, M_WBRegisters &m_wb,
              DataMemory dataMemory, Mux<MemAddress, Input> &pcMux)
      : Stage(pipelining), ex_m(ex_m), m_wb(m_wb), dataMemory(dataMemory),
        pcMux(pcMux) {}

  void propagate() override;
  void clockPulse() override;

private:
  const EX_MRegisters &ex_m;
  M_WBRegisters &m_wb;

  DataMemory dataMemory;

  MemAddress PC{};
  /* TODO: add other necessary fields/buffers */
  RegValue aluRes{};
  ALUOp aluOp{};
  RegNumber rd{};
  Mux<MemAddress, Input> &pcMux;
};

/*
 * Write back
 */

class WriteBackStage : public Stage {
public:
  WriteBackStage(bool pipelining, const M_WBRegisters &m_wb,
                 RegisterFile &regfile, bool &flag, uint64_t &nInstrCompleted,
                 Mux<RegValue, Input> &wbMux)
      : Stage(pipelining), m_wb(m_wb), regfile(regfile), flag(flag),
        nInstrCompleted(nInstrCompleted), wbMux(wbMux) {}

  void propagate() override;
  void clockPulse() override;

private:
  const M_WBRegisters &m_wb;

  RegisterFile &regfile;
  bool &flag;

  /* TODO add other necessary fields/buffers and components */

  uint64_t &nInstrCompleted;
  RegValue aluRes{};
  ALUOp aluOp{};
  RegNumber rd{};
  Mux<RegValue, Input> &wbMux;
};

#endif /* __STAGES_H__ */
