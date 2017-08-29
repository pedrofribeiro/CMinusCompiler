#ifndef _RUN_TIME_ENVIRONMENT_
#define _RUN_TIME_ENVIRONMENT_

#include <stdarg.h>

typedef enum {$acc, $zero, $sp, $fp, $ra, $t1, $t2, $rv, $paramp, $gp, $none} Register;

int NUMBER_OF_VARIABLES;

typedef struct VARIABLE {
  int identifier;
  int basePosition;
  int availablePositions;
  struct POSITION* next;
} VARIABLE;

VARIABLE* tempVar;
VARIABLE* variblesList;

typedef struct REGISTER {
  int regNumber;
  int isFree;
  int storesTriple;
  int storesVar;
}REGISTER;

REGISTER* REGISTER_BANK;
REGISTER* tempRegister;

typedef struct MEMORY_UNIT {
  int positionNumber;
  int isFree;
  int storesVar;
  int contents;
}MEMORY_UNIT;

MEMORY_UNIT* MEMORY;
MEMORY_UNIT* tempMemory;

int setFP(int n);
int getFP();
int setGP(int n);
int getGP();
int setSP(int n);
int getSP();

int setVarPosition(int id, int np, int op);
int getVarPosition(int id);
void printVars();



#endif
