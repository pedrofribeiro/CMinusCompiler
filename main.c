#include "globals.h"
/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE
/* set NO_INTERMCODE to TRUE to get a compiler that does not generate intermediary code  */
#define NO_INTERMCODE FALSE
/* set NO_ASM to TRUE to get a compiler that does not generate assembly code  */
#define NO_ASM FALSE
/* set NO_MACHINECODE to TRUE to get a compiler that does not generate machine code  */
#define NO_MACHINECODE TRUE

#include "util.h"
#if NO_PARSE
    #include "scan.h"
#else
    #include "parse.h"
    #if !NO_ANALYZE
      #include "analyze.h"
      #if !NO_INTERMCODE
        #include "intermediaryCode.h"
      #endif
      #if !NO_ASM
        #include "assemblyCode.h"
      #endif
      #if !NO_MACHINECODE
        #include "machineCode.h"
      #endif
    #endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceIntermCode = TRUE;
int TraceASM = TRUE;
int TraceMachineCode = FALSE;

int Error = FALSE;

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  char pgm[120]; /* source code file name */
  if (argc != 2)
    { fprintf(stderr,"USO CORRETO: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".txt");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"O ARQUIVO: %s NAO FOI ENCONTRADO.\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\n\n\nPROCESSO DE COMPILACAO INICIADO. %s\n\n",pgm);
  fprintf(listing,"\n\n===================================\n\n");
  fprintf(listing,"ANALISE LEXICA.");
  fprintf(listing,"\n===================================\n\n");

#if NO_PARSE
  while (getToken()!=ENDFILE);
#else
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing,"\n\n===================================\n");
    fprintf(listing,"ANALISE SINTATICA");
    fprintf(listing,"\n===================================\n\n\n\n");
    printTree(syntaxTree);
	  }

#if !NO_ANALYZE
  if (! Error)
  { if (TraceAnalyze) {
      fprintf(listing,"\n\n===================================\n");
      fprintf(listing,"CONSTRUCAO DA SYMBOL TABLE.");
      fprintf(listing,"\n===================================\n\n\n\n");
    }
    buildSymtab(syntaxTree);
    if(TraceAnalyze){
      fprintf(listing,"\n\n===================================\n");
      fprintf(listing,"ANALISE SEMANTICA.");
      fprintf(listing,"\n===================================\n\n\n\n");
    }
    if(mainVerify() == -1){
      fprintf(listing, "\nERRO ENCONTRADO. FUNCAO MAIN NAO DECLARADA. \n\n");
    }
    fprintf(listing, "Checagem de tipos.\n");
    typeCheck(syntaxTree);
    fprintf(listing, "Checagem de tipos finalizada.\n");

  }

#if !NO_INTERMCODE
if(TraceIntermCode){
    fprintf(listing,"\n\n===================================\n");
    fprintf(listing,"CÓDIGO INTERMEDIÁRIO.");
    fprintf(listing,"\n===================================\n\n\n\n");
    generateIntermediaryCode(syntaxTree);
}
#endif

#if !NO_ASM
if(TraceASM){
    fprintf(listing,"\n\n===================================\n");
    fprintf(listing,"ASSEMBLY.");
    fprintf(listing,"\n===================================\n\n\n\n");
    generateAssembly(tripleList->next);
}
#endif

#if !NO_MACHINECODE
  if (! Error)
  { char * codefile;
    int fnlen = strcspn(pgm,".");
    codefile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".asm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("NAO FOI POSSIVEL ABRIR O ARQUIVO: %s\n",codefile);
      exit(1);
    }
    fclose(code);
  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}
