/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>


std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  /* TODO: write a textual representation of the decoded instruction
   * in "decoder" to the output stream "os".
   */

  return os;
}
