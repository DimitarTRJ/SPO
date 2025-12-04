#pragma once
#include <string>

class Lexer {
public:
    explicit Lexer(const std::string& src) 
        : source(src), pos(0), col(1) {}

    char peek() const {
        return pos < source.size() ? source[pos] : '\0';
    }

    char advance() {
        if (pos < source.size()) {
            char c = source[pos++];
            if (c == '\n') col = 1;
            else ++col;
            return c;
        }
        return '\0';
    }

    bool advanceIf(char expected) {
        if (peek() == expected) {
            advance();
            return true;
        }
        return false;
    }

    void skipWhitespace() {
        while (pos < source.size() && (source[pos] == ' ' || source[pos] == '\t'))
            advance();
    }

    std::string readAlphanumeric() {
        std::string s;
        while (pos < source.size() && (isalnum(source[pos]) || source[pos] == '_'))
            s += advance();
        return s;
    }

    std::string readDigits() {
        std::string s;
        while (pos < source.size() && isdigit(source[pos]))
            s += advance();
        return s;
    }

    std::string readTo(char delimiter) {
        std::string s;
        while (pos < source.size() && source[pos] != delimiter)
            s += advance();
        return s;
    }

    int getCol() const { return col; }
    bool eof() const { return pos >= source.size(); }
    bool eol() const { return peek() == '\n' || eof(); }

private:
    const std::string& source;
    size_t pos;
    int col;
};

