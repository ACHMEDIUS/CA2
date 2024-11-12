/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.cc - Memory Controller
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "memory-control.h"
#include "arch.h"
#include "memory-bus.h"
#include "memory-interface.h"

#include <cstdint>

InstructionMemory::InstructionMemory(MemoryBus &bus)
    : bus(bus), size(0), addr(0) {}

void InstructionMemory::setSize(const uint8_t size) {
  if (size != 2 and size != 4)
    throw IllegalAccess("Invalid size");

  this->size = size;
}

void InstructionMemory::setAddress(const MemAddress addr) { this->addr = addr; }

RegValue InstructionMemory::getValue() const {
  switch (size) {
  case 2:
    return bus.readHalfWord(addr);

  case 4:
    return bus.readWord(addr);

  default:
    throw IllegalAccess("Invalid size get");
  }
}

DataMemory::DataMemory(MemoryBus &bus) : bus{bus} {}

void DataMemory::setSize(const uint8_t size) {
  /* TODO: Done: check validity of size argument */
  if (size != 1 and size != 2 and size != 4 and size != 8)
    throw IllegalAccess("Invalid size set");

  this->size = size;
}

void DataMemory::setAddress(const MemAddress addr) { this->addr = addr; }

void DataMemory::setDataIn(const RegValue value) { this->dataIn = value; }

void DataMemory::setReadEnable(bool setting) { readEnable = setting; }

void DataMemory::setWriteEnable(bool setting) { writeEnable = setting; }

RegValue DataMemory::getDataOut(bool /*signExtend*/) const {
  /* TODO: check my implementation, do we need the default? */
  // TODO: Implt signExtend
  if (readEnable) {
    switch (size) {
    case 1:
      return bus.readByte(addr);
    case 2:
      return bus.readHalfWord(addr);
    case 4:
      return bus.readWord(addr);
    case 8:
      return bus.readDoubleWord(addr);
    }
  }

  return 0;
}

void DataMemory::clockPulse() const {
  /* TODO: Do we need the default? */
  if (writeEnable) {
    switch (size) {
    case 1:
      bus.writeByte(addr, dataIn);
      break;
    case 2:
      bus.writeHalfWord(addr, dataIn);
      break;
    case 4:
      bus.writeWord(addr, dataIn);
      break;
    case 8:
      bus.writeDoubleWord(addr, dataIn);
      break;
    }
  }
}
