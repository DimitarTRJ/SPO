#include "AssemblerPass1.h"
#include "Mnemonic.h"
#include "AssemblerUtils.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <vector>

AssemblerPass1::AssemblerPass1() : locctr(0), startAddress(0), programLength(0), startSeen(false) {}

void AssemblerPass1::run(std::vector<SourceLine>& lines) {   
    locctr = 0;
    startSeen = false;

    for (size_t i = 0; i < lines.size(); ++i) 
    {
        SourceLine& line = lines[i];
        if (line.isComment) continue;

        // START directive: set start address and program name
        if (line.opcode == "START") 
        {
            if (startSeen) throw std::runtime_error("Duplicate START");
            startSeen = true;
            startAddress = std::stoi(line.operand, nullptr, 16);
            locctr = startAddress;
            line.address = locctr;

            if (!line.label.empty())
            {
                programName = line.label;
            }
            // Clear the opcode so it is not treated as an instruction
            line.opcode.clear();     
            continue;
        }

       
        // ORG directive
        if (line.opcode == "ORG") 
        {
            if (line.operand.empty())
                throw std::runtime_error("ORG requires an operand");

            if (line.operand == "*") 
            {
                if (savedLocctr == -1)
                    throw std::runtime_error("ORG * without previous ORG");

                locctr = savedLocctr;
                savedLocctr = -1;

            } else 
            {
                savedLocctr = locctr;

                // numeric?
                if (std::isdigit(line.operand[0])) 
                {
                    locctr = std::stoi(line.operand, nullptr, 10);
                }
                // symbol?
                else if (symbolTable.count(line.operand)) 
                {
                    locctr = symbolTable[line.operand];
                }
                else 
                {
                    throw std::runtime_error("Undefined symbol in ORG: " + line.operand);
                }
            }

            line.opcode.clear();
            continue;
        }


        // EQU directive
        if (line.opcode == "EQU") 
        {
            if (line.label.empty())
                throw std::runtime_error("EQU requires a label");

            int value = evaluateExpression(line.operand);

            if (symbolTable.count(line.label))
                throw std::runtime_error("Duplicate symbol: " + line.label);

            symbolTable[line.label] = value;
            line.opcode.clear();
            continue;
        }

        line.address = locctr;


        // Skip local labels starting with '.'
        if (!line.label.empty() && line.label[0] != '.') 
        {
            if (symbolTable.count(line.label))
                throw std::runtime_error("Duplicate symbol: " + line.label);
            symbolTable[line.label] = locctr;
        }

        // Literal detection
        if (!line.operand.empty() && line.operand[0] == '=') 
        {
            processLiteral(line.operand);
        }

        // Handle assembler directives
        if (!line.opcode.empty() && (line.opcode == "WORD" || line.opcode == "BYTE" || line.opcode == "RESW" || line.opcode == "RESB" )){
            handleDirective(line);
            continue;
        }

        if (!line.opcode.empty() && (line.opcode == "LTORG" || line.opcode == "END")){
            assignLiterals(lines, lines.size());
            break;
        }

        // BASE / NOBASE (Pass 1 ignores them)
        if (line.opcode == "BASE" || line.opcode == "NOBASE") {
            line.opcode.clear();
            continue;
        }

        // Only call handleInstruction if opcode is non-empty
        if (!line.opcode.empty())
            handleInstruction(line);
    }

    assignLiterals(lines, lines.size());
    programLength = locctr - startAddress;
}



void AssemblerPass1::handleDirective(SourceLine& line) {
    if (line.opcode == "WORD") locctr += 3;
    else if (line.opcode == "RESW") locctr += 3 * std::stoi(line.operand);
    else if (line.opcode == "RESB") locctr += std::stoi(line.operand);
    else if (line.opcode == "BYTE") 
    {
        if (line.operand[0] == 'C') locctr += line.operand.length() - 3;
        else if (line.operand[0] == 'X') locctr += (line.operand.length() - 3) / 2;
        else throw std::runtime_error("Invalid BYTE operand");
    }
    // BASE, NOBASE, LTORG, END have no LOCCTR effect
}

void AssemblerPass1::handleInstruction(SourceLine& line) {
    std::string op = line.opcode;
    bool extended = false;
    if (!op.empty() && op[0] == '+') { extended = true; op = op.substr(1); }

    if (!Mnemonic::exists(op))
        throw std::runtime_error("Invalid opcode: " + op);

    int format = Mnemonic::get(op).format;
    if (format == 1) locctr += 1;
    else if (format == 2) locctr += 2;
    else if (format == 3) locctr += extended ? 4 : 3;
}

void AssemblerPass1::processLiteral(const std::string& operand) {
    for (const auto& lit : literals) if (lit.name == operand) return;

    Literal l;
    l.name = operand;

    if (operand[1] == 'X') {
        l.value = operand.substr(3, operand.length() - 4);
        l.length = l.value.length() / 2;
    } else if (operand[1] == 'C') {
        std::ostringstream oss;
        for (char c : operand.substr(3, operand.length() - 4))
            oss << std::hex << std::uppercase << int(c);
        l.value = oss.str();
        l.length = l.value.length() / 2;
    } else throw std::runtime_error("Invalid literal: " + operand);

    literals.push_back(l);
}

void AssemblerPass1::assignLiterals(std::vector<SourceLine>& lines, size_t insertIndex) {
    for (auto& lit : literals) {
        if (lit.address != -1) continue;
        lit.address = locctr;

        SourceLine litLine;
        litLine.lineNumber = -1;
        litLine.address = locctr;
        litLine.label = "*";
        litLine.opcode = "BYTE";
        litLine.operand = "X'" + lit.value + "'";

        lines.insert(lines.begin() + insertIndex, litLine);
        insertIndex++;
        locctr += lit.length;
    }
}

int AssemblerPass1::getStartAddress() const { return startAddress; }
int AssemblerPass1::getProgramLength() const { return programLength; }
const std::unordered_map<std::string, int>& AssemblerPass1::getSymbolTable() const { return symbolTable; }
const std::vector<Literal>& AssemblerPass1::getLiterals() const { return literals; }

const std::string& AssemblerPass1::getProgramName() const {
    return programName;
}

// New helper for evaluating simple EQU expressions in Pass1 
int AssemblerPass1::evaluateExpression(const std::string& expr) {
    std::string s = expr;
    // trim whitespace
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);

    if (s.empty()) return 0;

    // current location
    if (s == "*") return locctr;

    // handle + or - in expression
    size_t plus = s.find('+');
    size_t minus = s.find('-');

    if (plus != std::string::npos) {
        std::string sym = s.substr(0, plus);
        int offset = std::stoi(s.substr(plus + 1));
        if (!symbolTable.count(sym)) throw std::runtime_error("Undefined symbol: " + sym);
        return symbolTable.at(sym) + offset;
    }

    if (minus != std::string::npos) {
        std::string sym = s.substr(0, minus);
        int offset = std::stoi(s.substr(minus + 1));
        if (!symbolTable.count(sym)) throw std::runtime_error("Undefined symbol: " + sym);
        return symbolTable.at(sym) - offset;
    }

    // single symbol or numeric constant
    if (std::isdigit(s[0])) return std::stoi(s);
    if (symbolTable.count(s)) return symbolTable.at(s);

    throw std::runtime_error("Undefined symbol in EQU: " + s);
}




