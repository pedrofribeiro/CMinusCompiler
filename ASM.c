#include "ASM.h"

ASM_INSTR* createRTYPE(CPU_OPERATIONS cop, Register rd, Register r1, Register r2){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createRTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = RTYPE;
  newInstruction->rtype.cpu_operation = cop;
  newInstruction->rtype.rd = rd;
  newInstruction->rtype.r1 = r1;
  newInstruction->rtype.r2 = r2;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

ASM_INSTR* createITYPE(CPU_OPERATIONS cop, Register rd, Register r1, int imm){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createITYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = ITYPE;
  newInstruction->itype.cpu_operation = cop;
  newInstruction->itype.rd = rd;
  newInstruction->itype.r1 = r1;
  newInstruction->itype.immediate = imm;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

ASM_INSTR* createJTYPE(CPU_OPERATIONS cop, int addr){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createJTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = JTYPE;
  newInstruction->jtype.cpu_operation = cop;
  newInstruction->jtype.im_address = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

ASM_INSTR* createLTYPE(char cop[], int addr){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createLTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = LTYPE;
  strcpy(newInstruction->ltype.functionName,cop);
  newInstruction->ltype.asmAddress = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

void addASM(ASM_INSTR* newInstruction){

  if (newInstruction == NULL) {
    callException("addASM",14,5);
    return;
  }

  if (asmList->next == NULL) {

      asmList->next = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
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

void printASM(ASM_INSTR* instr){

  if(instr == NULL){
    callException("printASM",4,5);
    return;
  }

  int SAFE_LOOP = 0;
  tempAsm = asmList->next;

  while (tempAsm->next != NULL) {

      /*safe loop measure*/
      SAFE_LOOP++;
      if(SAFE_LOOP > SAFE_LOOP_SIZE){
        callException("printASM",10,5);
        return;
      }

      //NEED TO MAKE A DECENT TRANSLATION FUNCTION TO USE WITH ENUMS

      switch (instr->type) {
        case RTYPE:
            printf("%d ",instr->rtype.cpu_operation);
            printf("%d, ",instr->rtype.rd);
            printf("%d, ",instr->rtype.r1);
            printf("%d \n",instr->rtype.r2);
        break;
        case ITYPE:
            printf("%d ",instr->itype.cpu_operation);
            printf("%d, ",instr->itype.rd);
            printf("%d, ",instr->itype.r1);
            printf("%d \n",instr->itype.immediate);
        break;
        case JTYPE:
            printf("%d ",instr->jtype.cpu_operation);
            printf("%d \n",instr->jtype.im_address);
        break;
        case LTYPE:
            printf("%s ",instr->ltype.functionName);
            printf("%d \n",instr->ltype.asmAddress);
        break;
        default:
            callException("printASM",15,5);
        break;
      }
      tempAsm = tempAsm->next;
	}
}


void initializeASMList(){
  NUMBER_OF_ASM = 0;
  asmList = createRTYPE(NONE,$NONE,$NONE,$NONE);
  asmList = createRTYPE(NONE,$NONE,$NONE,$NONE);
}
