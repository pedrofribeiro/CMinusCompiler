/*
Runtime environment for the CMinus Compiler
ALL MEMORY STRUCTURES' (registers and data & instruction memories) CONTROL are defined here.
*/

void initializeMemory(){
  MEMORY_COUNTER = 0;
  MEMORY = (MEMORY_UNIT*) malloc(sizeof(MEMORY_UNIT)*MEMORY_SIZE);
  if (MEMORY == NULL) { callException("initializeMemory",3,5); return; }
  for (size_t i = 0; i < MEMORY_SIZE; i++) {
    MEMORY[i]->positionNumber = MEMORY_COUNTER;
    MEMORY_COUNTER = MEMORY_COUNTER + 1;
    MEMORY[i]->isFree = TRUE;
    MEMORY[i]->storesVar = -1;
    MEMORY[i]->contents = -1;
  }
  tempMemory = (MEMORY_UNIT*) malloc(sizeof(MEMORY_UNIT)*1);
  if (tempMemory == NULL) { callException("initializeMemory",3,5); return; }
}

void initializeRegisterBank(){
  REGISTER_COUNTER = 0;
  REGISTER_BANK = (REGISTER*) malloc(sizeof(REGISTER)*REGISTER_BANK_SIZE);
  if (REGISTER_BANK == NULL) { callException("initializeRegisterBank",3,5); return; }
  for (size_t i = 0; i < REGISTER_BANK_SIZE; i++) {
    REGISTER_BANK[i]->regNumber = REGISTER_COUNTER;
    REGISTER_COUNTER = REGISTER_COUNTER + 1;
    REGISTER_BANK[i]->isFree = (i < RESERVED_REGISTERS) ? TRUE : FALSE;;
    REGISTER_BANK[i]->storesTriple = -1;
    REGISTER_BANK[i]->storesVar = -1;
  }
  tempRegister = (REGISTER*) malloc(sizeof(REGISTER)*1);
  if (tempRegister == NULL) { callException("initializeRegisterBank",3,5); return; }
}

void cleanRuntimeEnvironment(){
  free(MEMORY);
  MEMORY = NULL;
  free(REGISTER_BANK);
  REGISTER_BANK = NULL;
}

int allocateMemory(int n){
  if (n <= 0) { callException("allocateMemory",8,5); return -999; }
  //if (n > MEMORY_SIZE - MEMORY_COUNTER) { callException("allocateMemory",26,5); return; }
  int freePositions = 0;
  int beginsAt = -1;
  int initialPosition = -1;

  int SAFE_LOOP = 0;

  while (freePositions < n) {
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
      if (MEMORY[i]->isFree == TRUE) {
        freePositions++;
        beginsAt = i;
      }
    }
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("allocateMemory",10,5); return -999; }
  }

  initialPosition = beginsAt - freePositions + 1; //REVISE THIS. POSSIBLE SOURCE OF ERRORS.

  for (size_t i = initialPosition; i < initialPosition+n; i++) {
    MEMORY[i]->isFree = FALSE;
  }

  return initialPosition;
}


int allocateRegister(){
  for (int i = RESERVED_REGISTERS; i < REGISTER_BANK_SIZE; i++) {
    if (REGISTER_BANK[i]->isFree == TRUE) {
      REGISTER_BANK[i]->isFree = FALSE;
      return i;
    }
  }
  callException("allocateRegister",27,5);
  return -999;
}


MEMORY_UNIT* memoryHandler(int ip, int fp){
  if (ip > fp) { callException("memoryHandler",28,5); return NULL; }
  return MEMORY[i];
}


void useMemory(int argNumber, ...){
  if (np > MEMORY_SIZE) { callException("useMemory",8,5); return; }

  va_list listOfArguments;
  va_start(listOfArguments, argNumber);

  int initialPosition = allocateMemory(np);
  if (np == -999) { callException("useMemory",26,5); return; }
  int finalPosition = initialPosition + np - 1;
  tempMemory = memoryHandler(initialPosition,finalPosition);

  for (int i = initialPosition; i < finalPosition; i++) {
    MEMORY[i]->storesVar = va_arg(listOfArguments, int);
    MEMORY[i]->contents = va_arg(listOfArguments, int);
  }

  va_end(listOfArguments);
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
  VARIABLE* newVariable = (VARIABLE*) malloc(sizeof(VARIABLE)*1);
  if (newVariable == NULL) { callException("createPosition",3,5); return NULL; }
  newVariable->identifier = id;
  _VERBOSE_5 printf("Memory position %d allocated for id (%d)\n",MEMORY,id);
  newVariable->basePosition = requestMemory(ap);
  newVariable->availablePositions = ap;
  newVariable->next = NULL;
  return newVariable;
}

int getVarPosition(int id){
  if ((id == -2) || (id == -1)) return -999; /*special control variables have those ids*/
  else if (id < 0) { callException("getVarPosition",8,5); return -999; }

  tempVar = variablesList->next;
  int SAFE_LOOP = 0;

  while (tempVar != NULL) {
    if (tempVar->identifier == id) { return tempVar->basePosition; }
    tempVar = tempVar->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("getVarPosition",10,5); return -999; }
  }
  return -999;
}


int setVarPosition(int id, int ap, int op){
/*if op == 0, ap is how many positions in memory id needs
if op == 1, ap is the new base position of id.
*/

  if (id < 0) { callException("setVarPosition",8,5); return -999; }

  /*creates a new var*/
  if (op == 0) {

      /*tests if the var is already on the list*/
      int existenceTest = 0;
      existenceTest = getVarPosition(id);
      if (existenceTest != -999) {
        callException("setVarPosition",19,5);
        return -999;
      }

      /*if it's not, creates a new position for it*/
      VARIABLE * newVariable = createPosition(id,ap);
      if (newVariable == NULL) { callException("setVarPosition",20,5); return -999; }

      /*inserts the new position on the positions list*/
      if (variablesList->next == NULL) {

          variablesList->next = (VARIABLE*) malloc(sizeof(VARIABLE)*1);
          if (variablesList->next == NULL) { callException("setVarPosition: variablesList",3,5); return -999; }
          variablesList->next = newVariable;
          return 1;

      } else {

          int SAFE_LOOP = 0;
          tempVar = variablesList->next;

          while (tempVar->next != NULL) {
            tempVar = tempVar->next;
            /*safe loop measure*/
            SAFE_LOOP++;
            if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setVarPosition",10,5); return -999; }
          }
          tempVar->next = newVariable;
          return 1;

      }

  } else if (op == 1) { /*redefines the base position of an existing var*/

    /*tests if the var is already on the list*/
    int nexistenceTest = 0;
    nexistenceTest = getVarPosition(id);
    if (nexistenceTest == -999) {
      callException("setPosition",22,5);
      return -999;
    }

      int SAFE_LOOP = 0;
      tempVar = variablesList->next;

      while (tempVar != NULL) {
        if (tempVar->identifier == id) { tempVar->basePosition = ap; return 1; }
        tempVar = tempVar->next;
        /*safe loop measure*/
        SAFE_LOOP++;
        if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setVarPosition",10,5); return -999; }
      }
      return -999;

  } else {
      callException("setVarPosition",22,5);
  }

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
