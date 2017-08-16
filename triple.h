#ifndef _TRIPLE_H_
#define _TRIPLE_H_

#include "globals.h"
#include "util.h"

typedef enum {SymboltableAddress, TripleAddress, ConstantNoAddress, EmptyAddress} operandType;

int NUMBER_OF_TRIPLES;

typedef struct triple{
  char operation[6];
  int firstOperand;
  int secondOperand;
  operandType firstOperandType;
  operandType secondOperandType;
  int tripleNumber;
  struct triple* next;
}triple;

triple* tripleList;
triple* tempTriple;

triple* createTriple(char op[], int fo, int so, operandType fot, operandType sot);

void addTriple(char op[], int fo, int so, operandType fot, operandType sot);

void printTripleList();

void initializeTripleList();

#endif
