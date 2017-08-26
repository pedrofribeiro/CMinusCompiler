
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* ===================================== */

extern char* ESCOPO;

/*=======================================*/

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int loc, int lineno, ExpType dataType, StmtKind idType, char* scope)
{
  int h = hash(name);
  BucketList l =  hashTable[h];

  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;

  if ( (l == NULL) || (strcmp(ESCOPO,l->scope) != 0) ) /* variable not yet in table */
  {
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->lines->next = NULL;
    l->memloc = loc;
    l->dataType = dataType;
    l->idType = idType;
    l->scope = scope;
    l->next = hashTable[h];
    hashTable[h] = l;
  }
  else /* found in table, so just add line number */
  {
    LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */

int st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) && (strcmp(ESCOPO,l->scope) == 0) ) {
       return 1;
    }
    l = l->next;
  }
  return -1;
}

int st_lookupGlobal ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) && (strcmp("global",l->scope) == 0) ) {
       return 1;
    }
    l = l->next;
  }
  return -1;
}

int st_lookup2 ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) ) {
       return 1;
    }
    l = l->next;
  }
  return -1;
}

int st_lookup3 ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) && (l->idType == 4) ) {
       return 1;
    }
    l = l->next;
  }
  return -1;
}

int st_lookup4 ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) && (l->dataType == 0) ) {
       return 1;
    }
    l = l->next;
  }
  return -1;
}

int st_lookupFnStart( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( strcmp(name,l->name) == 0 ) {
       return l->memloc;
    }
    l = l->next;
  }
  return -1;
}

int st_lookupVarPosition( char * name, char* scope )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while (l != NULL) {
    if( ( strcmp(name,l->name) == 0) && (strcmp(scope,l->scope) == 0) ) {
       return l->memloc;
    }
    l = l->next;
  }
  return -1;
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing, "POSICAO   IDENTIFICADOR   TIPO DADO   TIPO ID   ESCOPO   LINHAS DE OCORRENCIA\n");
  fprintf(listing, "========  =============   =========   =======   ======   ====================\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing, "  %d   ",l->memloc);
        fprintf(listing, "        %-8s   ",l->name);
	      fprintf(listing, "   %-9d   ",l->dataType);
        fprintf(listing, " %-7d ",l->idType);
	      fprintf(listing, "%-8s ",l->scope);
        while (t != NULL)
        { fprintf(listing, " %-14d",t->lineno);
          t = t->next;
        }
        fprintf(listing, "\n");
        l = l->next;
      }
    }
  }
} /* printSymTab */
