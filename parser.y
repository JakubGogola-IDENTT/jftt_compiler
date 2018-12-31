%{
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "CodeGenerator.hpp"
#include "defs.hpp"
#include "Data.hpp"

int yylex();
void yyerror(const char *s);
int yyparse();

std::shared_ptr<Data> Data();

%}
%union{
    std::string *pidentifier;
    long long num;
    std::vector<std::string> *code;
    struct label *cond;
    struct symbol *sym;
}

/*%type <code> program
%type <code> commands
%type <code> command
%type <code> expression*/
%start program
%token DECLARE IN END
%token <cond> IF WHILE
%token FOR
%token THEN ELSE ENDIF FROM TO DOWNTO DO ENDFOR ENDWHILE ENDDO
%token READ WRITE       
%token LESS GREATER LEQ GEQ EQ NEQ
%token ASSIGN
%token ERROR

%token <pidentifier> pidentifier
%token <num> num

//%type <num> condition
//%type <sym> value
//%type <sym> identifier

%left ADD SUB
%left MUL DIV MOD
%%      

program:        DECLARE
                        declarations
                IN
                        commands
                END 
;

declarations:   declarations pidentifier';'                             
                | declarations pidentifier'('num ':' num')'';'          
                |                                                        
;

commands:       commands command
                | command       
;

command:        identifier ASSIGN expression';'
                | IF condition THEN commands ELSE commands ENDIF
                | IF condition THEN commands ENDIF
                | WHILE condition DO commands ENDWHILE
                | DO commands WHILE condition ENDWHILE
                | FOR pidentifier FROM value TO value DO commands ENDFOR
                | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
                | READ identifier';'
                | WRITE value';'
;

expression:     value
                | value ADD value
                | value SUB value
                | value MUL value
                | value DIV value
                | value MOD value
;

condition:      value EQ value
                | value NEQ value
                | value LESS value
                | value GREATER value
                | value LEQ value
                | value GEQ value
;

value:          num
                | identifier
;

identifier:     pidentifier
                | pidentifier'('pidentifier')'
                | pidentifier'('num')'
;
%%

int main() {
        return yyparse();
}

void yyerror(const char *s) {

}