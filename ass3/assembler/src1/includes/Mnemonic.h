#pragma once
#include <string>
#include <unordered_map>

struct MnemonicInfo {
    std::string name;
    uint8_t opcode;
    int format;   // 1, 2, 3/4
};

class Mnemonic {
public:
    static bool exists(const std::string& name);
    static const MnemonicInfo& get(const std::string& name);

private:
    static const std::unordered_map<std::string, MnemonicInfo> table;
};
