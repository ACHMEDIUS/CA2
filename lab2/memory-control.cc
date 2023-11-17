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
  if (size != 2 && size != 4)
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
  if (size != 1 && size != 2 && size != 4 && size != 8) {
    throw IllegalAccess("Invalid size " + std::to_string(size));
  }

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
  if (!readEnable) {
    return 0;
  }

  switch (size) {
    case 1:
      return signExtend ? static_cast<int8_t>(bus.readByte(addr)) : bus.readByte(addr);

    case 2:
      return signExtend ? static_cast<int16_t>(bus.readHalfWord(addr)) : bus.readHalfWord(addr);

    case 4:
      return bus.readWord(addr);

    case 8:
      return bus.readDoubleWord(addr);

    default:
      throw IllegalAccess("Invalid size " + std::to_string(size));
  }
}

void
DataMemory::clockPulse() const
{
  if (writeEnable) {
    switch (size) {
      case 1:
        bus.writeByte(addr, static_cast<uint8_t>(dataIn));
        break;

      case 2:
        bus.writeHalfWord(addr, static_cast<uint16_t>(dataIn));
        break;

      case 4:
        bus.writeWord(addr, static_cast<uint32_t>(dataIn));
        break;

      case 8:
        bus.writeDoubleWord(addr, static_cast<uint64_t>(dataIn));
        break;

      default:
        throw IllegalAccess("Invalid size " + std::to_string(size));
    }
  }
}
