#include "intermediaryCode.h"
#include "triple.h"

//addTriple(operationType op, int fo, int so, operandType fot, operandType sot);

void evalStmt(TreeNode *node){

  TreeNode *p0, *p1, *p2;

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
      p0 = node->child[0];
      evalProgram(p0);

      if(p0->kind.exp == ConstK){
        addTriple("RETURN",p0->attr.val,-1,ConstantNoAddress,EmptyAddress);
      }else if(p0->kind.exp == IdK){
        addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else if(p0->kind.exp == IdVetK){
        addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else if(p0->kind.exp == IdFunK){
        addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      }else if(p0->kind.exp == OpK){
        addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      }else if(p0->kind.exp == AtribK){
        addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      }else{
        callException("evalStmt: ReturnK",7,4);
      }
    break;
    case AtribK:
      p0 = node->child[0];
      p1 = node->child[1];
      evalProgram(p0);
      evalProgram(p1);

      if((p0->kind.exp == IdK) && (p1->kind.exp == ConstK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),p1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if((p0->kind.exp == IdK) && (p1->kind.exp == IdK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((p0->kind.exp == IdK) && (p1->kind.exp == IdVetK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((p0->kind.exp == IdK) && (p1->kind.exp == OpK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if((p0->kind.exp == IdK) && (p1->kind.exp == IdFunK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if((p0->kind.exp == IdVetK) && (p1->kind.exp == ConstK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),p1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if((p0->kind.exp == IdVetK) && (p1->kind.exp == IdK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((p0->kind.exp == IdVetK) && (p1->kind.exp == IdVetK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if((p0->kind.exp == IdVetK) && (p1->kind.exp == OpK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if((p0->kind.exp == IdVetK) && (p1->kind.exp == IdFunK)){
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
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
  TreeNode *q0, *q1;
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
      }else if(node->child[0]->kind.exp == IdK){
        //not yet
      }else if(node->child[0]->kind.exp == IdVetK){
        //not yet
      }else if(node->child[0]->kind.exp == IdFunK){
        //not yet
      }else{
        callException("evalExp: OpK",7,4);
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
