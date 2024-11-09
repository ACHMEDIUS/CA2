/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"

#include <map>


/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

void
InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

uint32_t
InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}

RegNumber InstructionDecoder::getRS1() const {
    return (instructionWord >> 15) & 0x1F;  // Bits 15-19
}

RegNumber InstructionDecoder::getRS2() const {
    return (instructionWord >> 20) & 0x1F;  // Bits 20-24
}

RegNumber InstructionDecoder::getRD() const {
    return (instructionWord >> 7) & 0x1F;  // Bits 7-11
}

Opcode InstructionDecoder::getOpcode() const {
    return static_cast<Opcode>(instructionWord & 0x7F);  // Bits 0-6
}

uint32_t InstructionDecoder::getFunct3() const {
    return (instructionWord >> 12) & 0x7;  // Bits 12-14
}

uint32_t InstructionDecoder::getFunct7() const {
    return (instructionWord >> 25) & 0x7F;  // Bits 25-31
}

int32_t InstructionDecoder::getImmediate() const {
    uint32_t opcode = instructionWord & 0x7F;
    int32_t imm = 0;

    switch (opcode) {
        case 0b0010011:  // I-type
            imm = (instructionWord >> 20) & 0xFFF;
            if (imm & 0x800) imm |= 0xFFFFF000;  // Sign-extend 12-bit imm
            break;
        case 0b0000011:  // Load
        case 0b1100111:  // JALR
            imm = (instructionWord >> 20) & 0xFFF;
            if (imm & 0x800) imm |= 0xFFFFF000;  // Sign-extend 12-bit imm
            break;
        case 0b1100011:  // B-type
            imm = ((instructionWord >> 7) & 0x1E) | ((instructionWord >> 20) & 0x7E0);
            imm |= ((instructionWord & 0x80) << 4) | ((instructionWord >> 19) & 0x1000);
            if (imm & 0x1000) imm |= 0xFFFFE000;  // Sign-extend 13-bit imm
            break;
        case 0b0100011:  // S-type
            imm = ((instructionWord >> 7) & 0x1F) | ((instructionWord >> 25) << 5);
            if (imm & 0x800) imm |= 0xFFFFF000;  // Sign-extend 12-bit imm
            break;
        case 0b1101111:  // J-type
            imm = ((instructionWord >> 12) & 0xFF) | ((instructionWord >> 20) & 0x800);
            imm |= ((instructionWord >> 21) & 0x3FF) << 1 | ((instructionWord >> 31) & 0x1) << 20;
            if (imm & 0x100000) imm |= 0xFFE00000;  // Sign-extend 21-bit imm
            break;
        default:
            throw IllegalInstruction("Unsupported opcode for immediate extraction");
    }

    return imm;
}
