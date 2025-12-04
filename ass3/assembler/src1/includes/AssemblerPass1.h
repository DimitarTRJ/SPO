#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Literal {
    std::string name;
    std::string value;
    int length;
    int address = -1;
};

struct SourceLine {
    int lineNumber;
    int address;
    std::string label;
    std::string opcode;
    std::string operand;
    bool isComment = false;
};

class AssemblerPass1 {
public:
    AssemblerPass1();
    void run(std::vector<SourceLine>& lines);

    int evaluateExpression(const std::string& expr);

    int getStartAddress() const;
    int getProgramLength() const;
    const std::unordered_map<std::string, int>& getSymbolTable() const;
    const std::vector<Literal>& getLiterals() const;

    const std::string& getProgramName() const;  //

private:
    int locctr;
    int startAddress;
    int programLength;
    bool startSeen;

    int savedLocctr = -1;

    std::string programName = "NONAME";  //

    std::unordered_map<std::string, int> symbolTable;
    std::vector<Literal> literals;

    void handleDirective(SourceLine& line);
    void handleInstruction(SourceLine& line);
    void processLiteral(const std::string& operand);
    void assignLiterals(std::vector<SourceLine>& lines, size_t insertIndex);
};
