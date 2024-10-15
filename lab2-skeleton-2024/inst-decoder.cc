/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"

#include <map>


/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

void
InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

uint32_t
InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}

RegNumber
InstructionDecoder::getRS1() const
{
  /* TODO: implement */

  return 0;  /* result undefined */
}

RegNumber
InstructionDecoder::getRS2() const
{
  /* TODO: implement */

  return 0;  /* result undefined */
}

RegNumber
InstructionDecoder::getRD() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}
