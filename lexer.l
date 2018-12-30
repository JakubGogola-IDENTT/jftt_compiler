%option noyywrap
%option yylineno
     
%{
#include <iostream>
#include <string>
#include <memory>
#include "parser.tab.h"
%}
%%
"DECLARE"               { return DECLARE; }
"IN"                    { return IN; }
"END"                   { return END; }

"READ"                  { return READ; }
"WRITE"                 { return WRITE; }

"IF"                    { return IF; }
"THEN"                  { return THEN; }
"ELSE"                  { return ELSE; }
"ENDIf"                 { return ENDIF; }

"WHILE"                 { return WHILE; }
"DO"                    { return DO; }
"ENDWHILE"              { return ENDWHILE; }
"ENDDO"                 { return ENDDO; }

"FOR"                   { return FOR; }
"FROM"                  { return FROM; }
"TO"                    { return TO; }
"DOWNTO"                { return DOWNTO; }
"ENDFOR"                { return ENDFOR; }

":="                    { return ASSIGN; }

"="                     { return EQ; }
"!="                    { return NEQ; }
"<"                     { return LESS; }
">"                     { return GREATER; }
"<="                    { return LEQ; }
">="                    { return GEQ; }

"+"                     { return ADD; }
"-"                     { return SUB; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"*"                     { return MUL; }
";"                     { return TERM; } 
[_a-z]+                 { 
                            std::cout << yytext << std::endl;
                            yylval.pidentifier = std::make_shared<std::string>(yytext).get();
                            return pidentifier;
                        }
[0-9]+                  { yylval.num = std::stoull(yytext); return num; }
\[.*\]                  { /*ignore comments TODO: make comments better again*/ }
[ \t\n]+                { /*ignore whitespaces*/ }
.                       { return yytext[0]; }
%%