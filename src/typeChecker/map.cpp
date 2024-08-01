#include "../ast/ast.hpp"
#include "type.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace TypeChecker;

std::vector<std::pair<NodeKind, TypeChecker::StmtNodeHandler>>
    TypeChecker::stmts = {
        {NodeKind::ND_PROGRAM, visitProgram},
        {NodeKind::ND_CONST_STMT, visitConst},
        {NodeKind::ND_FN_STMT, visitFn},
        {NodeKind::ND_BLOCK_STMT, visitBlock},
        {NodeKind::ND_RETURN_STMT, visitReturn},
        {NodeKind::ND_VAR_STMT, visitVar},
        {NodeKind::ND_EXPR_STMT,
         [](Maps::global_symbol_table &gTable, Maps::local_symbol_table &lTable,
            Maps::function_table &fn_table, Node::Stmt *stmt) {
           auto expr_stmt = static_cast<ExprStmt *>(stmt);
           visitExpr(gTable, lTable, fn_table, expr_stmt->expr);
         }},
};

std::vector<std::pair<NodeKind, TypeChecker::ExprNodeHandler>>
    TypeChecker::exprs = {
        {NodeKind::ND_NUMBER, visitNumber}, {NodeKind::ND_IDENT, visitIdent},
        {NodeKind::ND_STRING, visitString}, {NodeKind::ND_BINARY, visitBinary},
        {NodeKind::ND_CALL, visitCall},
};

Node::Stmt *TypeChecker::StmtAstLookup(Node::Stmt *node,
                                       Maps::global_symbol_table gTable,
                                       Maps::local_symbol_table lTable,
                                       Maps::function_table fn_table) {
  auto res = std::find_if(stmts.begin(), stmts.end(), [&](auto &stmtHandler) {
    return node->kind == stmtHandler.first;
  });
  if (res != stmts.end())
    res->second(gTable, lTable, fn_table, node);
  return node;
}

Node::Expr *TypeChecker::ExprAstLookup(Node::Expr *node,
                                       Maps::global_symbol_table gTable,
                                       Maps::local_symbol_table lTable,
                                       Maps::function_table fn_table) {
  auto res = std::find_if(exprs.begin(), exprs.end(), [&](auto &exprHandler) {
    return node->kind == exprHandler.first;
  });
  if (res != exprs.end())
    res->second(gTable, lTable, fn_table, node);
  return node;
}

void TypeChecker::Maps::declare_fn(
    Maps::function_table &fn_table, std::string name,
    const Maps::NameTypePair &pair,
    std::vector<std::pair<std::string, Node::Type *>> paramTypes, int line,
    int pos) {
  auto res = std::find_if(
      fn_table.begin(), fn_table.end(),
      [&name](const std::pair<NameTypePair,
                              std::vector<std::pair<std::string, Node::Type *>>>
                  &pair) { return pair.first.first == name; });
  if (res != fn_table.end()) {
    std::string msg = "'" + name + "' is already defined";
    handlerError(line, pos, msg, "", "Function Table Error");
    return;
  }
  fn_table.push_back({pair, paramTypes});
}

std::pair<TypeChecker::Maps::NameTypePair,
          std::vector<std::pair<std::string, Node::Type *>>>
TypeChecker::Maps::lookup_fn(Maps::function_table &fn_table, std::string name,
                             int line, int pos) {
  auto res = std::find_if(
      fn_table.begin(), fn_table.end(),
      [&name](const std::pair<NameTypePair,
                              std::vector<std::pair<std::string, Node::Type *>>>
                  &pair) { return pair.first.first == name; });
  if (res != fn_table.end())
    return *res;

  std::string msg = "'" + name + "' is not defined in the function table";
  handlerError(line, pos, msg, "", "Function Table Error");
  return {{name, new SymbolType("unknown")}, {}};
}

void TypeChecker::Maps::printTables(Maps::global_symbol_table &gTable,
                                    Maps::local_symbol_table &lTable,
                                    Maps::function_table &fn_table) {
  std::cout << "Global Table" << std::endl;
  for (auto &pair : gTable) {
    std::cout << "\t" << pair.first << " : " << type_to_string(pair.second)
              << std::endl;
  }

  std::cout << "Local Table" << std::endl;
  for (auto &pair : lTable) {
    std::cout << "\t" << pair.first << " : " << type_to_string(pair.second)
              << std::endl;
  }

  std::cout << "Function Table" << std::endl;
  for (auto &pair : fn_table) {
    std::cout << "\t" << pair.first.first << " : "
              << type_to_string(pair.first.second) << std::endl;
    for (auto &param : pair.second) {
      std::cout << "\t\t" << param.first << " : "
                << type_to_string(param.second) << std::endl;
    }
  }
}