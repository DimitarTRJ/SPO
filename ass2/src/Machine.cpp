using namespace std;

#include "Machine.h"

#include "Device.h"
#include "OutputDevice.h"
#include "InputDevice.h"
#include "FileDevice.h"
#include "Opcode.h"
#include "Utils.h"


#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iomanip>


//---------- Machine -----------
    
    Machine::Machine() :  memory(MEM_SIZE, 0) {
        intRegs.fill(0);
        Freg = 0.0;

        //devices.fill(nullptr);
        for(auto &d : devices)
        {
            d = nullptr; 
        }

        devices[0] = std::make_unique<InputDevice>(std::cin);
        devices[1] = std::make_unique<OutputDevice>(std::cout);
        devices[2] = std::make_unique<OutputDevice>(std::cerr);

        for(int i = 3; i < NUM_DEVICES; i++)
        {
            std::string name = "dev" + std::to_string(i) + ".dat";
            devices[i] = std::make_unique<FileDevice>(name);
        }
       
    }


    //-------------Get and set methods-------------

    int Machine::getA() const { return intRegs[A];}
    void Machine::setA(int val) { intRegs[A] = val;}

    int Machine::getX() const { return intRegs[X];}
    void Machine::setX(int val) { intRegs[X] = val;}

    int Machine::getL() const { return intRegs[L];}
    void Machine::setL(int val) { intRegs[L] = val;}

    int Machine::getB() const { return intRegs[B];}
    void Machine::setB(int val) { intRegs[B] = val;}

    int Machine::getS() const { return intRegs[S];}
    void Machine::setS(int val) { intRegs[S] = val;}

    int Machine::getT() const { return intRegs[T];}
    void Machine::setT(int val) { intRegs[T] = val;}

    double Machine::getF() const { return Freg;}
    void Machine::setF(double val) { Freg = val;}

    int Machine::getPC() const { return intRegs[PC];}
    void Machine::setPC(int val) { intRegs[PC] = val;}

    int Machine::getSW() const { return intRegs[SW];}
    void Machine::setSW(int val) { intRegs[SW] = val;}


//-----------Default get and set methods-------------
    int Machine::getReg(int reg){
        if(reg < 0 || reg > 9 || reg == 7) 
        {
            throw std::out_of_range("Invalid register index");
        }
        else if(reg == F)
        {
            return static_cast<int>(Freg);
        }
        else
        {
            return intRegs[reg];
        }
    }

    void Machine::setReg(int reg, int val){
        if(reg < 0 || reg > 9 || reg == 7) 
        {
            throw std::out_of_range("Invalid register index");
        }
        else if(reg == F)
        {
            Freg = static_cast<double>(val);
        }
        else
        {
            intRegs[reg] = val;
        }
    }

//-----------------CC condition for SW---------------

    void Machine::setCC(Condition c) { intRegs[SW] = c;}
    Machine::Condition Machine::getCC() const { return static_cast<Condition>(intRegs[SW]);}


//------------------------------------------------------------
//======================== Memory ============================
//------------------------------------------------------------

//--------Bytes----------
    int Machine::getByte(int addr) const{
        checkAddress(addr);
        return memory[addr];
    }
    void Machine::setByte(int addr, int val){
        checkAddress(addr);                                 //.....
        memory[addr] = (uint8_t)(val & 0xFF);
    }

//---------Words-------------
    int Machine::getWord(int addr)
    {
        checkRange(addr, 3);
        return (memory[addr] << 16) | (memory[addr + 1] << 8) | (memory[addr + 2]);
    }
    void Machine::setWord(int addr, int val){
        checkRange(addr, 3);
        memory[addr] = (val >> 16) & 0xFF;
        memory[addr + 1] = (val >> 8) & 0xFF;
        memory[addr + 2] = val & 0xFF;
    }

