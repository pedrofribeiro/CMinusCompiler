#include "ASM.h"


ASM_INSTR* createRTYPE(Operation cop, Register rd, Register r1, Register r2){
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createRTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = RTYPE;
  newInstruction->needsAlignment = FALSE;
  newInstruction->rtype.cpu_operation = cop;
  newInstruction->rtype.rd = rd;
  newInstruction->rtype.r1 = r1;
  newInstruction->rtype.r2 = r2;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  _VERBOSE_5 printf("[R] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->rtype.rd,newInstruction->rtype.r1,newInstruction->rtype.r2);
  return newInstruction;
}


ASM_INSTR* createITYPE(Operation cop, Register rd, Register r1, int imm){
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createITYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = ITYPE;
  newInstruction->needsAlignment = FALSE;
  newInstruction->itype.cpu_operation = cop;
  newInstruction->itype.rd = rd;
  newInstruction->itype.r1 = r1;
  newInstruction->itype.immediate = imm;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  _VERBOSE_5 printf("[I] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->itype.rd,newInstruction->itype.r1,newInstruction->itype.immediate);
  return newInstruction;
}


ASM_INSTR* createJTYPE(Operation cop, int addr){
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createJTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = JTYPE;
  newInstruction->needsAlignment = FALSE;
  newInstruction->jtype.cpu_operation = cop;
  newInstruction->jtype.address = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  _VERBOSE_5 printf("[J] %d: %d %d \n",newInstruction->asmNumber,newInstruction->jtype.cpu_operation,newInstruction->jtype.address);
  return newInstruction;
}


ASM_INSTR* createLTYPE(char cop[], int addr){
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createLTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = LTYPE;
  newInstruction->needsAlignment = FALSE;
  strcpy(newInstruction->ltype.functionName,cop);
  newInstruction->ltype.asmAddress = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  _VERBOSE_5 printf("[L] %d: %s %d ",newInstruction->asmNumber,newInstruction->ltype.functionName,newInstruction->ltype.asmAddress);
  return newInstruction;
}


int getCurrentASMNumber(){
  return NUMBER_OF_ASM;
}


void addASM(ASM_INSTR* newInstruction){
  if (newInstruction == NULL) {
    callException("addASM",14,5);
    return;
  }

  if (asmList->next == NULL) {
      asmList->next = malloc(sizeof(ASM_INSTR)*1);
      if (asmList->next == NULL) {
        callException("addASM",3,5);
        return;
      } else {
        asmList->next = newInstruction;
      }
  } else {

      int SAFE_LOOP = 0;
      tempAsm = asmList->next;
      while (tempAsm->next != NULL) {
        tempAsm = tempAsm->next;
        /*safe loop measure*/
        SAFE_LOOP++;
        if(SAFE_LOOP > SAFE_LOOP_SIZE){
          callException("addASM",10,5);
          return;
        }
      }

      tempAsm->next = newInstruction;
  }

}


int adjustASM(int an, int field, int nv){
  if ((an <= 0) || (an > NUMBER_OF_ASM)) {
    callException("adjustASM: number of asm",8,5);
    return -1;
  }
  if ((field < 1) || (field > 3)){
    callException("adjustASM: field",8,5);
    return -1;
  }
  int SAFE_LOOP = 0;
  tempAsm = asmList->next;
  while (tempAsm->next != NULL) {
    if (tempAsm->asmNumber == an) {
      if (field == 1) { tempAsm->itype.immediate = nv; }
      else if (field == 2) { tempAsm->jtype.address = nv; }
      else if (field == 3) { tempAsm->ltype.asmAddress = nv; }
      _VERBOSE_5 printf("Endereço de salto alterado para %d.\n",nv);
      return 1;
    } else { tempAsm = tempAsm->next; }
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE){
      callException("adjustTriple",10,4);
      return -1;
    }
  }
  return -1;
}


char* toChar(Operation op, Register reg){
  int control;
  char* resultString = malloc(sizeof(char)*6);

  if ((op != NONE) && (reg == $none)) control = 1;
  else if ((op == NONE) && (reg != $none)) control = 2;
  else if ((op != NONE) && (reg != $none)){ callException("toChar: dif",21,5); return NULL; }
  else if ((op == NONE) && (reg == $none)){ callException("toChar: eq",21,5); return NULL; }

  switch (control) {
    case 1:

        switch (op) {
          case ADD:
            strcpy(resultString,"ADD");
          break;
          case SUB:
            strcpy(resultString,"SUB");
          break;
          case MUL:
            strcpy(resultString,"MUL");
          break;
          case DIV:
            strcpy(resultString,"DIV");
          break;
          case ADDIU:
            strcpy(resultString,"ADDIU");
          break;
          case LW:
            strcpy(resultString,"LW");
          break;
          case LI:
            strcpy(resultString,"LI");
          break;
          case SW:
            strcpy(resultString,"SW");
          break;
          case JUMP:
            strcpy(resultString,"J");
          break;
          case BEQ:
            strcpy(resultString,"BEQ");
          break;
          case BNE:
            strcpy(resultString,"BNE");
          break;
          case BGT:
            strcpy(resultString,"BGT");
          break;
          case BLT:
            strcpy(resultString,"BLT");
          break;
          case BGE:
            strcpy(resultString,"BGE");
          break;
          case BLE:
            strcpy(resultString,"BLE");
          break;
          case AND:
            strcpy(resultString,"AND");
          break;
          case XOR:
            strcpy(resultString,"XOR");
          break;
          case MOVE:
            strcpy(resultString,"MOV");
          break;
          case JR:
            strcpy(resultString,"JR");
          break;
          case JAL:
            strcpy(resultString,"JAL");
          break;
          case INPUT:
            strcpy(resultString,"IPT");
          break;
          case OUTPUT:
            strcpy(resultString,"OTP");
          break;
          case NONE:
            strcpy(resultString,"");
          break;
          case LST:
            printf("PASSOU UM LST DE ALGUM LUGAR\n");
            return NULL;
          break;
          default:
            callException("toChar: op",1,5);
            printf("MISMATCHED OP: %d\n",op);
            return NULL;
          break;
        }

    break;
    case 2:

        switch (reg) {
          case $sp:
            strcpy(resultString,"$sp");
          break;
          case $fp:
            strcpy(resultString,"$fp");
          break;
          case $ra:
            strcpy(resultString,"$ra");
          break;
          case $gp:
            strcpy(resultString,"$gp");
          break;
          case $zero:
            strcpy(resultString,"$zr");
          break;
          case $acc:
            strcpy(resultString,"$acc");
          break;
          case $paramp:
            strcpy(resultString,"$ap");
          break;
          case $t1:
            strcpy(resultString,"$t1");
          break;
          case $t2:
            strcpy(resultString,"$t2");
          break;
          case $rv:
            strcpy(resultString,"$rv");
          break;
          case $one:
            strcpy(resultString,"$one");
          break;
          case $output:
            strcpy(resultString,"$otp");
          break;
          case $none:
            strcpy(resultString,"");
          break;
          default:
            callException("toChar: op",13,5);
            printf("FOUND: < %d > \n",reg);
          break;
        }

    break;
  }
  return resultString;
}


void toBeAligned(int asmNumber){
  if ((asmNumber < 0) || (asmNumber > NUMBER_OF_ASM)) { callException("toBeAligned",8,5); return; }

  int SAFE_LOOP = 0;
  tempAsm = asmList->next;

  while (tempAsm != NULL) {
    if (tempAsm->asmNumber == asmNumber) {
      tempAsm->needsAlignment = TRUE;
      _VERBOSE_5 printf("A instrução %d precisa ter seu endereço de destino alinhado.\n",asmNumber);
    }

    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("toBeAligned",10,5); return; }

    tempAsm = tempAsm->next;
  }
}

