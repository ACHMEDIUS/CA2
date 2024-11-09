/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.cc - Memory Controller
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "memory-control.h"

#include "ReadBitFunctions.h"

InstructionMemory::InstructionMemory(MemoryBus& bus)
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


DataMemory::DataMemory(MemoryBus& bus)
  : bus{bus},
    ReadByte_(0),
    ReadHalfWord_(0),
    ReadWord_(0)
{
}

void
DataMemory::setSize(const uint8_t size)
{
  if (size != DataMemoryProperty::EDataSizes::Byte and
    size != DataMemoryProperty::EDataSizes::HalfWord and
    size != DataMemoryProperty::EDataSizes::Word)
  {
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
  if (!signExtend)
  {
    switch (size)
    {
    case DataMemoryProperty::EDataSizes::Byte:
      return ReadByte_;
    case DataMemoryProperty::EDataSizes::HalfWord:
      return ReadHalfWord_;
    case DataMemoryProperty::EDataSizes::Word:
      return ReadWord_;
    default:
      throw IllegalAccess("Invalid size " + std::to_string(size));
    }
  }
  else
  {
    switch (size)
    {
    case DataMemoryProperty::EDataSizes::Byte:
      return BitFieldOperations::SignExtend<uint8_t>(ReadByte_);
    case DataMemoryProperty::EDataSizes::HalfWord:
      return BitFieldOperations::SignExtend<uint16_t>(ReadHalfWord_);
    case DataMemoryProperty::EDataSizes::Word:
      return BitFieldOperations::SignExtend<uint32_t>(ReadWord_);
    default:
      throw IllegalAccess("Invalid size " + std::to_string(size));
    }
  }
}

void
DataMemory::clockPulse()
{
  if (writeEnable)
  {
    switch (size)
    {
      case DataMemoryProperty::EDataSizes::Byte:
        bus.writeByte(addr, dataIn);
        break;
      case DataMemoryProperty::EDataSizes::HalfWord:
        bus.writeHalfWord(addr, dataIn);
        break;
    case DataMemoryProperty::EDataSizes::Word:
      bus.writeWord(addr, dataIn);
      break;
      default:
        throw IllegalAccess("Invalid size");
    }
  }
  else if (readEnable)
  {
    switch (size)
    {
      case DataMemoryProperty::EDataSizes::Byte:
        ReadByte_ = bus.readByte(addr);
        break;
      case DataMemoryProperty::EDataSizes::HalfWord:
        ReadHalfWord_ = bus.readHalfWord(addr);
        break;
    case DataMemoryProperty::EDataSizes::Word:
      ReadWord_ = bus.readWord(addr);
      break;
      default:
        throw IllegalAccess("Invalid size");
    }
  }
}
