#pragma once

#include "../lexer/lexer.hpp"

#include <iostream>
#include <string>
#include <vector>

enum NodeKind {
  // Expressions
  ND_NUMBER,
  ND_IDENT,
  ND_STRING,
  ND_BINARY,
  ND_UNARY,
  ND_PREFIX,
  ND_GROUP,
  ND_ARRAY,
  ND_CALL,
  ND_ASSIGN,
  ND_TERNARY,
  ND_MEMBER,

  // Statements
  ND_EXPR_STMT,
  ND_VAR_STMT,
  ND_CONST_STMT,
  ND_BLOCK_STMT,
  ND_FN_STMT,
  ND_PROGRAM,
  ND_RETURN_STMT,
  ND_IF_STMT,
  ND_STRUCT_STMT,
  ND_WHILE_STMT,
  ND_FOR_STMT,
  ND_PRINT_STMT,
  ND_ENUM_STMT,
  ND_IMPORT_STMT,

  // Types
  ND_SYMBOL_TYPE,
  ND_ARRAY_TYPE,
  ND_POINTER_TYPE,
  ND_CALLABLE_TYPE,
  ND_FUNCTION_TYPE,
};

class Node {
public:
  // Store the list of tks generated by the parser
  // so that we can generate the line from the line and
  // pos stored on the ast nodes; for error reporting
  std::vector<Lexer::Token> tks;
  // Store the current file name
  std::string current_file;

  struct Expr {
    NodeKind kind;
    virtual void debug(int ident = 0) const = 0;
    virtual ~Expr() = default;
  };

  struct Stmt {
    NodeKind kind;
    virtual void debug(int ident = 0) const = 0;
    virtual ~Stmt() = default;
  };

  struct Type {
    NodeKind kind;
    virtual void debug(int ident = 0) const = 0;
    virtual ~Type() = default;
  };

  static void printIndent(int ident) {
    for (int i = 0; i < ident; i++) {
      std::cout << "    ";
    }
  }
};

// This is probably a really bad idea,
// but it works so i am not touching it
inline Node node;