void logicalBranch(Operation op, int tripleNumber){
  switch (op) {
    case EQL:
      addASM( createRTYPE( SUB, $acc, $acc, $t1 ) );
      addASM( createITYPE( BNE, $acc, $zero, tripleNumber ) );
    break;
    case DIFE:
      addASM( createRTYPE( SUB, $acc, $acc, $t1 ) );
      addASM( createITYPE( BEQ, $acc, $zero, tripleNumber ) );
    break;
    case GRT:
      addASM( createITYPE( BLE, $acc, $t1, tripleNumber ) );
    break;
    case LST:
      addASM( createITYPE( BGE, $acc, $t1, tripleNumber ) );
    break;
    case GTE:
      addASM( createITYPE( BLT, $acc, $t1, tripleNumber ) );
    break;
    case LTE:
      addASM( createITYPE( BGT, $acc, $t1, tripleNumber ) );
    break;
    default:
    break;
  }
  toBeAligned(getCurrentASMNumber());
}


void printASM(int printMode){
  if (asmList == NULL) {
      callException("printASM",4,5);
      return;
  } else if (asmList->next == NULL) {
      callException("printASM",4,5);
      return;
  }
  int SAFE_LOOP = 0;
  tempAsm = asmList->next;
  while (tempAsm != NULL) {
      /*safe loop measure*/
      SAFE_LOOP++;
      if(SAFE_LOOP > SAFE_LOOP_SIZE){
        callException("printASM",10,5);
        return;
      }
      switch (printMode) {
        case 0:
            switch (tempAsm->type) {
              case RTYPE:
                  printf("%d: %s %s %s %s \n",tempAsm->asmNumber,toChar(tempAsm->rtype.cpu_operation,$none),toChar(NONE,tempAsm->rtype.rd),toChar(NONE,tempAsm->rtype.r1),toChar(NONE,tempAsm->rtype.r2));
              break;
              case ITYPE:
                  printf("%d: %s %s %s %d \n",tempAsm->asmNumber,toChar(tempAsm->itype.cpu_operation,$none),toChar(NONE,tempAsm->itype.rd),toChar(NONE,tempAsm->itype.r1),tempAsm->itype.immediate);
              break;
              case JTYPE:
                  if (tempAsm->jtype.cpu_operation == JR)
                    printf("%d: %s %s \n",tempAsm->asmNumber,toChar(tempAsm->jtype.cpu_operation,$none),toChar(NONE,tempAsm->jtype.address));
                  else
                    printf("%d: %s %d \n",tempAsm->asmNumber,toChar(tempAsm->jtype.cpu_operation,$none),tempAsm->jtype.address);
              break;
              case LTYPE:
                  printf("%d: %s %d \n",tempAsm->asmNumber,tempAsm->ltype.functionName,tempAsm->ltype.asmAddress);
              break;
              default:
                  callException("printASM",15,5);
              break;
            }
        break;
        case 1:
            switch (tempAsm->type) {
              case RTYPE:
                  printf("%d: %d %d %d %d \n",tempAsm->asmNumber,tempAsm->rtype.cpu_operation,tempAsm->rtype.rd,tempAsm->rtype.r1,tempAsm->rtype.r2);
              break;
              case ITYPE:
                  printf("%d: %d %d %d %d \n",tempAsm->asmNumber,tempAsm->itype.cpu_operation,tempAsm->itype.rd,tempAsm->itype.r1,tempAsm->itype.immediate);
              break;
              case JTYPE:
                  printf("%d: %d %d \n",tempAsm->asmNumber,tempAsm->jtype.cpu_operation,tempAsm->jtype.address);
              break;
              case LTYPE:
                  printf("%d: %s %d \n",tempAsm->asmNumber,tempAsm->ltype.functionName,tempAsm->ltype.asmAddress);
              break;
              default:
                  callException("printASM",15,5);
              break;
            }
        break;
        default:
            callException("printASM: printMode",1,5);
            return;
        break;
      }
      tempAsm = tempAsm->next;
	}
}


