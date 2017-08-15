#include "globals.h"
#include "util.h"

typedef enum {ADD, SUB, MULT, DIV, IFF, WHILEE, ATRIB, EQT, DIFF, GRT, LESST, GRTET, LESSTET, FNDECL, FNCALL, PARAM, RTRN, TRIPLE} operationType;
typedef enum {SymboltableAddress, TripleAddress, Constant} operandType;

int NUMBER_OF_TRIPLES = 0;

typedef struct triple{
  operationType operation;
  int firstOperand;
  int secondOperand;
  operandType firstOperandType;
  operandType secondOperandType;
  int tripleNumber;
  struct triple* next;
}triple;

triple* tripleList;
triple* tempTriple;

triple* createTriple(operationType op, int fo, int so, operandType fot, operandType sot){
  triple* newTriple;
  newTriple = (triple*) malloc(sizeof(triple)*1);
  if(newTriple == NULL){
    callException("createTriple",3,4);
    return NULL;
  }
  NUMBER_OF_TRIPLES = NUMBER_OF_TRIPLES + 1;
  newTriple->operation = op;
  newTriple->firstOperand = fo;
  newTriple->secondOperand = so;
  newTriple->firstOperandType = fot;
  newTriple->secondOperandType = sot;
  newTriple->tripleNumber = NUMBER_OF_TRIPLES;
  newTriple->next = NULL;
  return newTriple;
}

void addTriple(operationType op, int fo, int so, operandType fot, operandType sot) {
  if(tripleList->next == NULL){
    tripleList->next = (triple*) malloc(sizeof(triple)*1);
    if(tripleList->next == NULL){
      callException("addTriple",3,4);
      return;
    }
  }else{
    tempTriple = tripleList->next;
    while (tempTriple != NULL) {
      tempTriple = tempTriple->next;
    }
    tempTriple->next = createTriple(op,fo,so,fot,sot);
  }
}

void printTripleList(triple* list){
  if(list == NULL){
    callException("printTripleList",4,4);
    return;
  }
  tempTriple = tripleList->next;
  while (tempTriple != NULL) {
    /*I chose to make the code clearer to read instead of faster to compile. */
    if((tempTriple->firstOperandType == Constant) && (tempTriple->secondOperandType == Constant))
      printf("%d: (%d, %d, %d)\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == Constant) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%d, %d, st[%d])\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == Constant) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%d, %d, t(%d))\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == Constant))
      printf("%d: (%d, st[%d], %d)\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%d, st[%d], st[%d])\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == SymboltableAddress) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%d, st[%d], t(%d))\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == Constant))
      printf("%d: (%d, t(%d), %d)\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == SymboltableAddress))
      printf("%d: (%d, t(%d), st[%d])\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else if((tempTriple->firstOperandType == TripleAddress) && (tempTriple->secondOperandType == TripleAddress))
      printf("%d: (%d, t(%d), t(%d))\n",tempTriple->tripleNumber, tempTriple->firstOperand, tempTriple->secondOperand);
    else callException("printTripleList",5,4);

    tempTriple = tempTriple->next;
  }
}

void initializeTripleList(){
  tripleList = createTriple(TRIPLE,-1,-1,Constant,Constant);
  tempTriple = createTriple(TRIPLE,-1,-1,Constant,Constant);
}

void evalStmt(TreeNode *node);

void evalExp(TreeNode *node);

void evalProgram(TreeNode *node);
