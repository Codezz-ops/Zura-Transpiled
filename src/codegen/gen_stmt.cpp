#include "gen.hpp"
#include "optimize.hpp"

void codegen::visitStmt(Node::Stmt *stmt) {
  auto handler = lookup(stmtHandlers, stmt->kind);
  if (handler) {
    handler(stmt);
  }
}

void codegen::expr(Node::Stmt *stmt) {
  auto expr = static_cast<ExprStmt *>(stmt);
  visitExpr(expr->expr);
}

void codegen::program(Node::Stmt *stmt) {
  auto program = static_cast<ProgramStmt *>(stmt);

  for (auto &s : program->stmt) {
    visitStmt(s);
  }
}

void codegen::constDecl(Node::Stmt *stmt) {
  auto constDecl = static_cast<ConstStmt *>(stmt);
  visitStmt(constDecl->value);
}

void codegen::funcDecl(Node::Stmt *stmt) {
  auto funcDecl = static_cast<fnStmt *>(stmt);

  if (funcDecl->name == "main") {
    isEntryPoint = true;
    push(Instr { .var = Label { .name = "_start" }, .type = InstrType::Label }, true);
  } else {
    push(Instr { .var = Label { .name = funcDecl->name }, .type = InstrType::Label }, true);
  }

  // Todo: Handle function arguments
  // Todo: Handle Function return type

  visitStmt(funcDecl->block);
  stackSize++;
}

void codegen::varDecl(Node::Stmt *stmt) {
  auto varDecl = static_cast<VarStmt *>(stmt);
  /*
  ---- ex 1
  user_add:
    ; Before function was called,
    ; we pushed our values.
    ; (lets assume 2)
    ; define these as variables
    ; it would look more like:
    push qword [rsp + 8] ; a (further down the stack)
    push qword [rsp]     ; b

    pop rbx ; 'b'
    pop rax ; 'a' (was pushed first)
    add rax, rbx ; ret juts escapes the function, we store result in rax
    ; its SO much easier after vardecl's are implemented because you have the 'infrastructure'
    ret - rax is not ENFORCED as the return type (stack works too, especially as a function call expr), but it is standard and pretty helpful sometimes.

    _start:
      push 8 ; 'a'
      ; do not define variables here!! do it in the function block!!!
      push 85 ; 'b'
      call user_add
      ; for this example
      ; becuase like every expr,
      ; we would push the return
      push rax
      ; then for rdi it would be
      pop rdi
      ; which would be optimized into
      ; this
      mov rdi, rax
      mov rax, 60 ; now we exit
      syscall ; and hope it didnt crash :)

      ----
      add (a, b) {}

      main () {
        let x = add(1, 2);
        return x;
      }
      a func table is smart for when you parse, but when you compile you know all the type-checking passed and you dont need to wrory about a function table
      ----
      _start:
        push 1 ; param 1
        push 2 ; param 2
        call user_add
        ---- whatever shit happens there
  push(Instr { .var = Ret {}, .type = InstrType::Ret }, true);
        refer to x:
        push qword [rsp + (stackSize - x.loc)]
        ; its the same algorithm, the expr for the vardecl is just "push rax" after call
  */
  
  // shut up error
  push(Instr { .var = Comment { .comment = "define variable '" + varDecl->name + "'" }, .type = InstrType::Comment }, true);
  visitStmt(varDecl->expr);

  // add variable to the stack
  stackTable.insert({varDecl->name, stackSize});
}

void codegen::block(Node::Stmt *stmt) {
  auto block = static_cast<BlockStmt *>(stmt);
  for (auto &s : block->stmts) {
    visitStmt(s);
  }
}

void codegen::retrun(Node::Stmt *stmt) {
  auto returnStmt = static_cast<ReturnStmt *>(stmt);

  if (isEntryPoint) {
    visitExpr(returnStmt->expr);

    // pop the expression we just visited
    push(Instr { .var = PopInstr { .where = "rdi" }, .type = InstrType::Pop }, true);
    stackSize--;
    
    push(Instr { .var = MovInstr { .dest = "rax", .src = "60" }, .type = InstrType::Mov }, true);
    push(Instr { .var = Syscall { .name = "SYS_EXIT" }, .type = InstrType::Syscall }, true);
    return;
  }
  visitExpr(returnStmt->expr);
  push(Instr { .var = PopInstr { .where = "rdi" }, .type = InstrType::Pop }, true);
  stackSize--;
  push(Instr { .var = Ret {}, .type = InstrType::Ret }, true);
}
