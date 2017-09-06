#include "runtime.h"

/*
Runtime environment for the CMinus Compiler
ALL MEMORY STRUCTURES' (registers, data and instruction memories) CONTROLS are defined here.
*/

void RuntimeEnvironmentStart(){
  initializeRegisterBank();
  initializeMemory();
  initializeVariables();
  setSP(0);
  setFP(0);
  setGP(0);
}

void RuntimeEnvironmentStop(){
  printRegisterBank();
  printMemory();
  cleanRuntimeEnvironment();
}

void initializeMemory(){
  MEMORY_COUNTER = 0;
  MEMORY = malloc(sizeof(MEMORY_UNIT)*MEMORY_SIZE);
  if (MEMORY == NULL) { callException("initializeMemory",3,5); return; }
  size_t i;
  for (i = 0; i < MEMORY_SIZE; i++) {
    MEMORY[i] = malloc(sizeof(MEMORY_UNIT)*1);
    if (MEMORY[i] == NULL) { callException("initializeMemory",3,5); return; }
    MEMORY[i]->positionNumber = MEMORY_COUNTER;
    MEMORY_COUNTER = MEMORY_COUNTER + 1;
    MEMORY[i]->isFree = TRUE;
    MEMORY[i]->storesVar = -1;
    MEMORY[i]->contents = -1;
  }
  tempMemory = malloc(sizeof(MEMORY_UNIT)*1);
  if (tempMemory == NULL) { callException("initializeMemory",3,5); return; }
}

void initializeRegisterBank(){
  REGISTER_COUNTER = 0;
  OCCUPIED_REGS = 0;
  REGS_TO_FREE = 0;
  REGISTER_BANK = malloc(sizeof(REGISTER)*REGISTER_BANK_SIZE);
  if (REGISTER_BANK == NULL) { callException("initializeRegisterBank",3,5); return; }
  size_t i;
  for (i = 0; i < REGISTER_BANK_SIZE; i++) {
    REGISTER_BANK[i] = malloc(sizeof(REGISTER)*1);
    if (REGISTER_BANK[i] == NULL) { callException("initializeRegisterBank",3,5); return; }
    REGISTER_BANK[i]->regNumber = REGISTER_COUNTER;
    REGISTER_COUNTER = REGISTER_COUNTER + 1;
    if (i < RESERVED_REGISTERS) REGISTER_BANK[i]->isFree = FALSE;
    else REGISTER_BANK[i]->isFree = TRUE;
    REGISTER_BANK[i]->storesTriple = -1;
    REGISTER_BANK[i]->storesVar = -1;
    REGISTER_BANK[i]->storesConst = -1;
    REGISTER_BANK[i]->contents = -1;
  }
  tempRegister = malloc(sizeof(REGISTER)*1);
  if (tempRegister == NULL) { callException("initializeRegisterBank",3,5); return; }
    tempRegister[0] = malloc(sizeof(REGISTER)*1);
    tempRegister[0]->regNumber = -1;
    tempRegister[0]->isFree = TRUE;
    tempRegister[0]->storesVar = -1;
    tempRegister[0]->storesTriple = -1;
    tempRegister[0]->storesConst = -1;
    tempRegister[0]->contents = -1;
}

void initializeVariables(){
  variablesList = createPosition(-2,0);
  if (variablesList == NULL) { callException("initializeVariables",20,5); return; }
  tempVar = createPosition(-1,0);
  if (tempVar == NULL) { callException("initializeVariables",20,5); return; }
}

void cleanRuntimeEnvironment(){
  free(MEMORY);
  MEMORY = NULL;
  free(REGISTER_BANK);
  REGISTER_BANK = NULL;
  free(tempRegister);
  tempRegister = NULL;
  free(tempMemory);
  tempMemory = NULL;
  free(tempVar);
  tempVar = NULL;
  free(variablesList);
  variablesList = NULL;
}


int allocateMemory(int n){
  if (n < 0) { callException("allocateMemory",8,5); return -999; } //cannot be <= because of 2 control variables.

  int freePositions = 0;
  int beginsAt = -1;
  int initialPosition = -1;

  int SAFE_LOOP = 0;
  while (freePositions < n) {
    size_t i;
    for (i = 0; i < MEMORY_SIZE; i++) {
      if (MEMORY[i]->isFree == TRUE) {
        freePositions++;
        beginsAt = i;
      }
    }
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("allocateMemory",10,5); return -999; }
  }

  initialPosition = beginsAt - freePositions + 1; //REVISE THIS. POSSIBLE SOURCE OF ERRORS.

  size_t i;
  for (i = initialPosition; i < initialPosition+n; i++) {
    MEMORY[i]->isFree = FALSE;
  }
  return initialPosition;
}


