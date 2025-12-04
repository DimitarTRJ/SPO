#pragma once
#include "Lexer.h"
#include "AssemblerPass1.h"
#include <vector>
#include <string>
#include <cstdint>

class Parser {
public:
    explicit Parser(Lexer& lex) : lexer(lex) {}

    std::string parseLabel();
    std::string parseSymbol();
    int parseRegister();
    void parseComma();
    bool parseIndexed();
    int parseNumber(int lo, int hi);
    std::vector<uint8_t> parseData();

    SourceLine parseSourceLine();

    std::string readLine();
    void skipWhitespace();
    bool eof() const;
    bool eol() const;

private:
    Lexer& lexer;
};

