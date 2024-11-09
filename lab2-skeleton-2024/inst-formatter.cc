/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>
#include <iomanip>
#include <sstream>

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
    uint32_t opcode = static_cast<uint32_t>(decoder.getOpcode());
    uint32_t funct3 = decoder.getFunct3();
    uint32_t funct7 = decoder.getFunct7();
    RegNumber rd = decoder.getRD();
    RegNumber rs1 = decoder.getRS1();
    RegNumber rs2 = decoder.getRS2();

    std::string mnemonic;
    std::stringstream operands;

    try
    {
        switch (opcode)
        {
        case 0b0110011: // R-type instructions
            switch (funct3)
            {
            case 0x0:
                if (funct7 == 0x00)
                    mnemonic = "add";
                else if (funct7 == 0x20)
                    mnemonic = "sub";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            case 0x7:
                if (funct7 == 0x00)
                    mnemonic = "and";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            case 0x6:
                if (funct7 == 0x00)
                    mnemonic = "or";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            case 0x4:
                if (funct7 == 0x00)
                    mnemonic = "xor";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            case 0x1:
                if (funct7 == 0x00)
                    mnemonic = "sll";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            case 0x5:
                if (funct7 == 0x00)
                    mnemonic = "srl";
                else if (funct7 == 0x20)
                    mnemonic = "sra";
                else
                    throw IllegalInstruction("Unknown funct7 for R-type instruction");
                break;
            default:
                throw IllegalInstruction("Unknown funct3 for R-type instruction");
            }
            operands << "x" << static_cast<int>(rd) << ", x" << static_cast<int>(rs1) << ", x" << static_cast<int>(rs2);
            break;

        case 0b0010011: // I-type instructions
        {
            int32_t imm = decoder.getImmediate();
            switch (funct3)
            {
            case 0x0:
                mnemonic = "addi";
                break;
            case 0x7:
                mnemonic = "andi";
                break;
            case 0x6:
                mnemonic = "ori";
                break;
            case 0x4:
                mnemonic = "xori";
                break;
            case 0x1:
                if ((funct7 & 0xFE) == 0x00)
                    mnemonic = "slli";
                else
                    throw IllegalInstruction("Unknown funct7 for slli");
                break;
            case 0x5:
                if (funct7 == 0x00)
                    mnemonic = "srli";
                else if (funct7 == 0x20)
                    mnemonic = "srai";
                else
                    throw IllegalInstruction("Unknown funct7 for shift immediate");
                break;
            default:
                throw IllegalInstruction("Unknown funct3 for I-type instruction");
            }
            operands << "x" << static_cast<int>(rd) << ", x" << static_cast<int>(rs1) << ", " << static_cast<int>(imm);
            break;
        }

        case 0b0000011: // Load instructions
        {
            int32_t imm = decoder.getImmediate();
            switch (funct3)
            {
            case 0x0:
                mnemonic = "lb";
                break;
            case 0x1:
                mnemonic = "lh";
                break;
            case 0x2:
                mnemonic = "lw";
                break;
            case 0x3:
                mnemonic = "ld";
                break;
            case 0x4:
                mnemonic = "lbu";
                break;
            case 0x5:
                mnemonic = "lhu";
                break;
            case 0x6:
                mnemonic = "lwu";
                break;
            default:
                throw IllegalInstruction("Unknown funct3 for load instruction");
            }
            operands << "x" << static_cast<int>(rd) << ", " << static_cast<int>(imm) << "(x" << static_cast<int>(rs1) << ")";
            break;
        }

        case 0b0100011: // S-type instructions (Store)
        {
            int32_t imm = decoder.getImmediate();
            switch (funct3)
            {
            case 0x0:
                mnemonic = "sb";
                break;
            case 0x1:
                mnemonic = "sh";
                break;
            case 0x2:
                mnemonic = "sw";
                break;
            case 0x3:
                mnemonic = "sd";
                break;
            default:
                throw IllegalInstruction("Unknown funct3 for store instruction");
            }
            operands << "x" << static_cast<int>(rs2) << ", " << static_cast<int>(imm) << "(x" << static_cast<int>(rs1) << ")";
            break;
        }

        case 0b1100011: // B-type instructions (Branch)
        {
            int32_t imm = decoder.getImmediate();
            switch (funct3)
            {
            case 0x0:
                mnemonic = "beq";
                break;
            case 0x1:
                mnemonic = "bne";
                break;
            case 0x4:
                mnemonic = "blt";
                break;
            case 0x5:
                mnemonic = "bge";
                break;
            case 0x6:
                mnemonic = "bltu";
                break;
            case 0x7:
                mnemonic = "bgeu";
                break;
            default:
                throw IllegalInstruction("Unknown funct3 for branch instruction");
            }
            operands << "x" << static_cast<int>(rs1) << ", x" << static_cast<int>(rs2) << ", " << static_cast<int>(imm);
            break;
        }

        case 0b1101111: // J-type instruction (JAL)
        {
            int32_t imm = decoder.getImmediate();
            mnemonic = "jal";
            operands << "x" << rd << ", " << imm;
            break;
        }

        case 0b1100111: // I-type JALR
        {
            int32_t imm = decoder.getImmediate();
            mnemonic = "jalr";
            operands << "x" << static_cast<int>(rd) << ", " << static_cast<int>(imm) << "(x" << static_cast<int>(rs1) << ")";
            break;
        }

        case 0b0110111: // U-type instruction (LUI)
        {
            int32_t imm = decoder.getImmediate();
            mnemonic = "lui";
            operands << "x" << static_cast<int>(rd) << ", " << static_cast<int>(imm);
            break;
        }

        case 0b0010111: // U-type instruction (AUIPC)
        {
            int32_t imm = decoder.getImmediate();
            mnemonic = "auipc";
            operands << "x" << static_cast<int>(rd) << ", " << static_cast<int>(imm);
            break;
        }

        case 0b1110011: 
            if (funct3 == 0x0)
            {
                int32_t imm = decoder.getImmediate();
                if (imm == 0x000)
                    mnemonic = "ecall";
                else if (imm == 0x001)
                    mnemonic = "ebreak";
                else
                    throw IllegalInstruction("Unknown system instruction");
                operands << "";
            }
            else
                throw IllegalInstruction("Unknown funct3 for system instruction");
            break;

        default:
            throw IllegalInstruction("Unsupported opcode");
        }
    }
    catch (IllegalInstruction &e)
    {
        mnemonic = "illegal";
        operands.str(""); // Clear operands
    }

    os << std::setw(7) << std::left << mnemonic << " " << operands.str();

    return os;
}