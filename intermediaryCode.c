#include "intermediaryCode.h"
#include "triple.h"

void evalStmt(TreeNode *node){
  TreeNode *p0, *p1, *p2;

  switch (node->kind.stmt) {
    case DadoK:
      _VERBOSE_4 printf("[DadoK]\n");
      p0 = node->child[0];

      if (p0 == NULL) {
        callException("evalStmt: DadoK",6,4);
        return;
      }

      evalProgram(p0);
    break;
    case VarK:
      _VERBOSE_4 printf("[VarK]\n");
      if (GLOBAL == TRUE) {
          NUMBER_OF_GLOBALS = NUMBER_OF_GLOBALS + 1;
          addTriple("G_VAR",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),1,SymboltableAddress,ConstantNoAddress);
      } else {
          NUMBER_OF_VARS = NUMBER_OF_VARS + 1;
          addTriple("VAR",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),1,SymboltableAddress,ConstantNoAddress);
      }

    break;
    case VetK:
      _VERBOSE_4 printf("[VetK]\n");
      int k = node->child[0]->attr.val;
      if (GLOBAL == TRUE) {
          NUMBER_OF_GLOBALS = NUMBER_OF_GLOBALS + k;
          addTriple("G_VET",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),k,SymboltableAddress,ConstantNoAddress);
      } else {
          NUMBER_OF_VARS = NUMBER_OF_VARS + k;
          addTriple("VET",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),k,SymboltableAddress,ConstantNoAddress);
      }
    break;
    case ReturnK:
      _VERBOSE_4 printf("[ReturnK]\n");
      p0 = node->child[0];

      if (p0 == NULL){
        addTriple("RETURN",-1,st_lookupFnStart(CURRENT_FUNCTION),EmptyAddress,SymboltableAddress);
        return;
      }

      if (p0->kind.exp == ConstK) {
          evalProgram(p0);
          addTriple("RETURN",p0->attr.val,st_lookupFnStart(CURRENT_FUNCTION),ConstantNoAddress,SymboltableAddress);
      } else if(p0->kind.exp == IdK) {
          evalProgram(p0);
          addTriple("RETURN",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),st_lookupFnStart(CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
      } else if(p0->kind.exp == IdVetK) {
          evalProgram(p0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",vectorTriple,st_lookupFnStart(CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
      } else if(p0->kind.exp == IdFunK) {
          evalProgram(p0);
          int returnTriple;
          returnTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",returnTriple,st_lookupFnStart(CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
      } else if(p0->kind.exp == OpK) {
          evalProgram(p0);
          int returnTriple;
          returnTriple = NUMBER_OF_TRIPLES;
          addTriple("RETURN",returnTriple,st_lookupFnStart(CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
      } else if(p0->kind.exp == AtribK) { /*this may need a very serious further analysis*/
          evalProgram(p0);
          addTriple("RETURN",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),st_lookupFnStart(CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
      } else {
          callException("evalStmt: ReturnK",7,4);
      }
    break;
    case AtribK:
      _VERBOSE_4 printf("[AtribK]\n");
      p0 = node->child[0];
      p1 = node->child[1];

      evalProgram(p0);
      int leftOperand;
      leftOperand = NUMBER_OF_TRIPLES;
      evalProgram(p1);
      int rightOperand;
      rightOperand = NUMBER_OF_TRIPLES;

      if ((p0->kind.exp == IdK) && (p1->kind.exp == ConstK)) {
          addTriple("ATRIB",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),p1->attr.val,SymboltableAddress,ConstantNoAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdK)) {
          addTriple("ATRIB",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),st_lookupVarPosition(p1->attr.name,CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdVetK)) {
          addTriple("ATRIB",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == OpK)) {
          addTriple("ATRIB",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdK) && (p1->kind.exp == IdFunK)) {
          addTriple("ATRIB",st_lookupVarPosition(p0->attr.name,CURRENT_FUNCTION),rightOperand,SymboltableAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == ConstK)) {
          addTriple("ATRIB",leftOperand,p1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdK)) {
          addTriple("ATRIB",leftOperand,st_lookupVarPosition(p1->attr.name,CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdVetK)) {
          addTriple("ATRIB",leftOperand,st_lookupVarPosition(p1->attr.name,CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == OpK)) {
          addTriple("ATRIB",leftOperand,rightOperand,TripleAddress,TripleAddress);
      } else if ((p0->kind.exp == IdVetK) && (p1->kind.exp == IdFunK)) {
          addTriple("ATRIB",leftOperand,rightOperand,TripleAddress,TripleAddress);
      } else {
          callException("evalStmt: AtribK",7,4);
      }
    break;
    case IfK:
      _VERBOSE_4 printf("[IfK]\n");
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
        if (adjustment != 1) { callException("evalStmt: IfK",9,4); }
      //new code <
      needsAlignment(elseTriple,1);
      //new code >
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
            if (adjustment != 1) { callException("evalStmt: IfK",9,4); }
        //new code <
        needsAlignment(elseTriple,1);
        //new code >
          evalProgram(p2);
          int addrToJumpElsePart;
          addrToJumpElsePart = NUMBER_OF_TRIPLES+1;
            int secondAdjustment;
            secondAdjustment = adjustTriple(gotoTriple,1,addrToJumpElsePart);
            if (secondAdjustment != 1) { callException("evalStmt: IfK",9,4); }
        //new code <
        needsAlignment(addrToJumpElsePart,2);
        //new code >
      }
    break;
    case RepeatK:
      _VERBOSE_4 printf("[RepeatK]\n");
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
        if (adjustment != 1) { callException("evalStmt: RepeatK",9,4); }
    //new code <
    needsAlignment(addrToJump,1);
    needsAlignment(testTriple,2);
    //new code >

    break;
    case FunK:
      _VERBOSE_4 printf("[FunK]\n");

      strcpy(CURRENT_FUNCTION,node->attr.name);
      GLOBAL = FALSE;
      NUMBER_OF_VARS = 0;
      p0 = node->child[0]; /* arguments*/
      p1 = node->child[1]; /* function statements' code*/

      if (p0 == NULL)
        addTriple(node->attr.name,-999,-999,ConstantNoAddress,EmptyAddress);
      else
        addTriple(node->attr.name,-999,-999,ConstantNoAddress,ConstantNoAddress);

      int fnTriple = NUMBER_OF_TRIPLES;

      //new code <
      needsAlignment(fnTriple,1);
      //new code >

      evalProgram(p0); /*evaluates the arguments*/
      evalProgram(p1); /*evaluates the fn code*/

      int alignmentOne = adjustTriple(fnTriple,1,NUMBER_OF_VARS);
      if (alignmentOne != 1) { callException("evalStmt: FunK",9,4); }

      int alignmentTwo = adjustTriple(fnTriple,2,node->numberOfParameters);
      if (alignmentTwo != 1) { callException("evalStmt: FunK",9,4); }

      /*handling of the parameters*/
      int parameterInsertion;
      size_t i;
      p2 = p0;
      for (i = 0; i < node->numberOfParameters; i++) {
        parameterInsertion = insertParameter(fnTriple, i, p2->child[0]->attr.name, st_lookupVarPosition(p2->child[0]->attr.name,CURRENT_FUNCTION));
        if(parameterInsertion != 1){ callException("evalStmt: FunK",3,4); return; }
        p2 = p2->sibling;
      }

      GLOBAL = TRUE;

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
      _VERBOSE_4 printf("[ConstK]\n");
      //done as a subcase of another case
    break;
    case IdK:
      _VERBOSE_4 printf("[IdK]\n");
      //done as a subcase of another case
    break;
    case IdVetK:
      _VERBOSE_4 printf("[IdVetK]\n");
      q0 = node->child[0];

      if (q0 == NULL){
        callException("evalExp: IdVetK",8,4);
        break;
      }

      evalProgram(q0);
      int indexTriple;
      indexTriple = NUMBER_OF_TRIPLES;

      /*verifies if there is a local variable with the name in use*/
      if (st_lookupVarPosition(node->attr.name, CURRENT_FUNCTION) != -1) {

          if (q0->kind.exp == ConstK) {
              addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),q0->attr.val,SymboltableAddress,ConstantNoAddress);
          } else if (q0->kind.exp == IdK) {
              if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
                  addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
              } else if (st_lookupGlobal(q0->attr.name) != -1) {
                  addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),st_lookupGlobal(q0->attr.name),SymboltableAddress,SymboltableAddress);
              } else { callException("evalExp: IdVetK",31,5); }
          } else if (q0->kind.exp == IdVetK) {
              addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == OpK) {
              addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == AtribK) {
              addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == IdFunK) {
              addTriple("V_INDEX",st_lookupVarPosition(node->attr.name,CURRENT_FUNCTION),indexTriple,SymboltableAddress,TripleAddress);
          } else {
            callException("evalExp: IdVetK",7,4);
          }

      } else if (st_lookupGlobal(node->attr.name) != -1){

          if (q0->kind.exp == ConstK) {
              addTriple("V_INDEX",st_lookupGlobal(node->attr.name),q0->attr.val,SymboltableAddress,ConstantNoAddress);
          } else if (q0->kind.exp == IdK) {
              if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
                  addTriple("V_INDEX",st_lookupGlobal(node->attr.name),st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
              } else if (st_lookupGlobal(q0->attr.name) != -1) {
                  addTriple("V_INDEX",st_lookupGlobal(node->attr.name),st_lookupGlobal(q0->attr.name),SymboltableAddress,SymboltableAddress);
              } else { callException("evalExp: Atr",31,5); }
          } else if (q0->kind.exp == IdVetK) {
              addTriple("V_INDEX",st_lookupGlobal(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == OpK) {
              addTriple("V_INDEX",st_lookupGlobal(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == AtribK) {
              addTriple("V_INDEX",st_lookupGlobal(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
          } else if (q0->kind.exp == IdFunK) {
              addTriple("V_INDEX",st_lookupGlobal(node->attr.name),indexTriple,SymboltableAddress,TripleAddress);
          } else {
            callException("evalExp: IdVetK",7,4);
          }

      } else {
          callException("evalExp: IdVetK",32,5);
      }

    break;
    case OpK:
      _VERBOSE_4 printf("[OpK]\n");
      q0 = node->child[0];
      q1 = node->child[1];

      if ((q0->kind.exp == ConstK) && (q1->kind.exp == ConstK)) {
          addTriple(node->attr.name,q0->attr.val,q1->attr.val,ConstantNoAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == ConstK) && (q1->kind.exp == IdK)) {

          if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
              addTriple(node->attr.name,q0->attr.val,st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),ConstantNoAddress,SymboltableAddress);
          } else if(st_lookupGlobal(q1->attr.name) != -1) {
              addTriple(node->attr.name,q0->attr.val,st_lookupGlobal(q1->attr.name),ConstantNoAddress,SymboltableAddress);
          } else { callException("evalExp: OpK",32,4); }

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
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == ConstK)) {

          if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
              addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),q1->attr.val,SymboltableAddress,ConstantNoAddress);
          } else if (st_lookupGlobal(q0->attr.name) != -1) {
              addTriple(node->attr.name,st_lookupGlobal(q0->attr.name),q1->attr.val,SymboltableAddress,ConstantNoAddress);
          } else { callException("evalExp: OpK",31,5); }

      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdK)) {

          if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
              if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
                  addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
              } else if (st_lookupGlobal(q1->attr.name) != -1) {
                  addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),st_lookupGlobal(q1->attr.name),SymboltableAddress,SymboltableAddress);
              } else { callException("evalExp: OpK",31,5); }
          } else if (st_lookupGlobal(q0->attr.name) != -1){
              if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
                  addTriple(node->attr.name,st_lookupGlobal(q0->attr.name),st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),SymboltableAddress,SymboltableAddress);
              } else if (st_lookupGlobal(q1->attr.name) != -1) {
                  addTriple(node->attr.name,st_lookupGlobal(q0->attr.name),st_lookupGlobal(q1->attr.name),SymboltableAddress,SymboltableAddress);
              } else { callException("evalExp: OpK",31,5); }
          } else { callException("evalExp: OpK",31,5); }

      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdVetK)) {
          evalProgram(q1);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),vectorTriple,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == IdFunK)) {
          evalProgram(q1);
          int idfunkTriple;
          idfunkTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),idfunkTriple,SymboltableAddress,TripleAddress);
      } else if ((q0->kind.exp == IdK) && (q1->kind.exp == OpK)) {
         evalProgram(q1);
         int opkTriple;
         opkTriple = NUMBER_OF_TRIPLES;
         addTriple(node->attr.name,st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),opkTriple,SymboltableAddress,TripleAddress);
      }

       else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == ConstK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          addTriple(node->attr.name,vectorTriple,q1->attr.val,TripleAddress,ConstantNoAddress);
      } else if ((q0->kind.exp == IdVetK) && (q1->kind.exp == IdK)) {
          evalProgram(q0);
          int vectorTriple;
          vectorTriple = NUMBER_OF_TRIPLES;
          if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
              addTriple(node->attr.name,vectorTriple,st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
          } else if (st_lookupGlobal(q1->attr.name) != -1) {
              addTriple(node->attr.name,vectorTriple,st_lookupGlobal(q1->attr.name),TripleAddress,SymboltableAddress);
          } else { callException("evalExp: ",31,5); }
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
         if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
              addTriple(node->attr.name,idfunkTriple,st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
         } else if (st_lookupGlobal(q1->attr.name) != -1) {
              addTriple(node->attr.name,idfunkTriple,st_lookupGlobal(q1->attr.name),TripleAddress,SymboltableAddress);
         } else { callException("evalExp: ",31,5); }

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
         if (st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION) != -1) {
              addTriple(node->attr.name,opkTriple,st_lookupVarPosition(q1->attr.name,CURRENT_FUNCTION),TripleAddress,SymboltableAddress);
         } else if (st_lookupGlobal(q1->attr.name) != -1) {
              addTriple(node->attr.name,opkTriple,st_lookupGlobal(q1->attr.name),TripleAddress,SymboltableAddress);
         } else { callException("evalExp: ",31,5); }

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
      _VERBOSE_4 printf("[IdFunK]\n");
      q0 = node->child[0];

      /*treating the two special functions input() and output() differently.*/
      if (strcmp(node->attr.name,"input") == 0){
          addTriple("INPUT",-1,-1,EmptyAddress,EmptyAddress);
          break;
      } else if (strcmp(node->attr.name,"output") == 0) {

            if (q0->kind.exp == ConstK) {
                evalExp(q0);
                addTriple("OUTPUT",q0->attr.val,-1,ConstantNoAddress,EmptyAddress);
            } else if (q0->kind.exp == IdK) {
                evalExp(q0);
                if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
                    addTriple("OUTPUT",st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),-1,SymboltableAddress,EmptyAddress);
                } else if (st_lookupGlobal(q0->attr.name) != -1) {
                    addTriple("OUTPUT",st_lookupGlobal(q0->attr.name),-1,SymboltableAddress,EmptyAddress);
                } else { callException("evalExp: ",31,5); }
            } else if (q0->kind.exp == IdVetK) {
                evalExp(q0);
                addTriple("OUTPUT",st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),-1,SymboltableAddress,EmptyAddress);
            } else if (q0->kind.exp == OpK) {
                evalExp(q0);
                int paramTriple;
                paramTriple = NUMBER_OF_TRIPLES;
                addTriple("OUTPUT",paramTriple,-1,TripleAddress,EmptyAddress);
            } else if (q0->kind.exp == IdFunK) {
                evalExp(q0);
                int paramTriple;
                paramTriple = NUMBER_OF_TRIPLES;
                addTriple("OUTPUT",paramTriple,-1,TripleAddress,EmptyAddress);
            } else {
                callException("evalExp: IdFunK",7,4);
                break;
            }
          break;
      }

      if (q0 == NULL) { /*the function does not take any arguments*/

        addTriple("FNCALL",st_lookupFnStart(node->attr.name),node->numberOfParameters,SymboltableAddress,ConstantNoAddress);
        setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);

      } else { /*the function takes on arguments */

        int SAFE_LOOP = 0;
        int k = node->numberOfParameters;

        while (k > 0) {

          if (q0->kind.exp == ConstK) {
              evalExp(q0);
              addTriple("PARAM",q0->attr.val,st_lookupFnStart(node->attr.name),ConstantNoAddress,SymboltableAddress);
              setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
          } else if (q0->kind.exp == IdK) {
              evalExp(q0);
              if (st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION) != -1) {
                  addTriple("PARAM",st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),st_lookupFnStart(node->attr.name),SymboltableAddress,SymboltableAddress);
                  setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
              } else if (st_lookupGlobal(q0->attr.name) != -1) {
                  addTriple("PARAM",st_lookupGlobal(q0->attr.name),st_lookupFnStart(node->attr.name),SymboltableAddress,SymboltableAddress);
                  setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
              } else { callException("evalExp: ",31,5); }
          } else if (q0->kind.exp == IdVetK) {
              evalExp(q0);
              addTriple("PARAM",st_lookupVarPosition(q0->attr.name,CURRENT_FUNCTION),st_lookupFnStart(node->attr.name),SymboltableAddress,SymboltableAddress);
              setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
          } else if (q0->kind.exp == OpK) {
              evalExp(q0);
              int paramTriple;
              paramTriple = NUMBER_OF_TRIPLES;
              addTriple("PARAM",paramTriple,st_lookupFnStart(node->attr.name),TripleAddress,SymboltableAddress);
              setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
          } else if (q0->kind.exp == IdFunK) {
              evalExp(q0);
              int paramTriple;
              paramTriple = NUMBER_OF_TRIPLES;
              addTriple("PARAM",paramTriple,st_lookupFnStart(node->attr.name),TripleAddress,SymboltableAddress);
              setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);
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
        setTripleFnName(NUMBER_OF_TRIPLES,node->attr.name);

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

void alignmentHalt(){
  addTriple("HALT",-1,-1,EmptyAddress,EmptyAddress);
  needsAlignment(NUMBER_OF_TRIPLES,1);
}

void cleanTriples(){
  free(tripleList);
  tripleList = NULL;
  free(tempTriple);
  tempTriple = NULL;
}

void generateIntermediaryCode(TreeNode * t){
  initializeTripleList();
  initializeAlignments();

  TRACE_TREE_WALK = FALSE;
  CURRENT_FUNCTION = malloc(sizeof(char)*10);
  if (CURRENT_FUNCTION == NULL) { callException("generateIntermediaryCode: scope",3,4); return; }
  strcpy(CURRENT_FUNCTION,"global");

  evalProgram(t);
  alignmentHalt();
  printTripleList();
}
