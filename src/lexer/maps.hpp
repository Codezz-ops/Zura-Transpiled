#include <unordered_map>
#include <functional>
#include <string>

#include "lexer.hpp"

using WhiteSpaceFunction = std::function<void(Lexer&)>;
const std::unordered_map<char, WhiteSpaceFunction> whiteSpaceMap = {
    {' ',  [](Lexer &lexer) { lexer.advance(); }},
    {'\r', [](Lexer &lexer) { lexer.advance(); }},
    {'\t', [](Lexer &lexer) { lexer.advance(); }},
    {'\n', [](Lexer &lexer) {
      lexer.scanner.line++;
      lexer.scanner.column = 0;
      lexer.advance();
    }},
    {'#', [](Lexer &lexer) {
      while (lexer.peek() != '\n' && !lexer.isAtEnd()) lexer.advance();
    }},
};

const std::unordered_map<std::string, TokenKind> keywords = {
      {"and", TokenKind::AND},
      {"class", TokenKind::CLASS},
      {"else", TokenKind::ELSE},
      {"false", TokenKind::FAL},
      {"fn", TokenKind::FUN},
      {"loop", TokenKind::LOOP},
      {"if", TokenKind::IF},
      {"nil", TokenKind::NIL},
      {"or", TokenKind::OR},
      {"dis", TokenKind::PRINT},
      {"return", TokenKind::RETURN},
      {"exit", TokenKind::EXIT},
      {"super", TokenKind::SUPER},
      {"this", TokenKind::THS},
      {"true", TokenKind::TR},
      {"have", TokenKind::VAR},
      {"pkg", TokenKind::PKG},
      {"type", TokenKind::TYPE},
      {"struct", TokenKind::STRUCT},
};

const std::unordered_map<char, TokenKind> scMap = {
    {'(', TokenKind::LEFT_PAREN},
    {')', TokenKind::RIGHT_PAREN},
    {'{', TokenKind::LEFT_BRACE},
    {'}', TokenKind::RIGHT_BRACE},
    {';', TokenKind::SEMICOLON},
    {',', TokenKind::COMMA},
    {'.', TokenKind::DOT},
    {'-', TokenKind::MINUS},
    {'+', TokenKind::PLUS},
    {'/', TokenKind::SLASH},
    {'*', TokenKind::STAR},
    {'%', TokenKind::MODULO},
    {'^', TokenKind::CARET},
    {'[', TokenKind::LEFT_BRACKET},
    {']', TokenKind::RIGHT_BRACKET},
    {':', TokenKind::COLON},
    {'=', TokenKind::EQUAL},
    {'!', TokenKind::BANG},
    {'<', TokenKind::LESS},
    {'>', TokenKind::GREATER},
};

const std::unordered_map<std::string, TokenKind> dcMap = {
      {"!=", TokenKind::BANG_EQUAL},
      {"==", TokenKind::EQUAL_EQUAL},
      {">=", TokenKind::GREATER_EQUAL},
      {"<=", TokenKind::LESS_EQUAL},
      {":=", TokenKind::WALRUS},
      {"++", TokenKind::PLUS_PLUS},
      {"--", TokenKind::MINUS_MINUS},
      {"+=", TokenKind::PLUS_EQUAL},
      {"-=", TokenKind::MINUS_EQUAL},
      {"*=", TokenKind::STAR_EQUAL},
      {"/=", TokenKind::SLASH_EQUAL},
      {"&&", TokenKind::AND},
      {"||", TokenKind::OR},
};

const std::unordered_map<TokenKind, const char *> tokenMap = {
    {TokenKind::LEFT_PAREN, "LEFT_PAREN"},
    {TokenKind::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenKind::LEFT_BRACE, "LEFT_BRACE"},
    {TokenKind::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenKind::LEFT_BRACKET, "LEFT_BRACKET"},
    {TokenKind::RIGHT_BRACKET, "RIGHT_BRACKET"},
    {TokenKind::COMMA, "COMMA"},
    {TokenKind::DOT, "DOT"},
    {TokenKind::MINUS, "MINUS"},
    {TokenKind::PLUS, "PLUS"},
    {TokenKind::SEMICOLON, "SEMICOLON"},
    {TokenKind::SLASH, "SLASH"},
    {TokenKind::STAR, "STAR"},
    {TokenKind::MODULO, "MODULO"},
    {TokenKind::CARET, "CARET"},
    {TokenKind::COLON, "COLON"},
    {TokenKind::STRUCT, "STRUCT"},
    {TokenKind::BANG, "BANG"},
    {TokenKind::BANG_EQUAL, "BANG_EQUAL"},
    {TokenKind::EQUAL, "EQUAL"},
    {TokenKind::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenKind::GREATER, "GREATER"},
    {TokenKind::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenKind::LESS, "LESS"},
    {TokenKind::LESS_EQUAL, "LESS_EQUAL"},
    {TokenKind::WALRUS, "WALRUS"},
    {TokenKind::IDENTIFIER, "IDENTIFIER"},
    {TokenKind::STRING, "STRING"},
    {TokenKind::NUMBER, "NUMBER"},
    {TokenKind::AND, "AND"},
    {TokenKind::CLASS, "CLASS"},
    {TokenKind::ELSE, "ELSE"},
    {TokenKind::FAL, "FAL"},
    {TokenKind::FUN, "FUN"},
    {TokenKind::LOOP, "LOOP"},
    {TokenKind::IF, "IF"},
    {TokenKind::NIL, "NIL"},
    {TokenKind::OR, "OR"},
    {TokenKind::PRINT, "PRINT"},
    {TokenKind::RETURN, "RETURN"},
    {TokenKind::SUPER, "SUPER"},
    {TokenKind::THS, "THS"},
    {TokenKind::TR, "TRUE"},
    {TokenKind::VAR, "VAR"},
    {TokenKind::PKG, "PKG"},
    {TokenKind::TYPE, "TYPE"},
    {TokenKind::EXIT, "EXIT"},
};