/* rv64-emu -- Simple 32-bit OpenRISC simulator
 *
 *    InstructionBase.h - base class for instructions
 *
 * Copyright (C) 2022 Bas Blokzijl, Lucas Willemsens
 */
#pragma once
#include <cstdint>
#include <string>

#include "arch.h"
#include "Enums.h"

// forward declaration.
enum class EAluOp;

/**
 * @brief Contains all information about where the immediate is stored in the 32 bit instruction.
 * @struct {
 * @param uint8_t ImmediateOffset_ 
 * @param uint8_t ImmediateSize_
 * @param uint8_t ImmediateOffset2_ 
 * @param uint8_t ImmediateSize2_
 * @param bool NeedsSignExtension_  Whether this immediate is Sign Extended or Zero Extended.
 * }
 */
struct FImmediateProperties
{
  FImmediateProperties(
    const uint8_t immediateOffset = 0,
    const uint8_t immediateSize = 0,
    const uint8_t immediateOffset2 = 0,
    const uint8_t immediateSize2 = 0)
    : ImmediateOffset_(immediateOffset),
      ImmediateOffset2_(immediateOffset2),
      ImmediateSize_(immediateSize),
      ImmediateSize2_(immediateSize2),
      NeedsSignExtension_(true),
      ShiftAmount_(0)
  {
  }

  /** @return Whether the instruction uses an immediate. */
  inline bool NeedImmediate() const { return ImmediateSize_ > 0; }

  /** @return Whether the instruction uses an immediate separated over 2 fields. */
  inline bool IsMultiFieldImmediate() const { return ImmediateSize2_ > 0; }

  /** @return Whether this immediate needs Sign Extension or Zero Extension. */
  inline bool NeedsSignExtension() const { return NeedsSignExtension_; }

  // The bit offset in the 32 bit instruction to read the immediate value of this instruction.
  uint32_t ImmediateOffset_, ImmediateOffset2_;

  // The bit size of the immediate value of this instruction.
  uint32_t ImmediateSize_, ImmediateSize2_;

  // Whether this Immediate is Sign Extended or Zero Extended.
  bool NeedsSignExtension_;

  // The amount of bits to shift the immediate value after it has been read.
  unsigned int ShiftAmount_;
}; // FImmediateProperties

class InstructionBase
{
public:
  InstructionBase();
  ~InstructionBase();

  /**
   * @brief Determine the size and offset of the immediate of this instruction.
   * @return The size and offset of the immediate of this instruction in a struct.
   */
  FImmediateProperties GetImmediateProperties() const;

  /** @return The ALU operation in enum form. */
  EAluOp GetAluInstruction() const;

  /** @return What kind of action needs to occur at the dataMemory with this operation. */
  Instruction::EDataMemoryBehaviour GetDataMemoryBehaviour() const;

  /** @return The register number of the destination register. */
  inline RegNumber GetD() const { return RegD_; }

  /** @return The register number of the source register A. */
  inline RegNumber GetA() const { return RegA_; }

  /** @return The register number of the source register B. */
  inline RegNumber GetB() const { return RegB_; }

  /** @return Whether the source register A is used. */
  inline bool NeedA() const { return NeedA_; }

  /** @return Whether the source register B is used. */
  inline bool NeedB() const { return NeedB_; }

  /** @return Whether the destination register is used. */
  inline bool NeedD() const { return NeedD_; }

  bool NeedAlu() const;

  /** @brief The type of input needed at the ALU A input. */
  inline Instruction::EAluInput GetAluInputA() const { return AluInputA_; }

  /** @brief The type of input needed at the ALU B input. */
  inline Instruction::EAluInput GetAluInputB() const { return AluInputB_; }
  
  /** @return Whether this instruction requires a write-back to the RD register. */
  inline bool NeedWriteBackToRd() const { return ValueWriteBackToRD_ != Instruction::EWriteBackValueType::Wb_Nothing; }

  /** @return Whether this instruction requires a Sign-extension of data read from memory. */
  inline bool NeedSExtDataMemoryOutput() const { return SExtDataMemoryOutput; }

  /** @return What type of value is written back to RD. */
  inline Instruction::EWriteBackValueType GetValueTypeWriteBackRd() const { return ValueWriteBackToRD_; }

  /** @return Whether this instruction needs to use an Immediate value. */
  inline bool NeedImmediate() const { return ImmediateProperties_.NeedImmediate(); }

  /** @return The size that is used to either read or write from the data memory at the computed effective address. */
  inline DataMemoryProperty::EDataSizes GetDataMemorySize() const { return DataMemorySize_; }

  /** @return What type is used to compute the Effective Address. */
  inline DataMemoryProperty::EMemoryAddressType GetMemoryAddressType() const { return DataMemoryAddressType_; }

  /** @return The type of value that is written back to the data memory. */
  inline DataMemoryProperty::EMemoryValueType GetMemoryValueType() const { return DataMemStoreValueType_; }

  inline Instruction::EBranchType GetBranchType() const { return BranchType_; }

  /** @return The type of comparison used to determine the flag value. */ 
  inline Instruction::EFlagComparision GetFlagComparision() const { return FlagComparision_; }

  /** @return Whether this is a valid instruction derived from instruction base */
  inline bool GetIsValidInstruction() const { return !IsInvalid_; }

  /** @return Whether this instruction is a NOP instruction. */
  inline bool GetIsNopInstruction() const { return IsNopInstruction_; }


protected:
  EAluOp AluInstruction_;
  RegNumber RegD_;
  RegNumber RegA_;
  RegNumber RegB_;

  // Whether the instruction uses a destination register.
  bool NeedD_;

  // Whether the instruction uses source register A.
  bool NeedA_;

  // Whether the instruction uses source register B.
  bool NeedB_;

  //wheter the data read from memory needs sign extension false = zeroextension
  bool SExtDataMemoryOutput;

  // What kind of input we need for the A input at our ALU.
  Instruction::EAluInput AluInputA_;

  // What kind of input we need for the B input at our ALU.
  Instruction::EAluInput AluInputB_;

  // Where the value to write back comes from: Alu, DataMemory,...
  Instruction::EWriteBackValueType ValueWriteBackToRD_;

  // Contains the Immediate size and offset in the 32 bit instruction.
  FImmediateProperties ImmediateProperties_;

  // What kind of action needs to occur at the dataMemory with this operation.
  Instruction::EDataMemoryBehaviour DataMemoryBehaviour_;

  // The size that is used to either read or write from the data memory at the computed effective address.
  DataMemoryProperty::EDataSizes DataMemorySize_;

  // The type of address value that is used to set the address of the data memory. Pc, Alu, Immediate.
  DataMemoryProperty::EMemoryAddressType DataMemoryAddressType_;

  // The type of value that is stored in the data memory. PC, Alu, Immediate.
  DataMemoryProperty::EMemoryValueType DataMemStoreValueType_;

  // To determine to what value the new PC needs to be set.
  Instruction::EBranchType BranchType_;

  // What type of comparison is used to determine if the branch is taken.
  Instruction::EFlagComparision FlagComparision_;

  bool IsInvalid_;

  bool IsNopInstruction_;
  
};


class Nop : public InstructionBase
{
public:
  Nop();
};

class InvalidInstruction : public InstructionBase
{
public:
  InvalidInstruction();
};
