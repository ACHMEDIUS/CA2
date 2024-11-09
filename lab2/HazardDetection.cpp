#include "HazardDetection.h"

#include "stages.h"

HazardDetection::HazardDetection(
  IF_IDRegisters& if_id,
  ID_EXRegisters& id_ex,
  EX_MRegisters& ex_mem,
  M_WBRegisters& mem_wb)
  : If_Id_(if_id),
    Id_Ex_(id_ex),
    Ex_Mem_(ex_mem),
    Mem_Wb_(mem_wb),
    IsAtEndProgram_(false),
    IDStagePc_(0)
{
}

void
HazardDetection::SetIdControlSignal(const ControlSignal id_control_signal)
{
  ControlSignalID_ = id_control_signal;
}

void
HazardDetection::SetIDStagePc(const MemAddress id_Stage_Pc)
{
  IDStagePc_ = id_Stage_Pc;
}

bool
HazardDetection::CheckPipelineEmpty() const
{
  if (If_Id_.ControlSignal_.GetIsStall() &&
    Id_Ex_.ControlSignal_.GetIsStall() &&
    Ex_Mem_.ControlSignal_.GetIsStall() &&
    Mem_Wb_.ControlSignal_.GetIsStall())
  {
    return true;
  }
  return false;
}

bool
HazardDetection::ShouldStall(const EStallStages stage) const
{
  switch (stage)
  {
  case EStallStages::IF_Propagate:
    return IsAtEndProgram_ && !CheckPipelineEmpty();
  case EStallStages::IF_Pulse:
    return DetectDataHazard() || (IsAtEndProgram_ && !CheckPipelineEmpty());
  case EStallStages::ID:
    return (IsAtEndProgram_ && (IDStagePc_ == 0)) || DetectDataHazard();
  }
  return false;
}

bool
HazardDetection::DetectDataHazard() const
{
  if (Id_Ex_.ControlSignal_.NeedWriteBackToRd())
  {
    if (ControlSignalID_.NeedA() && Id_Ex_.ControlSignal_.GetD() == ControlSignalID_.GetA())
    {
      return true;
    }
    if (ControlSignalID_.NeedB() && Id_Ex_.ControlSignal_.GetD() == ControlSignalID_.GetB())
    {
      return true;
    }
  }
  if (Ex_Mem_.ControlSignal_.NeedWriteBackToRd())
  {
    if (ControlSignalID_.NeedA() && Ex_Mem_.ControlSignal_.GetD() == ControlSignalID_.GetA())
    {
      return true;
    }
    if (ControlSignalID_.NeedB() && Ex_Mem_.ControlSignal_.GetD() == ControlSignalID_.GetB())
    {
      return true;
    }
  }
  if (Mem_Wb_.ControlSignal_.NeedWriteBackToRd())
  {
    if (ControlSignalID_.NeedA() && Mem_Wb_.ControlSignal_.GetD() == ControlSignalID_.GetA())
    {
      return true;
    }
    if (ControlSignalID_.NeedB() && Mem_Wb_.ControlSignal_.GetD() == ControlSignalID_.GetB())
    {
      return true;
    }
  }
  return false;
}

void
HazardDetection::SetAtEndProgram()
{
  IsAtEndProgram_ = true;
}