//---------Floating Point-------------
    double Machine::getFloat(int addr){
        checkRange(addr, 6);

        uint8_t buffer[8] = {};           //zero 
        for (int i = 0; i < 6; i++)
        {
            buffer[i] = memory[addr + i];
        }

        double result;
        std::memcpy(&result, buffer, sizeof(result));
        return result;
    }
    void Machine::setFloat(int addr, double val){
        checkRange(addr, 6);

        uint8_t buffer[8];
        std::memcpy(buffer, &val, sizeof(val));
        for(int i = 0; i < 6; i++)
        {
            memory[addr + i] = buffer[i];
        }
    }



//------------------------------------------------------------
//========================= Devices ==========================
//------------------------------------------------------------
    Device* Machine::getDevice(int num) {
        if (num < 0 || num >= NUM_DEVICES)
        {
            throw std::out_of_range("Invalid device number");
        }
        else
        {
            return devices[num].get();
        }
    }

    void Machine::setDevice(int num, std::unique_ptr<Device> dev) {
        if (num < 0 || num >= NUM_DEVICES)
        {
            throw std::out_of_range("Invalid device number");    
        }
        else
        {
             devices[num] = std::move(dev);
        }
        
    }


//--------------------------------------------------------------------------------
//============================= Obravnava težav ==================================
//--------------------------------------------------------------------------------

    void Machine::notImplemented(const std::string& mnemonic){
        std::cerr << "[NOT IMPLEMENTED] Instruction " << mnemonic << "is not implemented." << endl;
    }

    void Machine::invalidOpcode(int opcode){
        std::cerr << "[INVALID OPCODE] Opcode " << opcode << "is not valid." << endl;
    }

    void Machine::invalidAddressing(){
        std::cerr << "[INVALID ADDRESSING] Illegal addressing mode was used." << endl; 
    }


