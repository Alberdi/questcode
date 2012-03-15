%{
#include "mydatatypes.h"
#include "y.tab.h"
%}

%%
[S|s]urrender                { return QUIT; }
enters?" the battlefield"    { return ENTER; }
[S|s]"cry"(" on")?           { return OUTPUT; }
[C|c]ommand                  { return INPUT; }
[L|l]evel                    { return DEFVAL; }
[A|a]"s long as"             { return WHILE; }
[I|i]f                       { return IF; }
[E|e]lse                     { return ELSE; }
[F|f]"ollow tactic"          { return STARTBLOCK; }
[E|e]"nd tactic"             { return ENDBLOCK; }
"is alive"|"are alive"       { return ISPOSITIVE; }
beats?                       { return ISBIGGER; }
\.(.*#.*)?\n                 { yylineno++; return ENDLINE; }
(#.*)?\n                     { yylineno++; }
[\ \t]+                      { return WS; }
[A|a]n?|[T|t]he              { return SKIPPABLE; } 
[E|e]vil|[A|a]wesome|[B|b]ig { return SKIPPABLE; }
[F|f]ucking|[M|m]arvelous    { return SKIPPABLE; }
[Y|y]oung|[L|l]ittle|[M|m]ad { return SKIPPABLE; }
[J|j]oyful|[F|f]unny|[F|f]at { return SKIPPABLE; }
[B|b]ad|[B|b]oring|[O|o]ld   { return SKIPPABLE; }    
[H|h]appy|[S|s]ome|[G|g]ood  { return SKIPPABLE; }
[H|h]opeful|[N|n]eedless     { return SKIPPABLE; }
confronts?                   { return CLONE; }
heals?                       { return ADD; }
damages?                     { return SUBS; }
cheers?                      { return TIMES; }
boos?                        { return DIV; }
goblins?                     { return INTEGER; }
[0-9]+   		                 { yylval.number = atoi(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9_]*        { yylval.str = strdup(yytext); return IDENTIFIER; }
%%
