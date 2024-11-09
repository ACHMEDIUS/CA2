/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.h - Memory Control
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __MEMORY_CONTROL_H__
#define __MEMORY_CONTROL_H__

#include "memory-bus.h"
#include "Enums.h"


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

    void clockPulse();


  private:
    MemoryBus &bus;

    uint8_t size{};
    MemAddress addr{};
    RegValue dataIn{};
    bool readEnable{};
    bool writeEnable{};
    uint8_t ReadByte_{}; 
    uint16_t ReadHalfWord_{};
    uint32_t ReadWord_{};
};


#endif /* __MEMORY_CONTROL_H__ */
