#pragma once
#include <cstdint>

/**
 * @brief A class to add 32 bit integers.
 */
class Adder
{
public:
  Adder() = default;
  void SetInputA(uint32_t inputA);
  void SetInputB(uint32_t inputB);
  void CalculateOutput();
  uint32_t Output_;

private:
  uint32_t InputA_;
  uint32_t InputB_;

  
};
