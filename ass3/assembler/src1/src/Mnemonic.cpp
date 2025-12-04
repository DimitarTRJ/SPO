#include "Mnemonic.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <iostream>

// Static table
const std::unordered_map<std::string, MnemonicInfo> Mnemonic::table = {
    {"ADD", {"ADD", 0x18, 3}}, {"ADDF", {"ADDF", 0x58, 3}},
    {"ADDR", {"ADDR", 0x90, 2}}, {"AND", {"AND", 0x40, 3}},
    {"CLEAR", {"CLEAR", 0xB4, 2}}, {"COMP", {"COMP", 0x28, 3}},
    {"COMPF", {"COMPF", 0x88, 3}}, {"COMPR", {"COMPR", 0xA0, 2}},
    {"DIV", {"DIV", 0x24, 3}}, {"DIVF", {"DIVF", 0x64, 3}}, {"DIVR", {"DIVR", 0x9C, 2}},
    {"J", {"J", 0x3C, 3}}, {"JEQ", {"JEQ", 0x30, 3}}, {"JGT", {"JGT", 0x34, 3}}, {"JLT", {"JLT", 0x38, 3}},
    {"JSUB", {"JSUB", 0x48, 3}}, {"LDA", {"LDA", 0x00, 3}}, {"LDB", {"LDB", 0x68, 3}},
    {"LDCH", {"LDCH", 0x50, 3}}, {"LDF", {"LDF", 0x70, 3}}, {"LDL", {"LDL", 0x08, 3}},
    {"LDS", {"LDS", 0x6C, 3}}, {"LDT", {"LDT", 0x74, 3}}, {"LDX", {"LDX", 0x04, 3}},
    {"MUL", {"MUL", 0x20, 3}}, {"MULF", {"MULF", 0x60, 3}}, {"MULR", {"MULR", 0x98, 2}},
    {"OR", {"OR", 0x44, 3}}, {"RD", {"RD", 0xD8, 3}}, {"RSUB", {"RSUB", 0x4C, 3}},{"RMO" ,{"RMO", 0xAC, 2}},
    {"STA", {"STA", 0x0C, 3}}, {"STB", {"STB", 0x78, 3}}, {"STCH", {"STCH", 0x54, 3}},
    {"STF", {"STF", 0x80, 3}}, {"STI", {"STI", 0xD4, 3}}, {"STL", {"STL", 0x14, 3}},
    {"STS", {"STS", 0x7C, 3}}, {"STSW", {"STSW", 0xE8, 3}}, {"STT", {"STT", 0x84, 3}},
    {"STX", {"STX", 0x10, 3}}, {"SUB", {"SUB", 0x1C, 3}}, {"SUBF", {"SUBF", 0x5C, 3}},
    {"SUBR", {"SUBR", 0x94, 2}}, {"TD", {"TD", 0xE0, 3}}, {"TIX", {"TIX", 0x2C, 3}}, {"TIXR", {"TIXR", 0xB8, 2}},
    {"WD", {"WD", 0xDC, 3}}
};

bool Mnemonic::exists(const std::string& name) {
    std::string up = name;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    return table.find(up) != table.end();
}

const MnemonicInfo& Mnemonic::get(const std::string& name) {
    std::string up = name;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    auto it = table.find(up);
    if (it == table.end()) 
        throw std::runtime_error("Unknown mnemonic: " + name);
    return it->second;
}