int allocateRegister(int type, int id){
  int i;
  for (i = RESERVED_REGISTERS; i < REGISTER_BANK_SIZE; i++) {
    if (REGISTER_BANK[i]->isFree == TRUE) {
      REGISTER_BANK[i]->isFree = FALSE;
      if (type == 0){
          REGISTER_BANK[i]->storesVar = -1;
          REGISTER_BANK[i]->storesTriple = id;
          REGISTER_BANK[i]->storesConst = -1;
      } else if (type == 1) {
          REGISTER_BANK[i]->storesVar = id;
          REGISTER_BANK[i]->storesTriple = -1;
          REGISTER_BANK[i]->storesConst = -1;
      } else {
        REGISTER_BANK[i]->storesVar = -1;
        REGISTER_BANK[i]->storesTriple = -1;
        REGISTER_BANK[i]->storesConst = id;
      }
      OCCUPIED_REGS++;
      return REGISTER_BANK[i]->regNumber;
    }
  }
  return -999;
}

void setRegister(int rn, int value){
  if (rn > REGISTER_BANK_SIZE) { callException("setRegister",8,5); return;}
  REGISTER_BANK[rn]->contents = value;
}

void listToDeallocate(int rn){
  if (rn > REGISTER_BANK_SIZE) { callException("listToDeallocate",8,5); return;}
  if (rn < RESERVED_REGISTERS) { callException("listToDeallocate",8,5); return;}
  if ( tempRegister[0]->isFree == TRUE ){
      tempRegister[0]->isFree = FALSE;
      tempRegister[0]->regNumber = rn;
      REGS_TO_FREE++;
  } else {
      int size = OCCUPIED_REGS + 1;
      tempRegister = realloc(tempRegister,sizeof(REGISTER)*size);
      tempRegister[REGS_TO_FREE] = malloc(sizeof(REGISTER)*1);
      tempRegister[REGS_TO_FREE]->isFree = FALSE;
      tempRegister[REGS_TO_FREE]->regNumber = rn;
      REGS_TO_FREE++;
  }
}

int deallocateRegister(int n){
  if (n > REGISTER_BANK_SIZE) { callException("listToDeallocate",8,5); return -999;}
  if (n < RESERVED_REGISTERS) { callException("listToDeallocate",8,5); return -999;}
  REGISTER_BANK[n]->isFree = TRUE;
  return 1;
}

int deallocateRegisters(){
  while (REGS_TO_FREE > 0) {
    REGISTER_BANK[tempRegister[--REGS_TO_FREE]->regNumber]->isFree = TRUE;
  }
  return 1;
}

void memoryHandler(int position, int value){
  if (position > MEMORY_SIZE) { callException("memoryHandler",8,5); return; }
  MEMORY[position]->contents = value;
}

int requestMemory(int identifier, int numberOfPositions){
  if (numberOfPositions > MEMORY_SIZE) { callException("requestMemory",8,5); return -999; }

  int initialPosition = allocateMemory(numberOfPositions);
  if (initialPosition == -999) { callException("requestMemory",26,5); return -999; }
  int finalPosition = initialPosition + numberOfPositions - 1;

  int i;
  for (i = initialPosition; i <= finalPosition; i++) {
    MEMORY[i]->storesVar = identifier;
  }

  return initialPosition;
}

int freeMemory(int initialPosition, int finalPosition){
  if( finalPosition >= MEMORY_SIZE ) { callException("freeMemory",8,5); return -999; }
  if( initialPosition < 0 ) { callException("freeMemory",8,5); return -999; }
  if( finalPosition < initialPosition ) { callException("freeMemory",28,5); return -999; }

  size_t i;
  for (i = initialPosition; i <= finalPosition; i++) {
    MEMORY[i]->isFree = TRUE;
  }
  return 1;
}

void printMemory(){
  size_t i;
  printf("\n\nDATA MEMORY\n");
  for (i = 0; i < MEMORY_SIZE; i++) {
    if (MEMORY[i]->isFree == TRUE)
      printf("%d [        ]\n",MEMORY[i]->positionNumber);
    else
      printf("%d [ %d: %d ]\n",MEMORY[i]->positionNumber,MEMORY[i]->storesVar,MEMORY[i]->contents);
  }
}