Operation getOperation(triple *tr){
  if (tr == NULL) {
      callException("getOperation",3,5);
      return NONE;
  }
  char inputOperation[6];
  sprintf(inputOperation,"%s",tr->operation);
  if (strcmp(inputOperation,tr->operation) != 0) {
    callException("getOperation",12,5);
    return NONE;
  }
  /* finding out the operation*/
  if (strcmp(inputOperation,"+") == 0) {
    return ADD;
  } else if (strcmp(inputOperation,"-") == 0) {
    return SUB;
  } else if (strcmp(inputOperation,"*") == 0) {
    return MUL;
  } else if (strcmp(inputOperation,"/") == 0) {
    return DIV;
  } else if (strcmp(inputOperation,"RETURN") == 0) {
    return RET;
  } else if (strcmp(inputOperation,"FNCALL") == 0) {
    return CALL;
  } else if (strcmp(inputOperation,"ATRIB") == 0) {
    return ATR;
  } else if (strcmp(inputOperation,"IF_F") == 0) {
    return IF_F;
  } else if (strcmp(inputOperation,"GOTO") == 0) {
    return GOTO;
  } else if (strcmp(inputOperation,"V_INDEX") == 0) {
    return V_IN;
  } else if (strcmp(inputOperation,"==") == 0) {
    return EQL;
  } else if (strcmp(inputOperation,"!=") == 0) {
    return DIFE;
  } else if (strcmp(inputOperation,">") == 0) {
    return GRT;
  } else if (strcmp(inputOperation,"<") == 0) {
    return LST;
  } else if (strcmp(inputOperation,">=") == 0) {
    return GTE;
  } else if (strcmp(inputOperation,"<=") == 0) {
    return LTE;
  } else if (strcmp(inputOperation,"G_VAR") == 0) {
    return G_VAR;
  } else if (strcmp(inputOperation,"G_VET") == 0) {
    return G_VET;
  } else if (strcmp(inputOperation,"VET") == 0) {
    return VET;
  } else if (strcmp(inputOperation,"VAR") == 0) {
    return VAR;
  } else if (strcmp(inputOperation,"PARAM") == 0) {
    return PARAM;
  } else if (strcmp(inputOperation,"OUTPUT") == 0) {
    return OUTPUT;
  } else if (strcmp(inputOperation,"INPUT") == 0) {
    return INPUT;
  } else {
    return FNDECL;
  }

  return NONE;
}


