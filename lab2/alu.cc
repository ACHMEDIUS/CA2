/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.cc - ALU component.
 *
 * Copyright (C) 2016, 2018 Leiden University, The Netherlands.
 */

#include "alu.h"

ALU::ALU()
    : A(), B(), op(ALUOp::NOP)
{
}

RegValue ALU::getResult()
{
    RegValue result = 0;

    switch (op)
    {
    case ALUOp::NOP:
        break;

    case ALUOp::ADD:
        result = add();
        break;

    case ALUOp::SUB:
        result = sub();
        break;

    case ALUOp::AND:
        result = bitwiseAnd();
        break;

    case ALUOp::OR:
        result = bitwiseOr();
        break;

    case ALUOp::XOR:
        result = bitwiseXor();
        break;

    case ALUOp::SLL:
        result = shiftLeftLogical();
        break;

    case ALUOp::SRL:
        result = shiftRightLogical();
        break;

    case ALUOp::SRA:
        result = shiftRightArithmetic();
        break;

    case ALUOp::SLT:
        result = setLessThan();
        break;

    case ALUOp::SLTU:
        result = setLessThanUnsigned();
        break;

    default:
        throw IllegalInstruction("Unimplemented or unknown ALU operation");
    }

    return result;
}

RegValue ALU::add()
{
    return A + B;
}

RegValue ALU::sub()
{
    return A - B;
}

RegValue ALU::bitwiseAnd()
{
    return A & B;
}

RegValue ALU::bitwiseOr()
{
    return A | B;
}

RegValue ALU::bitwiseXor()
{
    return A ^ B;
}

RegValue ALU::shiftLeftLogical()
{
    return A << (B % (sizeof(RegValue) * 8));
}

RegValue ALU::shiftRightLogical()
{
    return A >> (B % (sizeof(RegValue) * 8));
}

RegValue ALU::shiftRightArithmetic()
{
    return static_cast<RegValue>(static_cast<int32_t>(A) >> (B % (sizeof(RegValue) * 8)));
}

RegValue ALU::setLessThan()
{
    return static_cast<RegValue>(static_cast<int32_t>(A) < static_cast<int32_t>(B));
}

RegValue ALU::setLessThanUnsigned()
{
    return A < B;
}