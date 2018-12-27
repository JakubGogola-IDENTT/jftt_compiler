/*
 * Lexer interpretera maszyny rejestrowej do projektu z JFTT2018
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2018-11-14
*/
%option noyywrap
%option yylineno
%option nounput
%{
  #include <vector>
  #include <tuple>

  #include "parser.hh"
  #include "instructions.hh"

  int yylex();
%}
%%
\#.*$  ;
[ \t]+ ;
GET    { yylval = GET; return COM_1; };
PUT    { yylval = PUT; return COM_1; };
LOAD   { yylval = LOAD; return COM_1; };
STORE  { yylval = STORE; return COM_1; };
HALF   { yylval = HALF; return COM_1; };
INC    { yylval = INC; return COM_1; };
DEC    { yylval = DEC; return COM_1; };
COPY   { yylval = COPY; return COM_2; };
ADD    { yylval = ADD; return COM_2; };
SUB    { yylval = SUB; return COM_2; };
JUMP   { yylval = JUMP; return JUMP_1; };
JZERO  { yylval = JZERO; return JUMP_2; };
JODD   { yylval = JODD; return JUMP_2; };
HALT   { yylval = HALT; return STOP; };
A      { yylval = 0; return REG; };
B      { yylval = 1; return REG; };
C      { yylval = 2; return REG; };
D      { yylval = 3; return REG; };
E      { yylval = 4; return REG; };
F      { yylval = 5; return REG; };
G      { yylval = 6; return REG; };
H      { yylval = 7; return REG; };
[0-9]+ { yylval = atoi( yytext ); return LABEL; };
\n     ;
.      { return ERROR; }
%%