void initializeAlignments(){
  AlignmentList = malloc(sizeof(Alignment)*1);
  if (AlignmentList == NULL) { callException("initializeAlignments",3,5); return; }
  AlignmentList->asmNumber = -1;
  AlignmentList->tripleNumber = -1;
  AlignmentList->kind = -1;
  AlignmentList->next = NULL;
  tempAlignment = malloc(sizeof(Alignment)*1);
  if (tempAlignment == NULL) { callException("initializeAlignments",3,5); return; }
  tempAlignment->asmNumber = -1;
  tempAlignment->tripleNumber = -1;
  tempAlignment->kind = -1;
  tempAlignment->next = NULL;
  _VERBOSE_5 printf("Alinhamentos inicializados\n");
}


void needsAlignment(int tripleNumber, int kind){
  if (AlignmentList == NULL) { callException("needsAlignment",4,5); return; }

  Alignment* newAlignment = malloc(sizeof(Alignment)*1);
  if (newAlignment == NULL) { callException("needsAlignment",3,5); return; }
  newAlignment->asmNumber = -1;
  newAlignment->tripleNumber = tripleNumber;
  newAlignment->kind = kind;
  newAlignment->next = NULL;

  if(AlignmentList->next == NULL)
    AlignmentList->next = newAlignment;
  else{
    int SAFE_LOOP = 0;
    tempAlignment = AlignmentList->next;

    while (tempAlignment->next != NULL) {
      tempAlignment = tempAlignment->next;
      SAFE_LOOP++;
      if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("needsAlignment",10,5); return; }
    }
    tempAlignment->next = newAlignment;
  }
  _VERBOSE_5 printf("Alinhamento criado para a tripla %d\n",tripleNumber);
}


