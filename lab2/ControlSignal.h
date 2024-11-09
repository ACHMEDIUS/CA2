#pragma once
#include "Adder.h"
#include "InstructionBase.h"

// forward declaration
enum class EAluOp;
struct ID_EXRegisters;
struct EX_MRegisters;
struct M_WBRegisters;

class ControlSignal
{
public:
  ControlSignal();

  void SetIsStall();
  
  /** @return The instruction that the ALU needs to execute. */
  EAluOp GetAluInstruction() const;

  /**
   * @param instruction The instruction used by this control signal class.
   */
  void SetInstruction(InstructionBase instruction);

  /** @return The register number of the source register A */
  RegNumber GetA() const;

  /** @return The register number of the source register B */
  RegNumber GetB() const;

  /** @return The register number of the destination register. */
  RegNumber GetD() const;

  /** @return Whether the source register A is used. */
  bool NeedA() const;

  /** @return Whether the source register B is used. */
  bool NeedB() const;

  /** @return Whether the destination register is used. */
  bool NeedD() const;

  /** @return Whether the ALU needs to execute an operation. */
  bool NeedAlu() const;

  /**
   * @brief Checks which input the ALU needs for field A.
   * @return Either the value of source register A,
   * The immediate value or the value of the PC.
   */
  RegValue GetAluInputA(const ID_EXRegisters* pipelineRegs) const;

  /**
   * @brief Checks which input the ALU needs for field A.
   * @return Either the value of source register A,
   * The immediate value or the value of the PC.
   */
  RegValue GetAluInputB(const ID_EXRegisters* pipelineRegs) const;
  
  /** @return Whether the instruction requires a write-back to the RD register. */
  bool NeedWriteBackToRd() const;

  /** @return Whether this instruction requires an Immediate value. */
  bool NeedImmediate() const;

  /** @return Struct with information about the immediate offset and its size. */
  FImmediateProperties GetImmediateProperties() const;

  /** @return Whether the instruction needs to read from data memory. */
  bool NeedReadDataMemory() const;

  /** @return Whether the instruction needs to write to data memory. */
  bool NeedWriteDataMemory() const;

  
  /** @return Whether the instruction needs to sign-extend the value read from data memory. */
  bool NeedSExtDataMemoryOutput() const;

  /** @return Whether the size used to read/write at the computed effective address in memory data . */
  DataMemoryProperty::EDataSizes GetDataMemorySize() const;

  /**
   * @brief Gets the correct value to write back to the RD register depending on the instruction.
   * @param pipelineRegs The pipeline registers between the Mem/WB stages.
   * @return 32 bit value to write back to the RD register.
   */
  RegValue GetWriteBackValueRD(const M_WBRegisters* pipelineRegs) const;

  /** @return The type of writeback that the instruction uses, needed to determine whether we write back to RD or R9 */
  Instruction::EWriteBackValueType GetWriteBackType() const;

  /**
   * @brief Gets the correct value of the address in memory where the instruction needs to
   * write data to.
   * @param pipelineRegs The pipeline registers between the EX/MEM stages.
   * @return 32 bit memory address.
   */ 
  MemAddress GetMemAddress(const EX_MRegisters* pipelineRegs) const;

  /**
   * @brief Gets the correct value to write back to Data Memory depending on the instruction.
   * @param pipelineRegs The pipeline registers between the EX/Mem stages.
   * @return 32 bit value to write back to Data Memory.
   */
  RegValue GetMemWriteValue(const EX_MRegisters* pipelineRegs) const;

  /** @return The type of branch of this instruction. */
  Instruction::EBranchType GetBranchType() const;

  /** @return Whether this instruction is of a branch type. */
  bool GetIsBranch() const;
  
  /**
   * @brief Marks the instruction of this control signal to be a delay instruction.
   * @param isDelayInstruction Whether the instruction is a delay instruction.
   */
  void SetIsDelayInstruction(const bool isDelayInstruction);
  
  /** @retrurn Whether this instruction is inside a delay-slot. */
  bool GetIsDelayInstruction() const;

  void SetBranchAdderValues(Adder* adder, MemAddress Pc, uint32_t immediate) const;

  /**
   * @brief Sets the flag value depending on the used comparision and register values.
   * @param RS1 The value of the RS1 register.
   * @param RS2 The value of the RS2 register.
   */ 
  bool GetFlagValue(RegValue RS1, RegValue RS2) const;

  /** @return Whether this instruction is a NOP instruction. */
  bool GetIsNop() const;

  /**  @return Whether this instruction is inside a delay-slot. */
  bool GetUsesDelaySlot() const;

private:
  // Decoded instruction from the 32 bits.
  InstructionBase Instruction_;

  // Whether this instruction is inside a delay slot.
  bool IsDelayInstruction_;
};
