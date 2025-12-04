#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstdint>

class Utils {
public:
    // Reading a string of length len from the stream
    static std::string readString(std::istream &r, int len) {
        std::string result(len, '\0');
        r.read(&result[0], len);
        return result;
    }

    // Reading a single byte (0-255)
    static int readByte(std::istream &r) {
        char c;
        if (!r.get(c)) return -1; // EOF
        return static_cast<uint8_t>(c);
    }

    // Reading a 3-byte word (big-endian)
    static int readWord(std::istream &r) {
        int w = 0;
        for (int i = 0; i < 3; ++i) {
            int b = readByte(r);
            if (b == -1) return -1; // EOF or error
            w = (w << 8) | b;
        }
        return w;
    }
};

#endif