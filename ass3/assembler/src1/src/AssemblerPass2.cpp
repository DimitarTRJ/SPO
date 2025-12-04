#include "AssemblerPass2.h"
#include "Mnemonic.h"
#include "Registers.h"
#include "AssemblerPass1.h" // for SourceLine
#include "AssemblerUtils.h" // for tohex
#include "Registers.h" // for parseregister
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <iostream>

AssemblerPass2::AssemblerPass2(const std::unordered_map<std::string, int>& symtab, int startAddr)
    : symbolTable(symtab), startAddress(startAddr), baseRegister(0), baseSet(false) {}

void AssemblerPass2::run(const std::vector<SourceLine>& lines) {
    objectLines.clear();
    modRecords.clear();

    for (const auto& line : lines) {
        if (line.isComment) continue;

        if (line.opcode == "BASE") {
            bool dummy;
            baseRegister = evaluateExpression(line.operand, dummy);
            baseSet = true;
            continue;
        }
        if (line.opcode == "NOBASE") {
            baseSet = false;
            baseRegister = 0;
            continue;
        }

        processLine(line);
    }
}

void AssemblerPass2::processLine(const SourceLine& line) {
    ObjectLine obj;
    obj.address = line.address;
    obj.needsModification = false;

    if (line.opcode == "BYTE") {
        if (line.operand[0] == 'C') {
            std::string data;
            for (char c : line.operand.substr(2, line.operand.length() - 3))
                data += toHex(int(c), 2);
            obj.objectCode = data;
        } else {
            obj.objectCode = line.operand.substr(2, line.operand.length() - 3);
        }
    }
    else if (line.opcode == "WORD") {
        bool sym;
        int val = evaluateExpression(line.operand, sym);
        obj.objectCode = toHex(static_cast<uint32_t>(val), 6); /////////////////////////  <----------------
    }
    else if (Mnemonic::exists(line.opcode[0] == '+' ? line.opcode.substr(1) : line.opcode)) {
        obj.objectCode = encodeInstruction(line, obj.needsModification);
    } else {
        return; // RESB/RESW/END/LTORG/BASE
    }

    if (!obj.objectCode.empty())
        objectLines.push_back(obj);
}