void setAlignment(int tripleNumber, int asmNumber){
  if (tripleNumber > NUMBER_OF_TRIPLES) { callException("setAlignment",8,5); return; }
  if (AlignmentList == NULL) { callException("setAlignment",4,5); return; }
  if (AlignmentList->next == NULL) { callException("setAlignment",4,5); return; }

  int SAFE_LOOP = 0;
  tempAlignment = AlignmentList->next;

  while (tempAlignment != NULL) {
    if (tempAlignment->tripleNumber == tripleNumber) {
      tempAlignment->asmNumber = asmNumber;
      _VERBOSE_5 printf("Alinhamento da tripla %d ajustado para ASM %d\n",tripleNumber,asmNumber);
      return;
    }
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setAlignment",10,5); return; }
    tempAlignment = tempAlignment->next;
  }
}

int seekAlignment(int tripleNumber){
  int SAFE_LOOP = 0;
  tempAlignment = AlignmentList->next;

  while (tempAlignment != NULL) {
    if (tempAlignment->tripleNumber == tripleNumber) {
      _VERBOSE_5 printf("Alinhamento necessario encontrado para a tripla %d\n",tripleNumber);
      return 1;
    }
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("seekAlignment",10,5); return -999; }
    tempAlignment = tempAlignment->next;
  }
  return -999;
}

void Align(){
  if (AlignmentList == NULL) { callException("Align",4,5); return; }
  if (AlignmentList->next == NULL) { callException("Align",4,5); return; }

  int SAFE_LOOP = 0;
  tempAlignment = AlignmentList->next;
  tempAsm = asmList->next;

  while (tempAsm != NULL) {

    tempAlignment = AlignmentList->next;

    if (tempAsm->needsAlignment == FALSE){
      tempAsm = tempAsm->next;
      continue;
    }

    while (tempAlignment != NULL) {

        if (tempAsm->type == JTYPE) {
              if (tempAsm->jtype.address == tempAlignment->tripleNumber) {
                    adjustASM(tempAsm->asmNumber,2,tempAlignment->asmNumber);
                    _VERBOSE_5 printf("Endereço de salto do ASM %d foi de t(%d) para ASM %d\n",tempAsm->asmNumber,tempAlignment->tripleNumber,tempAlignment->asmNumber);
                    tempAlignment = tempAlignment->next;
                    break;
              } else {
                    tempAlignment = tempAlignment->next;
                    continue;
              }
        } else if (tempAsm->type == ITYPE) {
              if (tempAsm->itype.immediate == tempAlignment->tripleNumber) {
                    adjustASM(tempAsm->asmNumber,1,tempAlignment->asmNumber);
                    _VERBOSE_5 printf("Endereço de salto do ASM %d foi de t(%d) para ASM %d\n",tempAsm->asmNumber,tempAlignment->tripleNumber,tempAlignment->asmNumber);
                    tempAlignment = tempAlignment->next;
                    break;
              } else {
                    tempAlignment = tempAlignment->next;
                    continue;
              }
        } else if (tempAsm->type == LTYPE) {
              if (tempAsm->ltype.asmAddress == tempAlignment->tripleNumber) {
                    adjustASM(tempAsm->asmNumber,3,tempAlignment->asmNumber);
                    _VERBOSE_5 printf("Endereço de salto do ASM %d foi de t(%d) para ASM %d\n",tempAsm->asmNumber,tempAlignment->tripleNumber,tempAlignment->asmNumber);
                    tempAlignment = tempAlignment->next;
                    break;
              } else {
                    tempAlignment = tempAlignment->next;
                    continue;
              }
        }
        tempAlignment = tempAlignment->next;
    }

    tempAsm = tempAsm->next;
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("Align",10,5); return; }

  }
}


void initializeASMList(){
  NUMBER_OF_ASM = -2;
  asmList = createRTYPE(NONE,$none,$none,$none);
  tempAsm = createRTYPE(NONE,$none,$none,$none);
  addASM( createJTYPE( JUMP, returnFunctionTriple("main")) );
  toBeAligned(getCurrentASMNumber());
}

void cleanASM(){
  free(asmList);
  asmList = NULL;
  free(tempAsm);
  tempAsm = NULL;
}
