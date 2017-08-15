#ifndef _TRIPLE_H_
#define _TRIPLE_H_

#include "globals.h"
#include "util.h"

typedef enum {ADD, SUB, MULT, DIV, IFF, WHILEE, ATRIB, EQT, DIFF, GRT, LESST, GRTET, LESSTET, FNDECL, FNCALL, PARAM, RTRN, TRIPLE} operationType;
typedef enum {SymboltableAddress, TripleAddress, Constant} operandType;

int NUMBER_OF_TRIPLES;

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

triple* createTriple(operationType op, int fo, int so, operandType fot, operandType sot);

void addTriple(operationType op, int fo, int so, operandType fot, operandType sot);

void printTripleList();

void initializeTripleList();

#endif
