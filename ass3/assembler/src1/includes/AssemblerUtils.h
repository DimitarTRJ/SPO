#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

inline std::string toHex(uint32_t value, int width) {
    std::ostringstream oss;
    oss << std::uppercase << std::hex << std::setw(width)
        << std::setfill('0') << (value & ((1ULL << (width * 4)) - 1));
    return oss.str();
}

#endif
