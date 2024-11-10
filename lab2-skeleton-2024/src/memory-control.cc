/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.cc - Memory Controller
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "memory-control.h"

InstructionMemory::InstructionMemory(MemoryBus &bus)
  : bus(bus), size(0), addr(0)
{
}

void
InstructionMemory::setSize(const uint8_t size)
{
  if (size != 2 and size != 4)
    throw IllegalAccess("Invalid size " + std::to_string(size));

  this->size = size;
}

void
InstructionMemory::setAddress(const MemAddress addr)
{
  this->addr = addr;
}

RegValue
InstructionMemory::getValue() const
{
  switch (size)
    {
      case 2:
        return bus.readHalfWord(addr);

      case 4:
        return bus.readWord(addr);

      default:
        throw IllegalAccess("Invalid size " + std::to_string(size));
    }
}


DataMemory::DataMemory(MemoryBus &bus)
  : bus{ bus }
{
}

void
DataMemory::setSize(const uint8_t size)
{
  /* TODO: check validity of size argument */

  this->size = size;
}

void
DataMemory::setAddress(const MemAddress addr)
{
  this->addr = addr;
}

void
DataMemory::setDataIn(const RegValue value)
{
  this->dataIn = value;
}

void
DataMemory::setReadEnable(bool setting)
{
  readEnable = setting;
}

void
DataMemory::setWriteEnable(bool setting)
{
  writeEnable = setting;
}

RegValue
DataMemory::getDataOut(bool signExtend) const
{
  /* TODO: implement */

  return 0;
}

void
DataMemory::clockPulse() const
{
  /* TODO: implement. Write the actual value to memory when required. */
}