//--------------------------------------------------------------------------
//======================= Izvajalnik =======================================
//--------------------------------------------------------------------------

    int Machine::fetch(){
        uint8_t byte = getByte(getPC());
        setPC(getPC() + 1);
        return byte;
    } 

    void Machine::execute(){
        int byte1 = fetch();
        int opcode = byte1 & 0xFC;

        if (Opcode::isFormat1(opcode)) 
        {
            execF1(opcode);
            return;
        }

        if (Opcode::isFormat2(opcode)) 
        {
            int byte2 = fetch();
            execF2(opcode, byte2);
            return;
        }

        int ni = byte1 & 0x03;
        int byte2 = fetch();
        bool e = (byte2 & 0x10) != 0;  
        int operand;

        if (!e) 
        {
            int byte3 = fetch();
            operand = (byte2 << 8) | byte3;
        }
        else 
        {
            int byte3 = fetch();
            int byte4 = fetch();
            operand = (byte2 << 16) | (byte3 << 8) | byte4;
        }

        execSICF3F4(opcode, ni, operand, e);
    }

    bool Machine::execF1(int opcode){
        switch(opcode)
        {
            case Opcode::FIX:
                setA(static_cast<int>(getF()));
                return true;
            case Opcode::FLOAT:
                setF(static_cast<double>(getA()));
                return true;
            //case Opcode::HIO:
            //case Opcode::NORM:
            //case Opcode::SIO:
            //case Opcode::TIO:
        return true;
        default:
            return false;
        }  
    }

    bool Machine::execF2(int opcode, int operand){
        int r1 = (operand & 0xF0) >> 4;
        int r2 = operand & 0x0F;

        switch(opcode)
        {
            case Opcode::ADDR:
                setReg(r2, getReg(r1) + getReg(r2));
                return true;
            case Opcode::CLEAR:
                setReg(r1, 0);
                return true;
            case Opcode::COMPR: {
                int diff = getReg(r1) - getReg(r2);
                setCC(diff < 0 ? CC_LT : (diff == 0 ? CC_EQ : CC_GT));
                return true;}
            case Opcode::DIVR:
                setReg(r2, getReg(r2) / getReg(r1));
                return true;
            case Opcode::MULR:
                setReg(r2, getReg(r2) * getReg(r1));
                return true;
            case Opcode::RMO:
                setReg(r2, getReg(r1));
                return true;
            case Opcode::SHIFTL:
                setReg(r1, getReg(r1) << r2);
                return true;
            case Opcode::SHIFTR:
                setReg(r1, getReg(r1) >> r2);
                return true;
            case Opcode::SUBR:
                setReg(r2, getReg(r2) - getReg(r1));
                return true;
            //case Opcode::SVC:
            case Opcode::TIXR: {
                int diff2 = getX() - getReg(r1);
                setCC(diff2 < 0 ? CC_LT : (diff2 == 0 ? CC_EQ : CC_GT));
                setX(getX() + 1);
                return true; }
            default:
                return false;
        }
    }

    bool Machine::execSICF3F4(int opcode, int ni, int operand, bool e){

        if (opcode == Opcode::RSUB) 
        {
            setPC(getL());
            halted = true;  
            return true;
        }
    
        int disp = e ? (operand & 0xFFFFF) : (operand & 0xFFF);

        bool x, b, p;

        if (e) 
        {
        // format 4: extract flags from bits 20-23
            x = (operand & 0x80000) != 0;
            b = (operand & 0x40000) != 0;
            p = (operand & 0x20000) != 0;
        } 
        else 
        {
        // format 3: extract flags from bits 12-15
            x = (operand & 0x8000) != 0;
            b = (operand & 0x4000) != 0;
            p = (operand & 0x2000) != 0;
        }

        // Compute effective address
        int EA = disp;

        if (!e) { // format 3
    // Sign-extend 12-bit displacement
            if (disp & 0x800) EA |= 0xFFFFF000; // negative displacement
        }

        if (p) EA += getPC();
        else if (b) EA += getReg(B);

        if (x) EA += getReg(X);

        int val = 0;
        if(ni == 0x0){}
        else if(ni == 0x1){ val = EA; } //immediate      (#100
        else if(ni == 0x3) //simple/direct      100
        {
            val = getWord(EA);
        }
        else if(ni == 0x2) //indirect       @100
        {
            //val = getWord(getWord(EA));
            val = getWord(EA);
            val = getWord(val);
        }


        switch(opcode)
        {
            case Opcode::ADD:   
                setA(getA() + val);
                return true;
            case Opcode::ADDF:
                setF(getF() + getFloat(EA));
                return true;
            case Opcode::AND:
                setA(getA() & getWord(EA));
                return true;
            case Opcode::COMP:  
                setCC(getA() - val < 0 ? CC_LT : getA() - val == 0 ? CC_EQ : CC_GT);
                return true;
            case Opcode::COMPF:
                setCC(getF() - getWord(EA) < 0 ? CC_LT : getF() - getWord(EA) == 0 ? CC_EQ : CC_GT);
                return true;
            case Opcode::DIV:   
                setA(getA() / val);
                return true;
            case Opcode::DIVF:
                setF(getF() / getFloat(EA));
                return true;

            case Opcode::J: 
                setPC(EA);
                return true;
            case Opcode::JEQ:   
                if (getCC() == CC_EQ) 
                    setPC(EA);
                return true;
            case Opcode::JGT:   
                if (getCC() == CC_GT) 
                    setPC(EA);
                return true;
            case Opcode::JLT:   
                if (getCC() == CC_LT) 
                    setPC(EA);
                return true;
            case Opcode::JSUB:  
                setL(getPC());
                setPC(EA);
                return true;


            case Opcode::LDA:   
                setA(val);
                return true;
            case Opcode::LDB:   
                setB(val);
                return true;
            case Opcode::LDCH:  
                setA((getA() & 0xFFFF00) | getByte(EA));
                return true;
            case Opcode::LDF:
                setF(getFloat(EA)); 
                return true;
            case Opcode::LDL:   
                setL(val);
                return true;
            case Opcode::LDS:
                setS(val);      
                return true;
            case Opcode::LDT:
                setT(val);      
                return true;
            case Opcode::LDX:
                setX(val);      
                return true;

            //case Opcode::LPS:
            case Opcode::MUL:   
                setA(getA() * val);
                return true;
            case Opcode::MULF:  
                setF(getF() * getFloat(EA));
                return true;
            case Opcode::OR:    
                setA(getA() | getWord(EA));
                return true;
            case Opcode::RSUB:  
                setPC(getL());
                return true;
            //case Opcode::SSK:

            case Opcode::STA:
                setWord(EA, getA());
                return true;
            case Opcode::STB:
                setWord(EA, getB());
                return true;
            case Opcode::STCH:
                setByte(EA, getA() & 0xFF);
                return true;
            case Opcode::STF:
                setFloat(EA, getF());
                return true;
            //case Opcode::STI:
            case Opcode::STL:
                setWord(EA, getL());
                return true;
            case Opcode::STS:
                setWord(EA, getS());
                return true;
            case Opcode::STSW:
                setWord(EA, getSW());
                return true;
            case Opcode::STT:
                setWord(EA, getT());
                return true;
            case Opcode::STX:
                setWord(EA, getX());
                return true;

            case Opcode::SUB:   
                setA(getA() - val);
                return true;
            case Opcode::SUBF:  
                setF(getF() - getFloat(EA));
                return true;
            case Opcode::TD:
                getDevice(EA)->test();
                return true;
            case Opcode::TIX:
                setCC(getX() - val < 0 ? CC_LT : getX() - val == 0 ? CC_EQ : CC_GT); 
                setX(getX() + 1);
                return true;
            case Opcode::WD:
                getDevice(EA)->write(getA() & 0xFF);
                return true;
            case Opcode::RD:
                setA(getDevice(EA)->read());
                return true;

            default:
                return false;
        }
    }