std::string AssemblerPass2::encodeInstruction(const SourceLine& line, bool& needsMod) {

    needsMod = false;       //////////////// <------------

    std::string op = line.opcode;
    bool extended = false;
    if (!op.empty() && op[0] == '+') {
        extended = true;
        op = op.substr(1);
    }

    const auto& m = Mnemonic::get(op);
    int format = extended ? 4 : m.format;

    // ----- RSUB special case -----
    if (op == "RSUB") {
        uint8_t opcodeByte = (m.opcode & 0xFC) | 0x3; // n=1, i=1
        uint32_t code = opcodeByte << 16;             // disp=0, flags=0
        return toHex(code, 6);
    }

    // ---------- FORMAT 1 ----------
    if (format == 1) return toHex(m.opcode, 2);

    // ---------- FORMAT 2 ----------
    if (format == 2) {
        auto regs = parseRegisters(line.operand);
        int code = (m.opcode << 8) | (regs[0] << 4);
        if (regs.size() == 2) code |= regs[1];
        return toHex(code, 4);
    }

    // ---------- FORMAT 3 / 4 ----------
    bool n = true, i = true, x = false, b = false, p = false, e = (format == 4);
    std::string operand = line.operand;

    bool immediateNumeric = false;
    if (!operand.empty()) {
        if (operand[0] == '#') {
            n = false; i = true;
            operand = operand.substr(1);
            if (!operand.empty() && std::isdigit(operand[0])) immediateNumeric = true;
        } else if (operand[0] == '@') {
            n = true; i = false;
            operand = operand.substr(1);
        }
    }

    // Indexed addressing
    size_t comma = operand.find(',');
    if (comma != std::string::npos) {
        std::string indexPart = operand.substr(comma + 1);
        indexPart.erase(0, indexPart.find_first_not_of(" \t"));
        indexPart.erase(indexPart.find_last_not_of(" \t") + 1);
        if (indexPart == "X") {
            x = true;
            operand = operand.substr(0, comma);
        }
    }

    // Trim
    operand.erase(0, operand.find_first_not_of(" \t"));
    operand.erase(operand.find_last_not_of(" \t") + 1);
                            
    bool symbolic = false;
    int target = 0;

    if (immediateNumeric) {
        target = std::stoi(operand);
    } else if (!operand.empty()) {
        target = evaluateExpression(operand, symbolic);
    }


    // Compute displacement
    int disp = 0;
    if (format == 4) {
        b = false;              //
        p = false;              //

        if (symbolic) 
        {
            disp = 0;               // ZERO for M record
            needsMod = true;
            modRecords.push_back({ line.address + 1, 5 });
        } else 
        {
            disp = target & 0xFFFFF; // numeric literal, 20bit address
        }
    } else { // format 3
        if (immediateNumeric) {
            disp = target;
            p = b = false;
        } else {
            int pc = line.address + 3;
            disp = target - pc;

            if (disp >= -2048 && disp <= 2047) {
                p = true; b = false;
            } else if (baseSet) {
                disp = target - baseRegister;
                if (disp < 0) disp += 4096; // 12-bit unsigned
                if (disp > 0xFFF)
                    throw std::runtime_error("BASE displacement out of range");
                b = true; p = false;
            } else {
                throw std::runtime_error("Displacement out of range for format 3 instruction. Use + for format 4.");
            }
        }
    }

    // ---------- Build code ----------
    uint32_t code = 0;
    if (format == 4) {
        uint8_t opcodeByte = (m.opcode & 0xFC) | (n << 1) | i;
        uint8_t flagsByte  = (x << 3) | (b << 2) | (p << 1) | e;
        code = (static_cast<uint32_t>(opcodeByte) << 24) |
               (static_cast<uint32_t>(flagsByte) << 20) |
               (disp & 0xFFFFF);
        return toHex(code, 8);
    } else {
        uint8_t opcodeByte = (m.opcode & 0xFC) | (n << 1) | i;
        uint8_t flagsByte  = (x << 3) | (b << 2) | (p << 1) | e;
        code = (static_cast<uint32_t>(opcodeByte) << 16) |
               (static_cast<uint32_t>(flagsByte) << 12) |
               (disp & 0xFFF);
        return toHex(code, 6);
    }
} 

int AssemblerPass2::evaluateExpression(const std::string& expr, bool& isSymbolic) {
    // Make a modifiable copy
    std::string s = expr;

    // Trim whitespace
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);

    isSymbolic = false;
    if (s.empty()) return 0;

    // Numeric constant
    if (std::isdigit(s[0])) return std::stoi(s);

    // Handle + and - offsets
    size_t plus = s.find('+');
    size_t minus = s.find('-');

    if (plus != std::string::npos) {
        std::string sym = s.substr(0, plus);
        int off = std::stoi(s.substr(plus + 1));
        if (!symbolTable.count(sym)) throw std::runtime_error("Undefined symbol: " + sym);
        isSymbolic = true;
        return symbolTable.at(sym) + off;
    }
    if (minus != std::string::npos) {
        std::string sym = s.substr(0, minus);
        int off = std::stoi(s.substr(minus + 1));
        if (!symbolTable.count(sym)) throw std::runtime_error("Undefined symbol: " + sym);
        isSymbolic = true;
        return symbolTable.at(sym) - off;
    }

    if (!symbolTable.count(s)) throw std::runtime_error("Undefined symbol: " + s);
    isSymbolic = true;
    return symbolTable.at(s);
}


const std::vector<ObjectLine>& AssemblerPass2::getObjectLines() const { return objectLines; }
const std::vector<ModificationRecord>& AssemblerPass2::getModRecords() const { return modRecords; }
