#include "assemblyCode.h"

void asmCode (triple* instruction) {

  if (instruction == NULL) { callException("asmCode",4,5); return; }

  Operation operation = NONE;
  operation = getOperation(instruction);
  if (operation == NONE) { callException("asmCode",13,5); return; }

  int i;

  switch (operation) {
    case FNDECL:
      addASM( createLTYPE( instruction->operation, currentASMNumber()+1 ) );
      addASM( createITYPE( SW, $ra, $sp, 0 ) );
      addASM( createRTYPE( ADD, $sp, $sp, $one ) );
      setNamePosition(st_lookupFnStart(instruction->operation),1);
      requestMemory(st_lookupFnStart(instruction->operation),1);
    break;
    case EQL:
    case DIFE:
    case GRT:
    case LST:
    case GTE:
    case LTE:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
        if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            int packegedConstants;
            switch (operation) {
              case ADD:
                  packegedConstants = instruction->firstOperand + instruction->secondOperand;
              break;
              case SUB:
                packegedConstants = instruction->firstOperand - instruction->secondOperand;
              break;
              case MUL:
                packegedConstants = instruction->firstOperand * instruction->secondOperand;
              break;
              case DIV:
                packegedConstants = instruction->firstOperand / instruction->secondOperand;
              break;
              case EQL:
                packegedConstants = instruction->firstOperand == instruction->secondOperand;
              break;
              case DIFE:
                packegedConstants = instruction->firstOperand != instruction->secondOperand;
              break;
              case GRT:
                packegedConstants = instruction->firstOperand > instruction->secondOperand;
              break;
              case LST:
                packegedConstants = instruction->firstOperand < instruction->secondOperand;
              break;
              case GTE:
                packegedConstants = instruction->firstOperand >= instruction->secondOperand;
              break;
              case LTE:
                packegedConstants = instruction->firstOperand <= instruction->secondOperand;
              break;
              default:
                callException("asmCode: const packaging",1,5);
              break;
            }

            addASM( createITYPE( LI, $acc, $zero, packegedConstants) );
            setRegister(2,packegedConstants);

        } else if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $acc, $zero, instruction->firstOperand ) );
            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister(2,instruction->firstOperand);
            setRegister(9,getNamePosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == TripleAddress)) {

            addASM( createITYPE( LI, $t1, $zero, instruction->firstOperand ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,instruction->firstOperand);

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, instruction->secondOperand ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getNamePosition(instruction->firstOperand));
            setRegister(2,instruction->secondOperand);

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $acc, $acc, 0 ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getNamePosition(instruction->firstOperand));
            setRegister(2,getNamePosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == TripleAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getNamePosition(instruction->firstOperand));
            setRegister(2,instruction->secondOperand);

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister($t1,instruction->secondOperand);

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            if ((operation == EQL) || (operation == DIFE) || (operation == GRT) || (operation == LST) || (operation == GTE) || (operation == LTE))
              logicalBranch(operation, instruction->next->secondOperand);
            else
              addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister($t1,getNamePosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == TripleAddress)) {
            addASM( createRTYPE( operation, $acc, $t1, $acc ) );
        } else {
            callException("asmCode: arithmetic ops",1,5);
        }
    break;
    case RET:
        addASM( createRTYPE( MOVE, $t1, $acc, $zero ) );
        addASM( createRTYPE( SUB, $t1, $sp, $one ) );
        addASM( createITYPE( LW, $ra, $t1, 0 ) );
        addASM( createJTYPE( JR, $ra ) );
    break;
    case PARAM:
        if (instruction->firstOperandType == ConstantNoAddress) {
            /* sw $acc 0($sp)
               addiu $sp $sp 1 */
            addASM ( createITYPE ( LI, $t1, $zero, instruction->firstOperandType ) );
            addASM ( createITYPE ( SW, $sp, $t1, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
            requestMemory(currentASMNumber(), 1);

        } else if (instruction->firstOperandType == SymboltableAddress) {
            /* lw $acc var_position_on_memory
               sw $acc 0($sp)
               addiu $sp $sp 1
            */
            addASM ( createITYPE ( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM ( createITYPE ( LW, $t1, $t1, 0 ) );
            addASM ( createITYPE ( SW, $sp, $t1, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
            requestMemory(instruction->firstOperand, 1);

        } else if (instruction->firstOperandType == TripleAddress) {
          /* sw $acc 0($sp)
             addiu $sp $sp 1 */
            addASM ( createITYPE ( SW, $sp, $acc, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
            requestMemory(instruction->tripleNumber, 1);

        } else {
            callException("asmCode: PARAM",1,5);
        }
    break;
    case CALL:
        addASM ( createJTYPE ( JAL, getNamePosition(instruction->firstOperand)) );
    break;
    case ATR:

        if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == ConstantNoAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( LI, $acc, $zero, instruction->secondOperand ) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == SymboltableAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( LI, $acc, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $acc, $acc, 0) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == TripleAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == ConstantNoAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            addASM( createITYPE( SW, $acc, $t1, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == SymboltableAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0) );
            addASM( createITYPE( SW, $acc, $t1, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == TripleAddress) ) {
            /*this only happens if a vector is receiving its value from an operation.
              vectors have a SPECIAL dedicated $reg for its parameter return, its $rv
            */
            addASM( createITYPE( SW, $rv, $acc, $zero ) );

        } else {
            callException("asmCode: ATR",26,5);
            return;
        }

    break;
    case IF_F:
        //addASM( createITYPE( BEQ, $acc, $zero, 222 ) );
    break;
    case GOTO:
        addASM( createJTYPE( JUMP, instruction->firstOperand ) );
        toBeAligned(currentASMNumber());
    break;
    case V_IN:
        if (instruction->secondOperandType == ConstantNoAddress) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            addASM( createITYPE( LI, $acc, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $acc, $t1 ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else if (instruction->secondOperandType == SymboltableAddress) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $acc, $t1 ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else if (instruction->secondOperandType == TripleAddress) {

            addASM( createITYPE( LI, $t1, $zero, getNamePosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $t1, $acc ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else {
            callException("asmCode: V_IN",1,5);
        }
    break;
    case G_VAR:
        /*
        sw $zero 0($sp)
        addiu $sp $sp 1
        */
        addASM( createITYPE ( SW, $sp, $zero, 0 ) );
        addASM( createRTYPE ( ADD, $sp, $sp, $one ) );
        int gvarCreation;
        gvarCreation = setNamePosition(instruction->firstOperand,1);
        if (gvarCreation == -999) { callException("asmCode: G_VAR",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }
        setGP(currentASMNumber());
        addASM( createRTYPE( LI, $gp, $zero, getGP() ) );

    break;
    case G_VET:
      /*
      loop n, n = number of parameters
        sw $zero 0($sp)
        addiu $sp $sp 1*/
        for (i = 0; i < instruction->secondOperand; i++) {
            addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        }
      int gvetCreation;
      gvetCreation = setNamePosition(instruction->firstOperand,instruction->secondOperand);
      if (gvetCreation == -999) { callException("asmCode: G_VET",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }
      setGP(currentASMNumber());
      addASM( createRTYPE( LI, $gp, $zero, getGP() ) );

    break;
    case VET:
        /*
        loop n, n = number of parameters
          sw $zero 0($sp)
          addiu $sp $sp 1
        */
        for (i = 0; i < instruction->secondOperand; i++) {
            addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        }
        int vetCreation;
        vetCreation = setNamePosition(instruction->firstOperand,instruction->secondOperand);
        if (vetCreation == -999) { callException("asmCode: VET",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }

    break;
    case VAR:
        /*
        sw $zero 0($sp)
        addiu $sp $sp 1
        */
        addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
        addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        int varCreation;
        varCreation = setNamePosition(instruction->firstOperand,1);
        if (varCreation == -999) { callException("asmCode: VAR",20,5); printf("TN:%d, st id(%d)\n",instruction->tripleNumber,instruction->firstOperand); return; }

    break;
    default:
        callException("asmCode",1,5);
    break;
  }

return;
}


void generateAssembly(triple* List){

  if(List == NULL){
    callException("generateAssembly",4,5);
    return;
  }

  TRACE_ASM_GEN = FALSE;

  RuntimeEnvironmentStart();
  initializeASMList();

  while (List != NULL) {
    asmCode(List);
    /*marcar no gerador de código intermediário quais as triplas que vão precisar de ajustes
    e ficar de sentinela nesse ponto, dentro do loop. Comparando o número da tripla corrente
    com os números das triplas que precisarão de ajustes. Se der match, basta setar o Alignment
    com o número do currentASMNumber() e pronto!
    */

    if (seekAlignment(List->tripleNumber) == 1) {
      setAlignment(List->tripleNumber,currentASMNumber());
    }

    List = List->next;
  }

  Align();
  printASM(0);
  printVars();
  cleanTriples();
  cleanASM();
  RuntimeEnvironmentStop();

}
