/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"

#include <stdexcept>
#include <cstdint>


/* TODO: add enums and constants necessary for your instruction decoder. */


/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
  public:
    explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
    { }

    explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
    { }
};


/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
  public:
    void                setInstructionWord(const uint32_t instructionWord);
    uint32_t            getInstructionWord() const;

    RegNumber           getRS1() const;
    RegNumber           getRS2() const;
    RegNumber           getRD() const;

    /* TODO: probably want methods to get opcode, function code */

    /* TODO: need a method to obtain the immediate */

  private:
    uint32_t instructionWord;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
