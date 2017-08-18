#include "intermediaryCode.h"
#include "triple.h"

void evalStmt(TreeNode *node){
  TreeNode *p0, *p1, *p2;

  switch (node->kind.stmt) {
    case DadoK:
      printf("[DadoK]\n");
      p0 = node->child[0];

      if (p0 == NULL) {
        callException("evalStmt: DadoK",6,4);
        return;
      }

      evalProgram(p0);
    break;
    case VarK:
      printf("[VarK]\n");
    break;
    case VetK:
      printf("[VetK]\n");
    break;
    case ReturnK:
      printf("[ReturnK]\n");
      p0 = node->child[0];

      if (p0 == NULL) break;

      evalProgram(p0);

      if (p0->kind.exp == ConstK) {
          addTriple("RETURN",p0->attr.val,-1,ConstantNoAddress,EmptyAddress);
      } else if(p0->kind.exp == IdK) {
          addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      } else if(p0->kind.exp == IdVetK) {
          addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      } else if(p0->kind.exp == IdFunK) {
          addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      } else if(p0->kind.exp == OpK) {
          addTriple("RETURN",NUMBER_OF_TRIPLES-1,-1,TripleAddress,EmptyAddress);
      } else if(p0->kind.exp == AtribK) {
          addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      } else {
          callException("evalStmt: ReturnK",7,4);
      }
    break;
    case AtribK:
      printf("[AtribK]\n");
      p0 = node->child[0];
      p1 = node->child[1];

      evalProgram(p0);
      int leftOperand;
      leftOperand = NUMBER_OF_TRIPLES;
      evalProgram(p1);
      int rightOperand;
      rightOperand = NUMBER_OF_TRIPLES;

      if ((p0->kind.exp == IdK) && (p1->kind.exp == ConstK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),p1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdVetK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == OpK)) {
          evalProgram(p1);
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdFunK)) {
          evalProgram(p1);
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == ConstK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),p1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdVetK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),st_lookupFnStart(p1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == OpK)) {
          evalProgram(p1);
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdFunK)) {
          evalProgram(p1);
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else {
          callException("evalStmt: AtribK",7,4);
      }
    break;
    case IfK:
      printf("[IfK]\n");
      p0 = node->child[0];
      p1 = node->child[1];
      p2 = node->child[2];

      if (p0 == NULL) break; /*verifying the test*/
      if (p1 == NULL) break; /*verifying the code of a true test*/

      if (p2 == NULL) { /*there is not an else part*/

        evalProgram(p0);
        int testTriple = NUMBER_OF_TRIPLES;
        addTriple("IF_F",testTriple,-999,TripleAddress,TripleAddress);
        int ifTriple = NUMBER_OF_TRIPLES;
        evalProgram(p1);
        int elseTriple = NUMBER_OF_TRIPLES+1;
        int adjustment;
        adjustment = adjustTriple(ifTriple,2,elseTriple); /*goes to the triple after the true part*/
        if (adjustment == 1) { printf("The operand address was correctly adjusted.\n"); }
        else { callException("evalStmt: IfK",9,4); }

      } else { /*there is an else part*/

          evalProgram(p0);
          int testTriple = NUMBER_OF_TRIPLES;
          addTriple("IF_F",testTriple,-999,TripleAddress,TripleAddress);
          int ifTriple = NUMBER_OF_TRIPLES;
          evalProgram(p1);
          addTriple("GOTO",-999,-999,TripleAddress,EmptyAddress);
          int gotoTriple = NUMBER_OF_TRIPLES;
          int elseTriple = gotoTriple+1;
            int adjustment;
            adjustment = adjustTriple(ifTriple,2,elseTriple); /*goes to the triple after the true part*/
            if (adjustment == 1) { printf("The operand address was correctly adjusted.\n"); }
            else { callException("evalStmt: IfK",9,4); }
          evalProgram(p2);
          int addrToJumpElsePart;
          addrToJumpElsePart = NUMBER_OF_TRIPLES+1;
            int secondAdjustment;
            secondAdjustment = adjustTriple(gotoTriple,1,addrToJumpElsePart);
            if (secondAdjustment == 1) { printf("The operand address was correctly adjusted.\n"); }
            else { callException("evalStmt: IfK",9,4); }

      }
    break;
    case RepeatK:
      printf("[RepeatK]\n");
      p0 = node->child[0];
      p1 = node->child[1];

      if (p0 == NULL) break;
      if (p1 == NULL) break;

      evalProgram(p0); /*evaluates the test*/
      int testTriple;
      testTriple = NUMBER_OF_TRIPLES;
      addTriple("IF_F",testTriple,-999,TripleAddress,TripleAddress);
      int repeatTriple;
      repeatTriple = NUMBER_OF_TRIPLES;
      evalProgram(p1);
      addTriple("GOTO",testTriple,-999,TripleAddress,EmptyAddress);
      int addrToJump;
      addrToJump = NUMBER_OF_TRIPLES+1;
        int adjustment;
        adjustment = adjustTriple(repeatTriple,2,addrToJump);
        if (adjustment == 1) { printf("The operand address was correctly adjusted.\n"); }
        else { callException("evalStmt: RepeatK",9,4); }

    break;
    case FunK:
      printf("[FunK]\n");

      p0 = node->child[0]; /* arguments*/
      p1 = node->child[1]; /* fn code*/

      evalProgram(p0); /*evaluates the arguments*/
      /*somehow the function must be marked as initiated at this particular Triple Number+1;   [ very important ]*/
      evalProgram(p1); /*evaluates the fn code*/

    break;
    default:
      callException("evalStmt",1,4);
    break;
  }
}