void printRegisterBank(){
  size_t i;
  printf("\n\nREGISTER BANK\n (var|tri|con)\n");
  for (i = 0; i < REGISTER_BANK_SIZE; i++) {
    if (REGISTER_BANK[i]->isFree == TRUE)
      printf("%d [             ]\n",REGISTER_BANK[i]->regNumber);
    else
      printf("%d [ (%d|%d|%d) : %d ]\n",REGISTER_BANK[i]->regNumber,REGISTER_BANK[i]->storesVar,REGISTER_BANK[i]->storesTriple,REGISTER_BANK[i]->storesConst,REGISTER_BANK[i]->contents);
  }
}


/* special purpose dedicated registers*/
void setSP(){
  REGISTER_BANK[1]->storesTriple = FALSE;
  REGISTER_BANK[1]->storesVar = FALSE;
  REGISTER_BANK[1]->contents = 1;
}

int getSP(){ return REGISTER_BANK[1]->contents; }

void setFP(){
  REGISTER_BANK[2]->storesTriple = FALSE;
  REGISTER_BANK[2]->storesVar = FALSE;
  REGISTER_BANK[2]->contents = 2;
}

int getFP(){ return REGISTER_BANK[2]->contents; }

void setGP(){
  REGISTER_BANK[3]->storesTriple = FALSE;
  REGISTER_BANK[3]->storesVar = FALSE;
  REGISTER_BANK[3]->contents = 3;
}

int getGP(){ return REGISTER_BANK[3]->contents; }


VARIABLE* createPosition(int id, int ap){
  VARIABLE* newVariable = malloc(sizeof(VARIABLE)*1);
  if (newVariable == NULL) { callException("createPosition",3,5); return NULL; }
  newVariable->identifier = id;
  newVariable->basePosition = requestMemory(id,ap);
  if(newVariable->basePosition == -999) { callException("createPosition",26,5); return NULL; }
  newVariable->availablePositions = ap;
  newVariable->next = NULL;
  return newVariable;
}

int getNamePosition(int id){
  if ((id == -2) || (id == -1)) return -999; /*special control variables have those ids*/
  else if (id < 0) { callException("getNamePosition",8,5); return -999; }

  tempVar = variablesList->next;
  int SAFE_LOOP = 0;

  while (tempVar != NULL) {
    if (tempVar->identifier == id) { return tempVar->basePosition; }
    tempVar = tempVar->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("getNamePosition",10,5); return -999; }
  }
  return -999;
}


int setNamePosition(int id, int ap){
  if (id < 0) { callException("setNamePosition",8,5); return -999; }

      /*tests if the var is already on the list*/
      int existenceTest = 0;
      existenceTest = getNamePosition(id);
      if (existenceTest != -999) {
        callException("setNamePosition",19,5);
        return -999;
      }

      /*if it's not, creates a new position for it*/
      VARIABLE * newVariable = createPosition(id,ap);
      if (newVariable == NULL) { callException("setNamePosition",20,5); return -999; }

      /*inserts the new position on the positions list*/
      if (variablesList->next == NULL) {

          variablesList->next = malloc(sizeof(VARIABLE)*1);
          if (variablesList->next == NULL) { callException("setNamePosition: variablesList",3,5); return -999; }
          variablesList->next = newVariable;
          return 1;

      } else {

          int SAFE_LOOP = 0;
          tempVar = variablesList->next;

          while (tempVar->next != NULL) {
            tempVar = tempVar->next;
            /*safe loop measure*/
            SAFE_LOOP++;
            if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setNamePosition",10,5); return -999; }
          }
          tempVar->next = newVariable;
          return 1;
      }
    callException("setNamePosition",33,5);
    return -999;
}

void printVars(){
  if(variablesList == NULL) { callException("printVars",4,5); return; }
  if(variablesList->next == NULL) { callException("printVars",4,5); return; }

  int SAFE_LOOP = 0;
  tempVar = variablesList->next;

  printf("\n\nVariables currently allocated on memory.\n");
  while (tempVar != NULL) {
    printf("id (%d), bp(%d), ap (%d)\n",tempVar->identifier,tempVar->basePosition,tempVar->availablePositions);
    tempVar = tempVar->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("printVars",10,5); return; }
  }
}
