#include "assemblyCode.h"
#include "triple.h"

CPU_OPERATIONS recognizeOperation(triple* tr){

  /*operation initialized with WRONG values, to be used if it doesn't match*/
  CPU_OPERATIONS newOperation = NONE;

  if (tr == NULL) { /*verify if the triple is null*/
    callException("recognizeOperation",11,5);
    return newOperation;
  }

  char rawOperation[6];
  strcpy(rawOperation,tr->operation);

  if (strcmp(rawOperation,tr->operation) != 0) { /*verify if the copy was done correctly*/
    callException("recognizeOperation",12,5);
    return newOperation;
  }

  printf("OPERATION = %s\n",rawOperation);
  if (strcmp("+",rawOperation) == 0) {
      newOperation = ADD;
      return newOperation;
  } else if (strcmp("-",rawOperation) == 0) {
      newOperation = SUB;
      return newOperation;
  } else if (strcmp("*",rawOperation) == 0) {
      newOperation = MUL;
      return newOperation;
  } else if (strcmp("/",rawOperation) == 0) {
      newOperation = DIV;
      return newOperation;
  } else if (strcmp("RETURN",rawOperation) == 0) {
      newOperation = RET;
      return newOperation;
  } else if (strcmp("FNCALL",rawOperation) == 0) {
      newOperation = CALL;
      return newOperation;
  } else if (strcmp("ATRIB",rawOperation) == 0) {
      newOperation = ATR;
      return newOperation;
  } else if (strcmp("IF_F",rawOperation) == 0) {
      newOperation = IF_F;
      return newOperation;
  } else if (strcmp("GOTO",rawOperation) == 0) {
      newOperation = GOTO;
      return newOperation;
  } else if (strcmp("V_INDEX",rawOperation) == 0) {
      newOperation = V_IN;
      return newOperation;
  } else if (strcmp("==",rawOperation) == 0) {
      newOperation = EQL;
      return newOperation;
  } else if (strcmp("!=",rawOperation) == 0) {
      newOperation = DIFE;
      return newOperation;
  } else if (strcmp(">",rawOperation) == 0) {
      newOperation = GRT;
      return newOperation;
  } else if (strcmp("<",rawOperation) == 0) {
      newOperation = LST;
      return newOperation;
  } else if (strcmp(">=",rawOperation) == 0) {
      newOperation = GTE;
      return newOperation;
  } else if (strcmp("<=",rawOperation) == 0) {
      newOperation = LTE;
      return newOperation;
  }
    callException("recognizeOperation",13,5);
    return newOperation;
}


void instructionMatch(triple* List){

  /* function label, creates a new Activation Record*/
  if ((List->firstOperandType == EmptyAddress) && (List->secondOperandType == EmptyAddress)) {
      addASM(createLTYPE(List->operation,111));
      addASM(createRTYPE(MOVE,$fp,$sp,$NONE));
      addASM(createITYPE(SW,$ra,$sp,0));
      addASM(createITYPE(ADDIU,$sp,$sp,1));
      generateAssembly(List->next);
      addASM(createITYPE(LW,$ra,$sp,-1));
      addASM(createITYPE(ADDIU,$sp,$sp,2)); //      CHANGE THIS IMMEDIATELY.
      addASM(createITYPE(LW,$fp,$sp,0));
      return;
  }

  CPU_OPERATIONS newOperation = recognizeOperation(List);
  if (newOperation == NONE) {
      callException("instructionMatch",13,5);
      return;
  }

  switch (newOperation) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:

        if ((List->firstOperandType == ConstantNoAddress) && (List->secondOperandType == ConstantNoAddress)) {

            /*packaging of constants*/
            int constantsResult = -999;

            if (newOperation == ADD) {
                constantsResult = List->firstOperand + List->secondOperand;
            } else if (newOperation == SUB) {
                constantsResult = List->firstOperand - List->secondOperand;
            } else if (newOperation == MUL) {
                constantsResult = List->firstOperand * List->secondOperand;
            } else if (newOperation == DIV){
                constantsResult = List->firstOperand / List->secondOperand;
            } else {
                callException("instructionMatch",17,5);
                break;
            }

            /*validates the packaging*/
            if (constantsResult == -999) { callException("instructionMatch",17,5); break; }

            addASM( createITYPE(LI,$acc,$zero,constantsResult) );

        } else if ((List->firstOperandType == ConstantNoAddress) && (List->secondOperandType == SymboltableAddress)) {

            /*needs a fully defined runtime environment to be correctly done*/

        } else if ((List->firstOperandType == SymboltableAddress) && (List->secondOperandType == ConstantNoAddress)) {

        } else if ((List->firstOperandType == SymboltableAddress) && (List->secondOperandType == SymboltableAddress)) {

        } else {
            callException("instructionMatch: ArithmeticOPs",16,5);
        }
    break;
    case RET:
    break;
    case CALL:
    break;
    case ATR:
    break;
    case IF_F:
    break;
    case GOTO:
    break;
    case V_IN:
    break;
    case EQL:
    break;
    case DIFE:
    break;
    case GRT:
    break;
    case LST:
    break;
    case GTE:
    break;
    case LTE:
    break;
    default:
        callException("instructionMatch",1,5);
    break;
  }

}


void generateAssembly(triple* List){

  if(List == NULL){
    callException("generateAssembly",4,5);
    return;
  }

  initializeASMList();
  while (List->next != NULL) {
    instructionMatch(List);
    List = List->next;
  }
  printASM(asmList);

}
