#include "ASM.h"

ASM_INSTR* createRTYPE(char cop[], char rd[], char ra[], char rb[]){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createRTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = RTYPE;
  strcpy(newInstruction->rtype.cpu_operation,cop);
  strcpy(newInstruction->rtype.reg_dest,rd);
  strcpy(newInstruction->rtype.reg_op_a,ra);
  strcpy(newInstruction->rtype.reg_op_b,rb);
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

ASM_INSTR* createITYPE(char cop[], char rd[], char ra[], int imm){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createITYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = ITYPE;
  strcpy(newInstruction->itype.cpu_operation,cop);
  strcpy(newInstruction->itype.reg_dest,rd);
  strcpy(newInstruction->itype.reg_op_a,ra);
  newInstruction->itype.immediate = imm;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
  return newInstruction;
}

ASM_INSTR* createJTYPE(char cop[], int addr){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createJTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = JTYPE;
  strcpy(newInstruction->jtype.cpu_operation,cop);
  newInstruction->jtype.im_address = addr;
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

	switch (instr->type) {
		case RTYPE:
				printf("%s ",instr->rtype.cpu_operation);
				printf("%s, ",instr->rtype.reg_dest);
        printf("%s, ",instr->rtype.reg_op_a);
        printf("%s \n",instr->rtype.reg_op_b);
		break;
		case ITYPE:
        printf("%s ",instr->itype.cpu_operation);
        printf("%s, ",instr->itype.reg_dest);
        printf("%s, ",instr->itype.reg_op_a);
        printf("%d \n",instr->itype.immediate);
		break;
		case JTYPE:
        printf("%s ",instr->jtype.cpu_operation);
        printf("%d ",instr->jtype.im_address);
		break;
    default:
        callException("printASM",15,5);
    break;
	}
}


void initializeASMList(){
  NUMBER_OF_ASM = 0;
  asmList = createRTYPE("asmList","none","none","none");
  tempAsm = createRTYPE("tempAsm","none","none","none");
}
