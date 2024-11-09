#pragma once
#include "ControlSignal.h"

struct IF_IDRegisters;
struct ID_EXRegisters;
struct EX_MRegisters;
struct M_WBRegisters;
class InstructionDecodeStage;

enum EStallStages
{
  IF_Propagate = 0,
  IF_Pulse,
  ID
};

class HazardDetection
{
public:
  HazardDetection(
    IF_IDRegisters& if_id,
    ID_EXRegisters& id_ex,
    EX_MRegisters& ex_mem,
    M_WBRegisters& mem_wb);

  /**
   * @brief Sets the contolsignal with the new instruction that was just decoded in ID.
   * @param id_control_signal The control signal of the instruction that was just decoded in ID.
   * @note This signal is used to determine data hazards.
   */ 
  void SetIdControlSignal(const ControlSignal id_control_signal);

  void SetIDStagePc(const MemAddress id_Stage_Pc);

  bool CheckPipelineEmpty() const;

  /** @return Whether a hazard occurs and a stall is needed. */
  bool ShouldStall(const EStallStages stage) const;

  /** @return Whether a data hazard occurs. */
  bool DetectDataHazard() const;

  void SetAtEndProgram();

  inline bool GetIsAtEndProgram() const { return IsAtEndProgram_; }



private:
  IF_IDRegisters& If_Id_;
  ID_EXRegisters& Id_Ex_;
  EX_MRegisters& Ex_Mem_;
  M_WBRegisters& Mem_Wb_;
  ControlSignal ControlSignalID_{};

  bool IsAtEndProgram_;
  MemAddress IDStagePc_;
  
};
