#include "triple.h"

triple* createTriple(char op[], int fo, int so, operandType fot, operandType sot){
  triple* newTriple;
  newTriple = (triple*) malloc(sizeof(triple)*1);
  if(newTriple == NULL){
    callException("createTriple",3,4);
    return NULL;
  }
  NUMBER_OF_TRIPLES = NUMBER_OF_TRIPLES + 1;
  strcpy(newTriple->operation,op);
  newTriple->firstOperand = fo;
  newTriple->secondOperand = so;
  newTriple->firstOperandType = fot;
  newTriple->secondOperandType = sot;
  newTriple->tripleNumber = NUMBER_OF_TRIPLES;
  newTriple->next = NULL;
  return newTriple;
}

void addTriple(char op[], int fo, int so, operandType fot, operandType sot) {
  if(tripleList->next == NULL){
    tripleList->next = createTriple(op,fo,so,fot,sot);
    return;
  }
  int SAFE_LOOP = 0;
  tempTriple = tripleList->next;
  while (tempTriple->next != NULL) {
    tempTriple = tempTriple->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE){
      callException("addTriple",10,4);
      return;
    }
  }
  tempTriple->next = createTriple(op,fo,so,fot,sot);
}

int adjustTriple(int tn, int ope, int nv){
  if ((tn <= 0) || (tn > NUMBER_OF_TRIPLES)) {
    callException("adjustTriple: number of triple",8,4);
    return -1;
  }
  if ((ope < 1) || (ope > 2)){
    callException("adjustTriple: operand",8,4);
    return -1;
  }
  int SAFE_LOOP = 0;
  tempTriple = tripleList->next;
  while (tempTriple->next != NULL) {
    if (tempTriple->tripleNumber == tn) {
      if (ope == 1) { tempTriple->firstOperand = nv; }
      else if (ope == 2) { tempTriple->secondOperand = nv; }
      return 1;
    } else { tempTriple = tempTriple->next; }
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE){
      callException("adjustTriple",10,4);
      return -1;
    }
  }
  return -1;
}

void printTripleList(){
  if(tripleList == NULL){
    callException("printTripleList",4,4);
    return;
  }
  int SAFE_LOOP = 0;
  tempTriple = tripleList->next;
  while (tempTriple != NULL) {
    /*I chose to make the code clearer to read instead of faster to compile. */
    if((tempTriple->firstOperandType == ConstantNoAddress) && (tempTriple->secondOperandType == ConstantNoAddress))
      printf("%d: (%s, %d, %d)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == ConstantNoAddress) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%s, %d, st[%d])\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == ConstantNoAddress) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%s, %d, t(%d))\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == ConstantNoAddress) && (tempTriple->secondOperandType == EmptyAddress))
      printf("%d: (%s, %d, _)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == ConstantNoAddress))
      printf("%d: (%s, st[%d], %d)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%s, st[%d], st[%d])\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%s, st[%d], t(%d))\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == EmptyAddress))
      printf("%d: (%s, st[%d], _)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == ConstantNoAddress))
      printf("%d: (%s, t(%d), %d)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%s, t(%d), st[%d])\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%s, t(%d), t(%d))\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == EmptyAddress))
      printf("%d: (%s, t(%d), _)\n",tempTriple->tripleNumber, tempTriple->operation, tempTriple->firstOperand);
    else if((tempTriple->firstOperandType == EmptyAddress) && (tempTriple->secondOperandType == EmptyAddress))
      printf("%d: (%s, _, _)\n",tempTriple->tripleNumber, tempTriple->operation);
    else callException("printTripleList",5,4);

    tempTriple = tempTriple->next;

    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE){
      callException("adjustTriple",10,4);
      return;
    }
  }
}

void initializeTripleList(){
  NUMBER_OF_TRIPLES = -2;
  tripleList = createTriple("TripleListHead",-1,-1,ConstantNoAddress,ConstantNoAddress);
  tempTriple = createTriple("TempTriple",-1,-1,ConstantNoAddress,ConstantNoAddress);
}
