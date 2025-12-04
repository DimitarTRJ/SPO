#include "Parser.h"
#include "Mnemonic.h"
#include <stdexcept>
#include <cctype>
#include <string>
#include <sstream>
#include <unordered_set>


std::string Parser::parseLabel() {
    if (lexer.getCol() == 1 && std::isalpha(lexer.peek()))
        return lexer.readAlphanumeric();
    return "";
}

std::string Parser::parseSymbol() {
    return lexer.readAlphanumeric();
}

int Parser::parseRegister() {
    char ch = lexer.advance();
    const std::string regs = "AXLBSTF";
    auto pos = regs.find(ch);
    if (pos == std::string::npos) 
        throw std::runtime_error(std::string("Invalid register: ") + ch);
    return static_cast<int>(pos);
}

void Parser::parseComma() {
    lexer.skipWhitespace();
    if (!lexer.advanceIf(',')) 
        throw std::runtime_error("Expected comma");
    lexer.skipWhitespace();
}

bool Parser::parseIndexed() {
    lexer.skipWhitespace();
    if (lexer.advanceIf(',')) 
    {
        lexer.skipWhitespace();
        if (!lexer.advanceIf('X'))
            throw std::runtime_error("Expected X for indexed addressing");
        return true;
    }
    return false;
}

int Parser::parseNumber(int lo, int hi) {
    std::string token = lexer.readDigits();
    int num = std::stoi(token);
    if (num < lo || num > hi)
        throw std::runtime_error("Number out of range");
    return num;
}

std::vector<uint8_t> Parser::parseData() {
    std::vector<uint8_t> data;
    if (lexer.advanceIf('C')) 
    {
        if (!lexer.advanceIf('\'')) throw std::runtime_error("Expected ' after C");
        std::string s = lexer.readTo('\'');
        lexer.advanceIf('\'');
        data.assign(s.begin(), s.end());
    } else if (lexer.advanceIf('X')) 
    {
        if (!lexer.advanceIf('\'')) throw std::runtime_error("Expected ' after X");
        std::string s = lexer.readTo('\'');
        lexer.advanceIf('\'');
        for (size_t i = 0; i < s.size() / 2; ++i)
            data.push_back(std::stoi(s.substr(2*i, 2), nullptr, 16));
    } else
    {
        int num = parseNumber(0, 0xFFFFFF);
        data.push_back((num >> 16) & 0xFF);
        data.push_back((num >> 8) & 0xFF);
        data.push_back(num & 0xFF);
    }
    return data;
}

SourceLine Parser::parseSourceLine() {
    SourceLine line{};
    line.isComment = false;

    lexer.skipWhitespace();
    if (lexer.eof()) return line;

    // Comment line
    if (lexer.peek() == '.') {
        line.isComment = true;
        readLine();
        return line;
    }

    std::string rawLine = readLine();
    if (rawLine.empty()) { line.isComment = true; return line; }

    auto trim = [](std::string &s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        if (start == std::string::npos) s = "";
        else s = s.substr(start, end - start + 1);
    };

    std::istringstream iss(rawLine);
    std::string firstToken, secondToken, thirdToken;
    iss >> firstToken >> secondToken;
    std::getline(iss, thirdToken);

    trim(firstToken);
    trim(secondToken);
    trim(thirdToken);

    auto isDirective = [](const std::string& s) {
    static const std::unordered_set<std::string> dirs = {
        "START","END","BYTE","WORD","RESB","RESW","BASE","NOBASE","LTORG","EQU","ORG"
    };
    return dirs.count(s) != 0;
    };


    // If second token is START, treat first token as program name (label)
    if (secondToken == "START") 
    {
    line.label = firstToken;  // program name
    line.opcode = secondToken;
    line.operand = thirdToken;
    } else if (Mnemonic::exists(firstToken) || firstToken[0] == '+' || isDirective(firstToken)) {
        line.label = "";
        line.opcode = firstToken;

    // Operand may span multiple tokens (e.g. "A, T")
        if (!thirdToken.empty())
            line.operand = secondToken + " " + thirdToken;
        else
            line.operand = secondToken;
    } else {
        line.label = firstToken;
        line.opcode = secondToken;
        line.operand = thirdToken;
    }


    return line;
}


std::string Parser::readLine() {
    std::string line = lexer.readTo('\n'); 
    if (lexer.peek() == '\n') lexer.advance(); // consume the newline
    return line;
}
void Parser::skipWhitespace() { lexer.skipWhitespace(); }
bool Parser::eof() const { return lexer.eof(); }
bool Parser::eol() const { return lexer.eol(); }


