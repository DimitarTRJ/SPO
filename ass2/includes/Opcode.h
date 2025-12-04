#ifndef OPCODE_H
#define OPCODE_H

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>

class Opcode{
public:
    static constexpr int ADD = 0x18;
    static constexpr int ADDF = 0x58;
    static constexpr int ADDR = 0x90;

    static constexpr int AND = 0x40;
    static constexpr int CLEAR = 0xB4;

    static constexpr int COMP = 0x28;
    static constexpr int COMPF = 0x88;
    static constexpr int COMPR = 0xA0;

    static constexpr int DIV = 0x24;
    static constexpr int DIVF = 0x64;
    static constexpr int DIVR = 0x9C;

    static constexpr int FIX = 0xC4;

    static constexpr int FLOAT = 0xC0;
    static constexpr int HIO = 0xF4;
    
    static constexpr int J = 0x3C;
    static constexpr int JEQ = 0x30;
    static constexpr int JGT = 0x34;
    static constexpr int JLT = 0x38;
    static constexpr int JSUB = 0x48;

    static constexpr int LDA = 0x00;
    static constexpr int LDB = 0x68;
    static constexpr int LDCH = 0x50;
    static constexpr int LDF = 0x70;
    static constexpr int LDL = 0x08;
    static constexpr int LDS = 0x6C;
    static constexpr int LDT = 0x74;
    static constexpr int LDX = 0x04;

    static constexpr int LPS = 0xD0;

    static constexpr int MUL = 0x20;
    static constexpr int MULF = 0x60;
    static constexpr int MULR = 0x98;

    static constexpr int NORM = 0xC8;

    static constexpr int OR = 0x44;
    static constexpr int RD = 0xD8;
    static constexpr int RMO = 0xAC;
    static constexpr int RSUB = 0x4C;

    static constexpr int SHIFTL = 0xA4;
    static constexpr int SHIFTR = 0xA8;

    static constexpr int SIO = 0xF0;
    static constexpr int SSK = 0xEC;

    static constexpr int STA = 0x0C;
    static constexpr int STB = 0x78;
    static constexpr int STCH = 0x54;
    static constexpr int STF = 0x80;
    static constexpr int STI = 0xD4;
    static constexpr int STL = 0x14;
    static constexpr int STS = 0x7C;
    static constexpr int STSW = 0xE8;
    static constexpr int STT = 0x84;
    static constexpr int STX = 0x10;

    static constexpr int SUB = 0x1C;
    static constexpr int SUBF = 0x5C;
    static constexpr int SUBR = 0x94;

    static constexpr int SVC = 0xB0;

    static constexpr int TD = 0xE0;
    static constexpr int WD = 0xDC;

    static constexpr int TIO = 0xF8;

    static constexpr int TIX = 0x2C;
    static constexpr int TIXR = 0xB8;

//----------- F1 ---------------------
    static bool isFormat1(int opcode)
    {
        switch (opcode & 0xFC) {
            case FIX:
            case FLOAT:
            case HIO:
            case NORM:
            case SIO:
            case TIO:
                return true;
            default:
                return false;
        }
    }

//------------ F2 ------------------
    static bool isFormat2(int opcode)
    {
        switch (opcode & 0xFC) {
            case ADDR:
            case CLEAR:
            case COMPR:
            case DIVR:
            case MULR:
            case RMO:
            case SHIFTL:
            case SHIFTR:
            case SUBR:
            case SVC:
            case TIXR:
                return true;
            default:
                return false;
        }
    }

//------------------ F3, F4 ------------------------
    static bool isFormat34(int opcode)
    {
        return !isFormat1(opcode) && !isFormat2(opcode);
    }

};

#endif