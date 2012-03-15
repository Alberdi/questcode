%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mydatatypes.h"
#include "myinout.h"
#include "mygcollector.h"
#include "mysymboltable.h"
#include "mysentences.h"
#include "mycommands.h"
#include "myexecution.h"

// Auxiliary variable
// It also keeps track of the topmost block during execution
// (useful to free memory in case of unexpected exit)
senblock* c;

context* ctxt = NULL; // Default context
sentence* s; // auxiliary

// allows modification of the command line parameters pointer
char** argv;
int* argc;

int main(int argcy, char* argvy[]) {
  extern char** argv;
  extern int* argc;
  
  argv = argvy;
  argv++;
  argc = &argcy;
  (*argc)--;
  
  init_context();
  yyparse();
  abort(); // clean exit
}
%}

%error-verbose

%start program

%union {
  int number;
  char charac;
  char* str;
  sentence* snt;
  senblock* sb;
  void* ptr;
  operation op;
}

%token <number> NUM
%token <str> QUIT ENTER OUTPUT INPUT DEFVAL WHILE SKIPPABLE ENDLINE IDENTIFIER WS INTEGER IF ELSE ISPOSITIVE ISBIGGER ENDBLOCK STARTBLOCK ADD SUBS TIMES DIV CLONE

%type <number> immvar value
%type <op> op
%type <str> truevar
%type <sb> sentenceblock block
%type <snt> sentence
%type <ptr> io declaration operation whileblock ifblock condition

%%

program      :    sentenceblock       { c = $1; execute($1); }
             ;

sentenceblock:    sentence               { c = (senblock*) sb_new(); sb_push(c, $1); $$ = c; }
             |    sentenceblock sentence { sb_push($1, $2); $$ = $1; }
             ;

sentence     :    WS sentence                { $$ = $2; }
             |    io endline                 { s = newsen(S_IO, $1);     $$ = s; }
             |    declaration endline        { s = newsen(S_DECL, $1);   $$ = s; }
             |    operation endline          { s = newsen(S_OP, $1);     $$ = s; }
             |    ifblock                    { s = newsen(S_IF, $1);     $$ = s; }
             |    whileblock                 { s = newsen(S_WHILE, $1);  $$ = s; }
             |    QUIT endline               { s = newsen(S_QUIT, NULL); $$ = s; }    
             ;
         
endline      :    ENDLINE
             ;
         
io           :    OUTPUT WS truevar              { $$ = iocomm($3, IOOP_OUT); }
             |    INPUT WS truevar               { $$ = iocomm($3, IOOP_IN); }
             ;
         
declaration  :    skippable WS value WS truevar WS ENTER  { $$ = declcomm($5, $3); }
             |    value WS truevar WS ENTER               { $$ = declcomm($3, $1); }
             |    truevar WS ENTER                        { $$ = declcomm($1, 0);  }
             ;
             
operation    :    truevar WS op WS truevar   { $$ = opcomm($5, $1,   0,  $3, OP_VAR); }
             |    immvar WS op WS truevar    { $$ = opcomm($5, NULL, $1, $3, OP_IMM); }
             ;
             
op           :    ADD   { $$ = OP_ADD; }
             |    SUBS  { $$ = OP_SUBS; }
             |    TIMES { $$ = OP_TIMES; }
             |    DIV   { $$ = OP_DIV; }
             |    CLONE { $$ = OP_EQ; }
             ;
             
whileblock   :    WHILE WS condition endline block     { $$ = whilecomm($3, $5); }
             ;
             
ifblock      :    IF WS condition endline block ELSE endline block   { $$ = ifcomm($3, $5, $8); }
             |    IF WS condition endline block                      { $$ = ifcomm($3, $5, NULL); }
             ;
             
condition    :    truevar WS ISPOSITIVE               { $$ = condcomm($1, NULL, 0,  CD_IMM); }
             |    truevar WS ISBIGGER WS truevar      { $$ = condcomm($1, $5,   0,  CD_VAR); }
             |    truevar WS ISBIGGER WS immvar       { $$ = condcomm($1, NULL, $5, CD_IMM); }
             ;
             
block        :    startblock sentenceblock endblock     { $$ = $2; }
             ;

startblock   :    WS startblock    
             |    STARTBLOCK endline
             ;

endblock     :    WS endblock
             |    ENDBLOCK endline
             ;

truevar      :    skippable WS IDENTIFIER            { $$ = $3; }
             |    IDENTIFIER                         { $$ = $1; }
             ;
             
immvar       :    skippable WS NUM WS INTEGER  { $$ = $3; }
             |    NUM WS INTEGER               { $$ = $1; }
             ;
             
skippable    :    SKIPPABLE
             |    skippable WS SKIPPABLE
             ;

value        :    DEFVAL WS NUM  { $$ = $3; }
             ;
             
%%
