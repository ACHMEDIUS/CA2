/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.h - Memory Control
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __MEMORY_CONTROL_H__
#define __MEMORY_CONTROL_H__

#include "memory-bus.h"

class ControlSignals
{
public:
  ControlSignals() : opcode(0), functionCode(0), immSel(0), aluOp(0), memWrite(0), memRead(0), regWrite(0), regDst(0), branch(0), jump(0) {}

  void setOpcode(uint8_t op) { opcode = op; }
  void setFunctionCode(uint8_t func) { functionCode = func; }
  void setImmSel(uint8_t sel) { immSel = sel; }
  void setAluOp(uint8_t op) { aluOp = op; }
  void setMemWrite(bool write) { memWrite = write; }
  void setMemRead(bool read) { memRead = read; }
  void setRegWrite(bool write) { regWrite = write; }
  void setRegDst(bool dst) { regDst = dst; }
  void setBranch(bool branch) { this->branch = branch; }
  void setJump(bool jump) { this->jump = jump; }

  uint8_t getOpcode() const { return opcode; }
  uint8_t getFunctionCode() const { return functionCode; }
  uint8_t getImmSel() const { return immSel; }
  uint8_t getAluOp() const { return aluOp; }
  bool getMemWrite() const { return memWrite; }
  bool getMemRead() const { return memRead; }
  bool getRegWrite() const { return regWrite; }
  bool getRegDst() const { return regDst; }
  bool getBranch() const { return branch; }
  bool getJump() const { return jump; }

private:
  uint8_t opcode;
  uint8_t functionCode;
  uint8_t immSel;
  uint8_t aluOp;
  bool memWrite;
  bool memRead;
  bool regWrite;
  bool regDst;
  bool branch;
  bool jump;
};

class InstructionMemory
{
  public:
    InstructionMemory(MemoryBus &bus);

    void     setSize(uint8_t size);
    void     setAddress(MemAddress addr);
    RegValue getValue() const;

  private:
    MemoryBus &bus;

    uint8_t    size;
    MemAddress addr;
};


class DataMemory
{
  public:
    DataMemory(MemoryBus &bus);

    void setSize(uint8_t size);
    void setAddress(MemAddress addr);
    void setDataIn(RegValue value);
    void setReadEnable(bool setting);
    void setWriteEnable(bool setting);

    RegValue getDataOut(bool signExtend) const;

    void clockPulse() const;


  private:
    MemoryBus &bus;

    uint8_t size{};
    MemAddress addr{};
    RegValue dataIn{};
    bool readEnable{};
    bool writeEnable{};
};


#endif /* __MEMORY_CONTROL_H__ */