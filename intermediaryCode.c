#include "intermediaryCode.h"
#include "triple.h"

//addTriple(operationType op, int fo, int so, operandType fot, operandType sot);

void evalStmt(TreeNode *node){
  switch (node->kind.stmt) {
    case DadoK:
      if(node->child[0] == NULL){
        callException("evalStmt: DadoK",6,4);
        return;
      }
      evalProgram(node->child[0]);
    break;
    case VarK:
    break;
    case VetK:
    break;
    case ReturnK:
      evalProgram(node->child[0]);

      if(node->child[0]->kind.exp == ConstK){
        addTriple("RETURN",node->child[0]->attr.value,-1,ConstantNoAddress,EmptyAddress);
      }else if(node->child[0]->kind.exp == IdVarK){
        addTriple("RETURN",st_lookupFnStart(node->child[0]->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else if(node->child[0]->kind.exp == IdVetK){
        addTriple("RETURN",st_lookupFnStart(node->child[0]->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else if(node->child[0]->kind.exp == IdFunK){
        addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      }else if(node->child[0]->kind.exp == OpK){
        addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      }else if(node->child[0]->kind.exp == AtribK){
        addTriple("RETURN",st_lookupFnStart(node->child[0]->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else{
        callException("evalStmt: ReturnK",7,4);
      }
    break;
    case AtribK:
      evalProgram(node->child[0]);
      evalProgram(node->child[1]);

      if((node->child[0]->kind.exp == IdVarK) && (node->child[1]->kind.exp == ConstK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),node->child[1]->attr.value,SymboltableAddress,ConstantNoAddress);
      } else if((node->child[0]->kind.exp == IdVarK) && (node->child[1]->kind.exp == IdVarK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),st_lookupFnStart(node->child[1]->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((node->child[0]->kind.exp == IdVarK) && (node->child[1]->kind.exp == IdVetK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),st_lookupFnStart(node->child[1]->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((node->child[0]->kind.exp == IdVarK) && (node->child[1]->kind.exp == OpK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if((node->child[0]->kind.exp == IdVarK) && (node->child[1]->kind.exp == IdFunK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if((node->child[0]->kind.exp == IdVetK) && (node->child[1]->kind.exp == ConstK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),node->child[1]->attr.value,SymboltableAddress,ConstantNoAddress);
      } else if((node->child[0]->kind.exp == IdVetK) && (node->child[1]->kind.exp == IdVarK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),st_lookupFnStart(node->child[1]->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((node->child[0]->kind.exp == IdVetK) && (node->child[1]->kind.exp == IdVetK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name),st_lookupFnStart(node->child[1]->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((node->child[0]->kind.exp == IdVetK) && (node->child[1]->kind.exp == OpK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name));
      } else if((node->child[0]->kind.exp == IdVetK) && (node->child[1]->kind.exp == IdFunK)){
          addTriple("ATRIB",st_lookupFnStart(node->child[0]->attr.name));
      } else{
          callException("evalStmt: AtribK",7,4);
      }
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
        // a = const, var, vet, fn call
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
