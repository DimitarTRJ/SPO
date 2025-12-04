#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct ObjectLine {
    int address;
    std::string objectCode;
    bool needsModification;
};

struct ModificationRecord {
    int address;
    int length; // half-bytes
};

struct SourceLine;

class AssemblerPass2 {
public:
    AssemblerPass2(const std::unordered_map<std::string, int>& symtab, int startAddr);
    void run(const std::vector<SourceLine>& lines);

    const std::vector<ObjectLine>& getObjectLines() const;
    const std::vector<ModificationRecord>& getModRecords() const;

private:
    const std::unordered_map<std::string, int>& symbolTable;
    int startAddress;
    int baseRegister;
    bool baseSet;

    std::vector<ObjectLine> objectLines;
    std::vector<ModificationRecord> modRecords;

    void processLine(const SourceLine& line);
    std::string encodeInstruction(const SourceLine& line, bool& needsMod);
    int evaluateExpression(const std::string& expr, bool& isSymbolic);
};
