#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

// Register table
static const std::unordered_map<std::string, uint8_t> registerTable = {
    {"A",0},{"X",1},{"L",2},{"B",3},{"S",4},{"T",5},{"F",6},{"PC",8},{"SW",9}
};

// Returns register number for a single register string
inline uint8_t getRegisterNumber(const std::string& r) {
    auto it = registerTable.find(r);
    return (it != registerTable.end()) ? it->second : 0xFF;
}

inline std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t");
    size_t e = s.find_last_not_of(" \t");
    if (b == std::string::npos) return "";
    return s.substr(b, e - b + 1);
}

inline std::vector<int> parseRegisters(const std::string& operand) {
    std::vector<int> regs;
    size_t start = 0;

    while (start < operand.size()) {
        size_t end = operand.find(',', start);
        std::string token =
            (end == std::string::npos)
                ? operand.substr(start)
                : operand.substr(start, end - start);

        token = trim(token); 

        uint8_t reg = getRegisterNumber(token);
        if (reg == 0xFF)
            throw std::runtime_error("Invalid register: " + token);

        regs.push_back(reg);

        if (end == std::string::npos) break;
        start = end + 1;
    }

    return regs;
}



