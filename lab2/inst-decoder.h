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
#include "InstructionBase.h"
#include "ReadBitFunctions.h"

static const int INSTRUCTION_SIZE = 4;

struct FOffsets
{
  const uint32_t Opcode = 26;
  const uint32_t BigOpcode = 24;
  const uint32_t OpcodeF = 21;
  const uint32_t Opcode2 = 6;
  const uint32_t Opcode3 = 0;
  const uint32_t Opcode4 = 6;
  const uint32_t OpcodeD = 16;
  const uint32_t DReg = 21;
  const uint32_t AReg = 16;
  const uint32_t BReg = 11;
};

struct FSizes
{
  const uint32_t Opcode = 6;
  const uint32_t BigOpcode = 8;
  const uint32_t OpcodeF = 11;
  const uint32_t Opcode2 = 4;
  const uint32_t Opcode3 = 4;
  const uint32_t Opcode4 = 2;
  const uint32_t OpcodeD = 1;
  const uint32_t DReg = 5;
  const uint32_t AReg = 5;
  const uint32_t BReg = 5;
};

/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
public:
  explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
  {
  }

  explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
  {
  }
};

/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
  friend class InstructionBase;

public:
  InstructionDecoder();
  void SetInstructionWord(const uint32_t instructionWord);
  uint32_t GetInstructionWord() const;

  /**
   * @brief Generates the opcode and calls the instruction factory to create the instruction
   * corresponding to the opcode.
   * @return pointer to the instruction.
   */
  InstructionBase GetInstruction();

  RegValue GetImmediate(FImmediateProperties immediateProperties) const;

  uint32_t GetOpcode() const;
  RegNumber GetA() const;
  RegNumber GetB() const;
  RegNumber GetD() const;

private:
  uint32_t InstructionWord_;
  FSizes SizeInstructionParts_;
  FOffsets OffsetInstructionParts_;

  uint32_t GetOpcode2() const;
  uint32_t GetOpcode3() const;
  uint32_t GetOpcodeD() const;
  uint32_t GetOpcodeF() const;

  /**
   * @brief Base function in the instruction factory pattern.
   * Calls the correct factory depending on the opcode of the instruction.
   * @param opcode The opcode located in the most significant bit of the 32 bit instruction. (size = 6 bits)
   * @return The created instruction.
   */
  InstructionBase InstructionFactory(const uint32_t opcode);

  /**
   * @brief Constructs all RType instructions in factory pattern.
   * @param opcode2 The opcode located between bits 8 and 9 of the 32 bit instruction. (size = 2 bits)
   * @param opcode3 The opcode located between 0 and 3 of the 32 bit instruction. (size = 4 bits)
   * @return the created instruction.
   */
  InstructionBase RTypeFactory(const uint32_t opcode2, const uint32_t opcode3) const;

  /**
 * @brief Constructs all IType instructions in factory pattern.
 * @param opcode The opcode of the 32 bit instruction.
 * @return The created instruction.
 */
  InstructionBase ITypeFactory(const uint32_t opcode) const;

  /**
  * @brief Constructs all SType instructions in factory pattern.
  * @param opcode The opcode of the 32 bit instruction.
  * @return The created instruction.
  */
  InstructionBase STypeFactory(const uint32_t opcode) const;

  /**
  * @brief Constructs all DType instructions in factory pattern.
  * @param opcode The opcode of the 32 bit instruction.
  * @return The created instruction.
  */
  InstructionBase DTypeFactory(const uint32_t opcode, const uint32_t dOpcode) const;

  InstructionBase FTypeFactory(const uint32_t opcodeF) const;
  /**
  * @brief Constructs all JRegType instructions in factory pattern.
  * @param opcode The opcode of the 32 bit instruction.
  * @return The created instruction.
  */
  InstructionBase JRegTypeFactory(const uint32_t opcode) const;

  /**
  * @brief Constructs all JType instructions in factory pattern.
  * @param opcode The opcode of the 32 bit instruction.
  * @return The created instruction.
  */
  InstructionBase JTypeFactory(const uint32_t opcode) const;

  /**
   * @return The computed immediate over (possibly) multiple fields sign extended to 32 bits.
   * @param immediateProperties The properties of the immediate of the instruction.
   */
  RegValue GetImmediateSignExtended(FImmediateProperties immediateProperties) const;

  /**
   * @return The computed immediate over (possibly) multiple fields zero extended to 32 bits.
   * @param immediateProperties The properties of the immediate of the instruction.
   */
  RegValue GetImmediateZeroExtended(FImmediateProperties immediateProperties) const;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
