%{
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "code_generator.hpp"
#include "defs.hpp"
#include "data.hpp"

int yylex();
int yyparse();
void yyerror(std::string s);

std::string input_file;
std::string output_file;

std::shared_ptr<data> d = std::make_shared<data>();
std::shared_ptr<code_generator> cg = std::make_shared<code_generator>(d);

%}
%union sem_rec {
    std::string *pidentifier;
    long long num;
    struct label *cond;
    struct symbol *sym;
}

//Types
%type <sym> identifier

//Tokens
%start program
%token DECLARE IN END
%token <cond> IF WHILE
%token FOR //TODO: struct for FOR 
%token THEN ELSE ENDIF FROM TO DOWNTO DO ENDFOR ENDWHILE ENDDO
%token READ WRITE       
%token LESS GREATER LEQ GEQ EQ NEQ
%token ASSIGN
%token ERROR
%token <pidentifier> pidentifier
%token <num> num

//Operators precedence
%left ADD SUB
%left MUL DIV MOD
%%      

program:        DECLARE
                        declarations
                IN
                        commands
                END                                                                     { cg->print_code(output_file); }
;

declarations:   declarations pidentifier';'                             
                | declarations pidentifier'('num':'num')'';'          
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

void yyerror(std::string s) {

}