#include "intermediaryCode.h"
#include "triple.h"

//addTriple(operationType op, int fo, int so, operandType fot, operandType sot);

void evalStmt(TreeNode *node){
  switch (node->kind.stmt) {
    case DadoK:
      if(node->child[0] == NULL){
        callException("evalStmt DadoK",6,4);
        return;
      }
      evalStmt(node->child[0]);
    break;
    case VarK:
    break;
    case VetK:
    break;
    case ReturnK:
      evalStmt(node->child[0]);
      if(node->child[0]->kind.exp == IdFunK){
          int tempVar = NUMBER_OF_TRIPLES + 1 - node->child[0]->numberOfParameters;
          addTriple(RTRN,tempVar,-1,TripleAddress,Empty);
      }
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
  switch (node->kind.exp) {
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