//---------------------------------------------------------------------------
//=========================== Nalegalnik ====================================
//---------------------------------------------------------------------------

bool Machine::loadSection(std::istream &r) {
    std::string line;
    while (std::getline(r, line)) {
        if (line.empty()) continue;  // skip empty lines

        char recType = line[0];
        std::cout << "recType ASCII=" << int(recType) << " char='" << recType << "'" << std::endl;

        if (recType == 'H') 
        {
            // Header record: H<name 6 chars><start 6 hex><length 6 hex>
            if (line.size() < 13) {
                std::cerr << "[LOAD ERROR] Header line too short" << std::endl;
                return false;
            }

            std::string name = line.substr(1, 6);
            int startAddr = std::stoi(line.substr(7, 6), nullptr, 16);
            int length = std::stoi(line.substr(13, 6), nullptr, 16);
        } 
        else if (recType == 'T') 
        {
            // Text record: T<start 6 hex><length 2 hex><object codes as hex>
            if (line.size() < 9) {
                std::cerr << "[LOAD ERROR] Text line too short" << std::endl;
                return false;
            }

            int startAddr = std::stoi(line.substr(1, 6), nullptr, 16);
            int length = std::stoi(line.substr(7, 2), nullptr, 16);
            std::string codes = line.substr(9);

            if ((int)codes.size() < length * 2) {
                std::cerr << "[LOAD ERROR] Not enough object code bytes in text record" << std::endl;
                return false;
            }

            for (int i = 0; i < length; ++i) {
                int byte = std::stoi(codes.substr(i * 2, 2), nullptr, 16);
                setByte(startAddr + i, byte);
            }
        } 
        else if (recType == 'E') 
        {
            // End record: E<start 6 hex>
            if (line.size() < 7) {
                std::cerr << "[LOAD ERROR] End line too short" << std::endl;
                return false;
            }

            int startAddr = std::stoi(line.substr(1, 6), nullptr, 16);
            setPC(startAddr);
            break;  // done loading
        } 
        else 
        {
            std::cerr << "[LOAD ERROR] Unknown record type: " << recType << std::endl;
            return false;
        }
    }

    return true;
}

//-------------------------------------------------------------------------
//================== Helper functions =====================================
//-------------------------------------------------------------------------

    // helpers for checking if the address is valid
    void Machine::checkAddress(int addr) const{
        if(addr < 0 || addr > MAX_ADDRESS)
        {
            throw std::out_of_range("Address outside memory!");
        }
    }

    void Machine::checkRange(int addr, int size) const{    //size in bytes
        if(addr < 0 || addr + size - 1 > MAX_ADDRESS)
        {
            throw std::out_of_range("Memory range outside bounds.");
        }
    }

