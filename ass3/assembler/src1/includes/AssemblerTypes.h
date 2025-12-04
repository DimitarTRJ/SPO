#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

#include <string>
#include <vector>
#include <unordered_map>

struct SourceLine {
    int lineNumber;
    int address;                // assigned in pass 1
    std::string label;
    std::string opcode;
    std::string operand;
    bool isComment = false;
};

struct Literal {
    std::string name;           // =X'05'
    std::string value;          // hex string
    int length;                 // bytes
    int address = -1;           // assigned at LTORG / END
};

#endif
