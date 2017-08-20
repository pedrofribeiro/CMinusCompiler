#include "assemblyCode.h"
#include "triple.h"

op recognizeOperation(triple* tr){

  /*operation initialized with WRONG values, to be used if it doesn't match*/
  op newOperation;
  strcpy(newOperation.ope,"NONE");
  newOperation.opeNumber = -1;

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

  /*models for recognition*/
  char models[16][6];
  strcpy(models[0],"+");
  strcpy(models[1],"-");
  strcpy(models[2],"*");
  strcpy(models[3],"/");
  strcpy(models[4],"RETURN");
  strcpy(models[5],"FNCALL");
  strcpy(models[6],"ATRIB");
  strcpy(models[7],"IF_F");
  strcpy(models[8],"GOTO");
  strcpy(models[9],"V_INDEX");
  strcpy(models[10],"==");
  strcpy(models[11],"!=");
  strcpy(models[12],">");
  strcpy(models[13],"<");
  strcpy(models[14],">=");
  strcpy(models[15],"<=");

  if (strcmp(models[0],rawOperation) == 0) {
      strcpy(newOperation.ope,"ADD");
      newOperation.opeNumber = 0;
      return newOperation;
  } else if (strcmp(models[1],rawOperation) == 0) {
      strcpy(newOperation.ope,"SUB");
      newOperation.opeNumber = 1;
      return newOperation;
  } else if (strcmp(models[2],rawOperation) == 0) {
      strcpy(newOperation.ope,"MUL");
      newOperation.opeNumber = 2;
      return newOperation;
  } else if (strcmp(models[3],rawOperation) == 0) {
      strcpy(newOperation.ope,"DIV");
      newOperation.opeNumber = 3;
      return newOperation;
  } else if (strcmp(models[4],rawOperation) == 0) {
      strcpy(newOperation.ope,"RET");
      newOperation.opeNumber = 4;
      return newOperation;
  } else if (strcmp(models[5],rawOperation) == 0) {
      strcpy(newOperation.ope,"CAL");
      newOperation.opeNumber = 5;
      return newOperation;
  } else if (strcmp(models[6],rawOperation) == 0) {
      strcpy(newOperation.ope,"ATR");
      newOperation.opeNumber = 6;
      return newOperation;
  } else if (strcmp(models[7],rawOperation) == 0) {
      strcpy(newOperation.ope,"IFF");
      newOperation.opeNumber = 7;
      return newOperation;
  } else if (strcmp(models[8],rawOperation) == 0) {
      strcpy(newOperation.ope,"GTO");
      newOperation.opeNumber = 8;
      return newOperation;
  } else if (strcmp(models[9],rawOperation) == 0) {
      strcpy(newOperation.ope,"VIN");
      newOperation.opeNumber = 9;
      return newOperation;
  } else if (strcmp(models[10],rawOperation) == 0) {
      strcpy(newOperation.ope,"EQL");
      newOperation.opeNumber = 10;
      return newOperation;
  } else if (strcmp(models[11],rawOperation) == 0) {
      strcpy(newOperation.ope,"DIF");
      newOperation.opeNumber = 11;
      return newOperation;
  } else if (strcmp(models[12],rawOperation) == 0) {
      strcpy(newOperation.ope,"GRT");
      newOperation.opeNumber = 12;
      return newOperation;
  } else if (strcmp(models[13],rawOperation) == 0) {
      strcpy(newOperation.ope,"LST");
      newOperation.opeNumber = 13;
      return newOperation;
  } else if (strcmp(models[14],rawOperation) == 0) {
      strcpy(newOperation.ope,"GTE");
      newOperation.opeNumber = 14;
      return newOperation;
  } else if (strcmp(models[15],rawOperation) == 0) {
      strcpy(newOperation.ope,"LTE");
      newOperation.opeNumber = 15;
      return newOperation;
  }
    callException("recognizeOperation",13,5);
    return newOperation;
}


void instructionMatch(triple* List){

  /* function label*/
  if ((List->firstOperandType == EmptyAddress) && (List->secondOperandType == EmptyAddress)) {
      addASM(createJTYPE(List->operation,111));
      return;
  }

  op newOperation = recognizeOperation(List);
  /*
  3: (gcd, _, _)                                  :                   gcd  :
  4: (==, st[16], 0)                              :                   ldi $acc, 0
                                                  :                   AND $acc, $acc, $r1
  5: (IF_F, t(4), t(8))                           :                   BNE
  6: (RETURN, st[3], _)
  7: (GOTO, t(15), _)
  */
  switch (newOperation.opeNumber) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:

        if ((List->firstOperandType == ConstantNoAddress) && (List->secondOperandType == ConstantNoAddress)) {

            /*packaging of constants*/
            int constantsResult = -999;

            if (newOperation.opeNumber == ADD) {
                constantsResult = List->firstOperand + List->secondOperand;
            } else if (newOperation.opeNumber == SUB) {
                constantsResult = List->firstOperand - List->secondOperand;
            } else if (newOperation.opeNumber == MUL) {
                constantsResult = List->firstOperand * List->secondOperand;
            } else if (newOperation.opeNumber == DIV){
                constantsResult = List->firstOperand / List->secondOperand;
            } else {
                callException("instructionMatch",17,5);
                break;
            }

            /*validates the packaging*/
            if (constantsResult == -999) { callException("instructionMatch",17,5); break; }

            addASM( createITYPE("li","$acc","$zero",constantsResult) );

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
  instructionMatch(List);

}