void evalExp(TreeNode *node){
  TreeNode *q0, *q1;
  switch (node->kind.exp) {
    case ConstK:
      printf("[ConstK]\n");
      //done as a subcase of another case
    break;
    case IdK:
      printf("[IdK]\n");
      //done as a subcase of another case
    break;
    case IdVetK:
      printf("[IdVetK]\n");
      //not yet, I think this shall be done as a subcase of another case as well
    break;
    case OpK:
      printf("[OpK]\n");
      q0 = node->child[0];
      q1 = node->child[1];

      if ((q0->kind.exp == ConstK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,q0->attr.val,q1->attr.val,ConstantNoAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdK)) {
          addTriple(node->attr.name,q0->attr.val,st_lookupFnStart(q1->attr.name),ConstantNoAddress,SymboltableAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdVetK)) { // [ NEEDS TREATMENT ]
          addTriple(node->attr.name,q0->attr.val,st_lookupFnStart(q1->attr.name),ConstantNoAddress,SymboltableAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdFunK)) { // [ NEEDS TREATMENT ] pdf da tripla
          evalProgram(q1);
          addTriple(node->attr.name,q0->attr.val,NUMBER_OF_TRIPLES-1,ConstantNoAddress,TripleAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         addTriple(node->attr.name,q0->attr.val,NUMBER_OF_TRIPLES-1,ConstantNoAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),q1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),st_lookupFnStart(q1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdVetK)) { // [ NEEDS TREATMENT ]
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),st_lookupFnStart(q1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdFunK)) { // [ NEEDS TREATMENT ] pdf da tripla
          evalProgram(q1);
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),q1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),st_lookupFnStart(q1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdVetK)) { // [ NEEDS TREATMENT ]
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),st_lookupFnStart(q1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdFunK)) { // [ NEEDS TREATMENT ] pdf da tripla
          evalProgram(q1);
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),NUMBER_OF_TRIPLES-1,SymboltableAddress,TripleAddress);
      }

      // [ NEEDS TREATMENT ] this whole block needs validation
       else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == ConstK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,q1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdVetK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdFunK)) {
         evalProgram(q0);
         int temp1 = NUMBER_OF_TRIPLES-1;
         evalProgram(q1);
         addTriple(node->attr.name,temp1,NUMBER_OF_TRIPLES-1,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == OpK)) {
         evalProgram(q0);
         int temp1 = NUMBER_OF_TRIPLES-1;
         evalProgram(q1);
         addTriple(node->attr.name,temp1,NUMBER_OF_TRIPLES-1,TripleAddress,TripleAddress);
      }

       else if ((q0->kind.exp == OpK) && (q1->kind.exp == ConstK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,q1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdVetK)) {
         evalProgram(q0);
         addTriple(node->attr.name,NUMBER_OF_TRIPLES-1,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdFunK)) {
         evalProgram(q0);
         int temp2 = NUMBER_OF_TRIPLES-1;
         evalProgram(q1);
         addTriple(node->attr.name,temp2,NUMBER_OF_TRIPLES-1,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == OpK)) {
         evalProgram(q0);
         int temp1 = NUMBER_OF_TRIPLES-1;
         evalProgram(q1);
         addTriple(node->attr.name,temp1,NUMBER_OF_TRIPLES-1,TripleAddress,TripleAddress);
      }

       else {
        callException("evalExp: OpK",7,4);
      }

    break;
    case IdFunK:
      printf("[IdFunK]\n");
      q0 = node->child[0];
      evalProgram(q0);
      addTriple("FNCALL",st_lookupFnStart(node->attr.name),node->numberOfParameters,SymboltableAddress,ConstantNoAddress);
    break;
    default:
      callException("evalExp",1,4);
    break;
  }
}

void evalProgram(TreeNode *node){
  if(node == NULL){
    callException("evalProgram",2,4);
    return;
  }
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

void generateIntermediaryCode(TreeNode * t){
  initializeTripleList();
  evalProgram(t);
  printTripleList();
}
