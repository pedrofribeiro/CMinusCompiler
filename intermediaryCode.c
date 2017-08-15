#include "intermediaryCode.h"

void evalStmt(TreeNode *node){
  switch (node->kind) {
    case DadoK:
    break;
    case VarK:
    break;
    case VetK:
    break;
    case ReturnK:
    break;
    case AtribK:
    break;
    case IfK:
    break;
    case RepeatK:
    break;
    case FunK:
    break;
    default:
      callException("evalStmt",1,4);
    break;
  }
}

//typedef enum {OpK, IdK, IdVetK, IdFunK, ConstK} ExpKind;
void evalExp(TreeNode *node){
  switch (node->kind) {
    case ConstK:
    break;
    case IdK:
    break;
    case IdVetK:
    break;
    case OpK:
    break;
    case IdFunK:
    break;
    default:
      callException("evalExp",1,4);
    break;
  }
}

void evalProgram(TreeNode *node){
  if(node == NULL) callException("evalProgram",2,4);
  switch (node->nodekind) {
    case StmtK:
      evalStmt(node);
    break;
    case ExpK:
      evalExp(node);
    break;
    default:
      callException("evalProgram",1,4);
    break;
  }
  evalProgram(node->sibling); /*recursive descent*/
}
