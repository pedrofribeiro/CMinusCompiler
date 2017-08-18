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

      if (p0->kind.exp == ConstK) {
          evalProgram(p0);
          addTriple("RETURN",p0->attr.val,-1,ConstantNoAddress,EmptyAddress);
      } else if(p0->kind.exp == IdK) {
          evalProgram(p0);
          addTriple("RETURN",st_lookupFnStart(p0->attr.name),-1,SymboltableAddress,EmptyAddress);
      } else if(p0->kind.exp == IdVetK) {
          evalProgram(p0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",vectorTriple,-1,TripleAddress,EmptyAddress);
      } else if(p0->kind.exp == IdFunK) {
          evalProgram(p0);
          int returnTriple;
          returnTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",returnTriple,-1,TripleAddress,EmptyAddress);
      } else if(p0->kind.exp == OpK) {
          evalProgram(p0);
          int returnTriple;
          returnTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",returnTriple,-1,TripleAddress,EmptyAddress);
      } else if(p0->kind.exp == AtribK) { /*this may need a very serious further analysis*/
          evalProgram(p0);
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
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == OpK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdFunK)) {
          addTriple("ATRIB",st_lookupFnStart(p0->attr.name),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == ConstK)) {
          addTriple("ATRIB",leftOperand,p1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdK)) {
          addTriple("ATRIB",leftOperand,st_lookupFnStart(p1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdVetK)) {
          addTriple("ATRIB",leftOperand,st_lookupFnStart(p1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == OpK)) {
          addTriple("ATRIB",leftOperand,rightOperand,TripleAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdFunK)) {
          addTriple("ATRIB",leftOperand,rightOperand,TripleAddress,TripleAddress);
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
      p1 = node->child[1]; /* function statements' code*/

      addTriple(node->attr.name,-999,-999,EmptyAddress,EmptyAddress);

      evalProgram(p0); /*evaluates the arguments*/
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
      q0 = node->child[0];

      if (q0 == NULL){
        callException("evalExp: IdVetK",8,4);
        break;
      }

      evalProgram(q0);
      int indexTriple;
      indexTriple = NUMBER_OF_TRIPLES;

      if (q0->kind.exp == ConstK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),q0->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if (q0->kind.exp == IdK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),st_lookupFnStart(q0->attr.name),SymboltableAddress,SymboltableAddress);
      } else if (q0->kind.exp == IdVetK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
      } else if (q0->kind.exp == OpK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
      } else if (q0->kind.exp == AtribK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
      } else if (q0->kind.exp == IdFunK) {
          addTriple("V_INDEX",st_lookupFnStart(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
      } else {
        callException("evalExp: IdVetK",7,4);
      }

    break;
    case OpK:
      printf("[OpK]\n");
      q0 = node->child[0];
      q1 = node->child[1];

      if ((q0->kind.exp == ConstK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,q0->attr.val,q1->attr.val,ConstantNoAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdK)) {
          addTriple(node->attr.name,q0->attr.val,st_lookupFnStart(q1->attr.name),ConstantNoAddress,SymboltableAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdVetK)) {
          evalProgram(q1);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,q0->attr.val,vectorTriple,ConstantNoAddress,TripleAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdFunK)) {
          evalProgram(q1);
          int idfunkTriple;
          idfunkTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,q0->attr.val,idfunkTriple,ConstantNoAddress,TripleAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,q0->attr.val,opkTriple,ConstantNoAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),q1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdK)) {
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),st_lookupFnStart(q1->attr.name),SymboltableAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdVetK)) {
          evalProgram(q1);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),vectorTriple,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdFunK)) {
          evalProgram(q1);
          int idfunkTriple;
          idfunkTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),idfunkTriple,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,st_lookupFnStart(q0->attr.name),opkTriple,SymboltableAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == ConstK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,q1->attr.val,vectorTriple,ConstantNoAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdVetK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          evalProgram(q1);
          int otherVectorTriple;
          otherVectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,otherVectorTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdFunK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          evalProgram(q1);
          int idfunkTriple;
          idfunkTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,idfunkTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == OpK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          evalProgram(q1);
          int opkTriple;
          opkTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,opkTriple,TripleAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == ConstK)) {
         evalProgram(q0);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,idfunkTriple,q1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdK)) {
         evalProgram(q0);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,idfunkTriple,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdVetK)) {
         evalProgram(q0);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int vectorTriple;
         vectorTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,idfunkTriple,vectorTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == IdFunK)) {
         evalProgram(q0);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int otheridfunkTriple;
         otheridfunkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,idfunkTriple,otheridfunkTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == IdFunK) && (q1->kind.exp == OpK)) {
         evalProgram(q0);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,idfunkTriple,opkTriple,TripleAddress,TripleAddress);
      }

       else if ((q0->kind.exp == OpK) && (q1->kind.exp == ConstK)) {
         evalProgram(q0);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,opkTriple,q1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdK)) {
         evalProgram(q0);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,opkTriple,st_lookupFnStart(q1->attr.name),TripleAddress,SymboltableAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdVetK)) {
         evalProgram(q0);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int vectorTriple;
         vectorTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,opkTriple,vectorTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == IdFunK)) {
         evalProgram(q0);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int idfunkTriple;
         idfunkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,opkTriple,idfunkTriple,TripleAddress,TripleAddress);
      } else if ((q0->kind.exp == OpK) && (q1->kind.exp == OpK)) {
         evalProgram(q0);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         evalProgram(q1);
         int otheropkTriple;
         otheropkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,opkTriple,otheropkTriple,TripleAddress,TripleAddress);
      }

       else {
        callException("evalExp: OpK",7,4);
      }

    break;
    case IdFunK:
      printf("[IdFunK]\n");
      q0 = node->child[0];

      if (q0 == NULL) { /*the function does not take any arguments*/

        addTriple("FNCALL",st_lookupFnStart(node->attr.name),node->numberOfParameters,SymboltableAddress,ConstantNoAddress);

      } else { /*the function takes on arguments */

        int SAFE_LOOP = 0;
        int k = node->numberOfParameters;

        while (k > 0) {

          if (q0->kind.exp == ConstK) {
              evalExp(q0);
              addTriple("PARAM",q0->attr.val,-999,ConstantNoAddress,EmptyAddress);
          } else if (q0->kind.exp == IdK) {
              evalExp(q0);
              addTriple("PARAM",st_lookupFnStart(q0->attr.name),-999,SymboltableAddress,EmptyAddress);
          } else if (q0->kind.exp == IdVetK) {
              evalExp(q0);
              addTriple("PARAM",st_lookupFnStart(q0->attr.name),-999,SymboltableAddress,EmptyAddress);
          } else if (q0->kind.exp == OpK) {
              evalExp(q0);
              int paramTriple;
              paramTriple = NUMBER_OF_TRIPLES;
              addTriple("PARAM",paramTriple,-999,TripleAddress,EmptyAddress);
          } else if (q0->kind.exp == IdFunK) {
              evalExp(q0);
              int paramTriple;
              paramTriple = NUMBER_OF_TRIPLES;
              addTriple("PARAM",paramTriple,-999,TripleAddress,EmptyAddress);
          } else {
              callException("evalExp: IdFunK",7,4);
              break;
          }

          q0 = q0->sibling;
          /*safe loop measure*/
          SAFE_LOOP++;
          if(SAFE_LOOP > SAFE_LOOP_SIZE){
            callException("evalExp: IdFunK",10,4);
            return;
          }
          k--;
        }
        addTriple("FNCALL",st_lookupFnStart(node->attr.name),node->numberOfParameters,SymboltableAddress,ConstantNoAddress);

      }

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
