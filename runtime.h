#ifndef _RUN_TIME_ENVIRONMENT_
#define _RUN_TIME_ENVIRONMENT_

#include "globals.h"
#include "util.h"

typedef enum {$acc, $zero, $sp, $fp, $ra, $t1, $t2, $rv, $paramp, $gp, $none} Register;

int OCCUPIED_REGS;

typedef struct VARIABLE {
  int identifier;
  int basePosition;
  int availablePositions;
  struct VARIABLE* next;
} VARIABLE;

VARIABLE* tempVar;
VARIABLE* variablesList;

typedef struct REGISTER {
  int regNumber;
  int isFree;
  int storesTriple;
  int storesVar;
  int storesConst;
  int contents;
}REGISTER;

REGISTER** REGISTER_BANK;
REGISTER** tempRegister;

typedef struct MEMORY_UNIT {
  int positionNumber;
  int isFree;
  int storesVar;
  int contents;
}MEMORY_UNIT;

MEMORY_UNIT** MEMORY;
MEMORY_UNIT* tempMemory;

void initializeMemory();
void initializeRegisterBank();
void initializeVariables();
void cleanRuntimeEnvironment();
int allocateMemory(int n);
int freeMemory(int initialPosition, int finalPosition);
int allocateRegister();
int deallocateRegister(int n);
void listToDeallocate(int rn);
int deallocateRegisters();
void memoryHandler(int position, int value);
int requestMemory(int identifier, int numberOfPositions);
void printMemory();
void printRegisterBank();

void setFP();
int getFP();
void setGP();
int getGP();
void setSP();
int getSP();

VARIABLE* createPosition(int id, int ap);
int setVarPosition(int id, int np);
int getVarPosition(int id);
void printVars();



#endif
