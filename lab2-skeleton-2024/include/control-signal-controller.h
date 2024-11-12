#ifndef _CSC_H_
#define _CSC_H_

#include "alu.h"
#include "arch.h"
#include "reg-file.h"

class CSC
{
public:
    void getControlSignalsForInstr();

private:
    RegValue A = 0;
    RegValue B = 0;
};

#endif // _CSC_H_