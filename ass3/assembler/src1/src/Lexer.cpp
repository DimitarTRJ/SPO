#include "Lexer.h"
#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input), pos(0), col(1) {}

char Lexer::peek() const {
    if (pos >= input.size()) return '\0';
    return input[pos];
}

char Lexer::advance() {
    if (pos >= input.size()) return '\0';
    char ch = input[pos++];
    col++;
    return ch;
}

bool Lexer::advanceIf(char ch) {
    if (peek() == ch) {
        advance();
        return true;
    }
    return false;
}

std::string Lexer::readAlphanumeric() {
    std::string token;
    while (std::isalnum(peek()) || peek() == '_') {
        token += advance();
    }
    return token;
}

std::string Lexer::readDigits() {
    std::string token;
    while (std::isdigit(peek())) {
        token += advance();
    }
    return token;
}

std::string Lexer::readTo(char delimiter) {
    std::string token;
    while (peek() != delimiter && peek() != '\0') {
        token += advance();
    }
    return token;
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek()) && peek() != '\n') advance();
}

int Lexer::getCol() const {
    return col;
}

bool Lexer::eof() const {
    return pos >= input.size();
}

bool Lexer::eol() const {
    return peek() == '\n' || peek() == '\0';
}
