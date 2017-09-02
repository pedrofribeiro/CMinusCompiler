#ifndef _TRIPLE_H_
#define _TRIPLE_H_

#include "globals.h"
#include "util.h"

typedef enum {SymboltableAddress, TripleAddress, ConstantNoAddress, EmptyAddress} operandType;

int NUMBER_OF_TRIPLES;
int NUMBER_OF_VARS;
int NUMBER_OF_GLOBALS;
int GLOBAL;

typedef struct ParamList {
  char parameterName[6];
  int parameterSTAddress;
}ParamList;

typedef struct triple{
  char operation[6];
  int firstOperand;
  int secondOperand;
  operandType firstOperandType;
  operandType secondOperandType;
  int tripleNumber;
  char functionName[6];
  ParamList params[3];
  struct triple* next;
}triple;

triple* tripleList;
triple* tempTriple;

triple* createTriple(char op[], int fo, int so, operandType fot, operandType sot);

void addTriple(char op[], int fo, int so, operandType fot, operandType sot);

int adjustTriple(int tn, int ope, int nv);

int insertParameter(int tn, int paramIndex, char paramName[], int paramAddr);

int setTripleFnName(int tripleNumber, char name[]);

int returnFunctionTriple(char* functionName);

triple* getTriple(char* functionName);

void printTripleList();

void initializeTripleList();

#endif
