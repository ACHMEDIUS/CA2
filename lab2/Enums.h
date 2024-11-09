#ifndef __Enums_H__
#define __Enums_H__

namespace Instruction
{
  // To differentiate between Register, PC, and immediate inputs.
  enum EAluInput
  {
    AluIn_Register = 0,
    AluIn_PC,
    AluIn_Immediate,
    AluIn_Nothing
  };
  

  // To differentiate between what kind of actions need to occur at the dataMemory with this operation.
  enum EDataMemoryBehaviour
  {
    Read = 0,
    Write,
    NoAction
  };

  // To differentiate between what kind of value needs to be stored at Write Back to RD.
  enum EWriteBackValueType
  {
    Wb_AluResult = 0,
    Wb_DataMemory,
    Wb_Immediate,
    Wb_R9ADD,
    Wb_Nothing
  };

  // To differentiate between the kind of branch instructions.
  enum EBranchType
  {
    BT_Immediate = 0,
    BT_Register,
    BT_SetsFlag,
    BT_JumpToImmediateOnFlag,
    BT_JumpToImmediateNoFlag,
    BT_Nothing
  };

  enum EFlagComparision
  {
    Flag_SignedLessOrEqual = 0,
    Flag_NotEqual,
    Flag_Equal,
    Flag_SignedGreaterOrEqual,
    Flag_Nothing
  };
}

namespace DataMemoryProperty
{
  enum EDataSizes
  {
    Byte = 1,
    HalfWord = 2,
    Word = 4
  };
  
  // To differentiate between the different addresses
  // that can be used to set the data memory address to.
  enum EMemoryAddressType
  {
    MemA_Pc = 0,
    MemA_AluResult,
    MemA_Nothing
  };

  // To differentiate between the values that can be
  // written back to data memory.
  enum EMemoryValueType
  {
    MemV_PC = 0,
    MemV_AluResult,
    MemV_RegB,
    MemV_Nothing
  };
}

// To select the different values in the MUX used for the PC.
namespace PipelineProperties
{
  enum class EPcSelect
  {
    PC_Jump = 0,
    PC_Increment,
    PC_Start,
    LAST
  };
}
#endif