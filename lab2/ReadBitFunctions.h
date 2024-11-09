#pragma once
#include <cstdint>

namespace BitFieldOperations
{
  template <typename ReturnType>
  static inline ReturnType Bitmask(const uint32_t position)
  {
    return (1 << position) - 1;
  }

  /**
   * @brief Gets the value of a bitfield from a 32-bit integer.
   * @param instructionWord The 32-bit integer to read the bitfield from.
   * @param offset The bit number at which the preferred bitfield starts.
   * @param size The size of the bitfield in bits.
   * @return The value of the bitfield.
   * @note Use RegNumber as return type to get the register values without implicit conversions.
   */
  template <typename ReturnType>
  static inline ReturnType GetFieldValue(
    const uint32_t instructionWord,
    const uint32_t offset,
    const uint32_t size)
  {
    return (instructionWord >> offset) & Bitmask<ReturnType>(size);
  }

  /**
   * @brief SignExtends the provided immediate. optional provided length allows immediates with
   * intermediary sizes to successfully signextend as well (useful in sra operation)
   * bitslength maxvalue = 32git add
   * 
   */ 
   
  template<typename ImmediateType>
  static uint32_t SignExtend(
    const ImmediateType immediate,
    const uint32_t bitslength = sizeof(ImmediateType) * 8)
  {
    {
      // Zero extend the immediate.
      uint32_t zeroExtended = (uint32_t)immediate;
      if((bool)(zeroExtended>>(bitslength-1)))
      {
        // Make all bits within the length of the immediate 0 counting from the least significant bit
        // and the rest 1. And then return the or with the zeroextended immediate
        uint32_t neg = ((-1 << (bitslength-1))); 
        neg = ((neg) | zeroExtended); 
        return neg;
      }
      else
      {
        return zeroExtended;
      }
    }
  }

  /**

  // expects 8,16,32 bit immediates.
  template <typename ImmediateType>
  static uint32_t SignExtend(const ImmediateType immediate)
  {
    uint32_t size = sizeof(ImmediateType) * 8;
    switch (size)
    {
    // Byte.
    case 8:
      if (immediate < 128)
      {
        // No sign.
        return (uint32_t)immediate;
      }
    // sign extend to 32 bits
      return (uint32_t)immediate | 0xFFFFFF00;
    // Half-word.
    case 16:
      if (immediate < 32768)
      {
        // No sign.
        return (uint32_t)immediate;
      }
    // sign extend to 32 bits.
      return (uint32_t)immediate | 0xFFFF0000;
    // Word.
    case 32:
      // No need to sign extend, already 32 bits!
      return immediate;
    default:
      throw std::runtime_error("SignExtend: Invalid immediate size.");
    }
  }
  */
}

