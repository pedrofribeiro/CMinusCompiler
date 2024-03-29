
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* memory location for variable */
     ExpType dataType;
     StmtKind idType;
     char* scope;
     struct BucketListRec * next;
     int numberOfParameters;
   } * BucketList;


void st_insert( char * name, int loc, int lineno, ExpType dataType, StmtKind idType, char* scope);
/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name );
int st_lookup2 ( char * name );
int st_lookup3 ( char * name );
int st_lookup4 ( char * name );
int st_lookupFnStart( char * name );
int st_lookupVarPosition( char * name, char* scope );
int st_lookupGlobal( char * name );
/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
