#include "Adder.h"

void Adder::SetInputA(uint32_t inputA)
{
  InputA_ = inputA;
}

void Adder::SetInputB(uint32_t inputB)
{
  InputB_ = inputB;
}

void Adder::CalculateOutput()
{
  Output_ = InputA_ + InputB_;
}

