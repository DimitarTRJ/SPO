#ifndef MACHINE_H
#define MACHINE_H

#include "Device.h"
#include "OutputDevice.h"
#include "InputDevice.h"
#include "FileDevice.h"


#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <atomic>
#include <chrono>

class Machine{
public:
    enum Register{
        A = 0,
        X = 1,
        L = 2,
        B = 3,
        S = 4,
        T = 5,
        F = 6,

        PC = 8,
        SW = 9
    };

    Machine();

    int getA() const;
    void setA(int v);

    int getX() const;
    void setX(int v);

    int getL() const;
    void setL(int v);

    int getB() const;
    void setB(int v);

    int getS() const;
    void setS(int v);

    int getT() const;
    void setT(int v);

    double getF() const;
    void setF(double v);

    int getPC() const;
    void setPC(int v);

    int getSW() const;
    void setSW(int v);


    int getReg(int reg);
    void setReg(int reg, int val);

    enum Condition{
        CC_LT = 0x40,
        CC_EQ = 0x00,
        CC_GT = 0x80
    };

    void setCC(Condition c);
    Condition getCC() const;

    static constexpr int MAX_ADDRESS = 0xFFFFF;
    static constexpr int MEM_SIZE = MAX_ADDRESS + 1;

    int getByte(int addr) const;
    void setByte(int addr, int val);

    int getWord(int addr);
    void setWord(int addr, int val); 

    double getFloat(int addr);
    void setFloat(int addr, double val);

    static constexpr int NUM_DEVICES = 256;

    Device* getDevice(int num);
    void setDevice(int num, std::unique_ptr<Device> dev);

//-----------------------------------------------------
//=========== Obravnava tezav =========================
//-----------------------------------------------------

    void notImplemented(const std::string& mnemonic);
    void invalidOpcode(int opcode);
    void invalidAddressing();

//-----------------------------------------------------
//================= Izvajalnik=========================
//-----------------------------------------------------

    int fetch();
    void execute();
    bool execF1(int opcode);
    bool execF2(int opcode, int operand);
    bool execSICF3F4(int opcode, int ni, int operand, bool e);


//-----------------------------------------------------
//================= Clock =========================
//-----------------------------------------------------

   //........

//---------------------------------------------------------------------------
//=========================== Nalegalnik ====================================
//---------------------------------------------------------------------------

    bool loadSection(std::istream &r);


//=====================================================

    bool halted = false;

private:
    std::array<int, 10> intRegs;
    std::vector<uint8_t> memory;
    std::array<std::unique_ptr<Device>, NUM_DEVICES> devices;
    double Freg;

    void checkAddress(int addr) const;
    void checkRange(int addr, int size) const;

};

#endif