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

//addTriple(operationType op, int fo, int so, operandType fot, operandType sot);
void evalExp(TreeNode *node){
  switch (node->kind.exp) {
    case ConstK:
      //done as a subcase of another case
    break;
    case IdK:
      //done as a subcase of another case
    break;
    case IdVetK:
      //not yet
    break;
    case OpK:
      //this conditional investigates the RHS, knowing that the LHS can only be either a constant, a var, a vet or a fn call.
      if(node->child[0]->kind.exp == ConstK){
        // a = const, var, vet, fn call,
      }else if(node->child[0]->kind.exp == IdVarK){
        //not yet
      }else if(node->child[0]->kind.exp == IdVetK){
        //not yet
      }else if(node->child[0]->kind.exp == IdFunK){
        //not yet
      }else{
        callException("evalExp OpK",7,4);
      }
    break;
    case IdFunK:
      //parameters must be done inside here. otherwise they will be treated as var decls.
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
