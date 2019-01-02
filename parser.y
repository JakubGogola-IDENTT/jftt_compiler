%{
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "code_generator.hpp"
#include "defs.hpp"
#include "data.hpp"
#include "io_handler.hpp"

int yylex();
int yyparse();
void yyerror(std::string s);
extern FILE *yyin;

std::shared_ptr<data> d = std::make_shared<data>();
std::shared_ptr<code_generator> cg = std::make_shared<code_generator>(d);

std::vector<std::string> code; 
std::string current_id;
std::string old_id;
std::vector<std::string> vars;

int addr;

%}
%union sem_rec {
    std::string *pidentifier;
    long long num;
    struct label *cond;
    struct variable *var;
    struct variable *id;
}

//Tokens
%start program
%token DECLARE IN END
%token <cond> IF WHILE
%token FOR //TODO: struct for FOR 
%token THEN ELSE ENDIF FROM TO DOWNTO DO ENDFOR ENDWHILE ENDDO
%token READ WRITE       
%token LT GT LEQ GEQ EQ NEQ
%token ASSIGN
%token ERROR
%token <pidentifier> pidentifier
%token <num> num

//Types
%type <var> value
%type <id> identifier

//Operators precedence
%left ADD SUB
%left MUL DIV MOD
%%      

program:        DECLARE                                                                 
                        declarations
                IN                                                                    
                        commands
                END                                                                     { 
                                                                                                cg->end_prog();
                                                                                                code = cg->get_code();
                                                                                        }
;

declarations:   declarations pidentifier';'                                             { d->put_symbol(*$2); }
                | declarations pidentifier'('num':'num')'';'                            { 
                                                                                                addr = d->put_symbol_array(*$2, $4, $6);  
                                                                                                if(addr != -1) {
                                                                                                        cg->array_offset(addr, $4);
                                                                                                }
                                                                                        }
                |                                                        
;

commands:       commands command
                | command       
;

command:        identifier ASSIGN expression';'                                         { cg->assign($1); d->init_variable(current_id); }             

                /* ################################################################################# */
                /*### IF ###*/
                | IF                                                                    
                        condition 
                  THEN 
                        commands 
                  ELSE 
                        commands 
                  ENDIF          

                /*### IF-ELSE ###*/
                | IF                                                                                                                                        
                        condition                                                                                                                                   
                  THEN 
                        commands 
                  ENDIF                                                                 { std::cout << "ENDIF" << std::endl; }                                                   

                /*### WHILE ###*/
                | WHILE 
                        condition               
                  DO 
                        commands 
                  ENDWHILE

                /*### DO-WHILE ###*/
                | DO 
                        commands 
                  WHILE 
                        condition 
                  ENDWHILE

                /*### FOR-FROM-TO ###*/
                | FOR 
                        pidentifier 
                  FROM 
                        value 
                  TO 
                        value 
                  DO 
                        commands 
                  ENDFOR

                /*### FOR-FROM-DOWNTO ###*/
                | FOR 
                        pidentifier 
                  FROM 
                        value 
                  DOWNTO 
                        value 
                  DO 
                        commands 
                  ENDFOR
                /* ################################################################################# */

                | READ identifier';'                                                    { cg->read($2); }
                | WRITE value';'                                                        { cg->write($2); }
;

expression:     value                                                                   { cg->constant($1); }
                | value ADD value                                                       { cg->add($1, $3); }
                | value SUB value                                                       { cg->sub($1, $3); }
                | value MUL value                                                       { cg->mul($1, $3); }
                | value DIV value                                                       { cg->div($1, $3); }
                | value MOD value                                                       { cg->rem($1, $3); }
;

condition:      value EQ value                                                          {  }
                | value NEQ value                                                       {  }
                | value LT value                                                        {  }
                | value GT value                                                        {  }
                | value LEQ value                                                       {  }
                | value GEQ value                                                       {  }
;

value:          num                                                                     { $$ = d->get_value_num($1); }
                | identifier                                                            { $$ = d->get_value($1); }
;

identifier:     pidentifier                                                             { $$ = d->get_variable(*$1); current_id = *$1; }
                | pidentifier '(' pidentifier ')'                                       { $$ = d->get_variable_array_var(*$1, *$3); }
                | pidentifier'('num')'                                                  { $$ = d->get_variable_array_num(*$1, $3); }
;
%%

int main(int argc, char** argv) {
        std::shared_ptr<io_handler> io = std::make_shared<io_handler>(argc, argv);
        yyin = io->read_code();

        if(yyin == nullptr) {
                return -1;
        }

        std::cout << "### \x1b[34mCompilator is running...\x1b[0m ###" << std::endl;

        yyparse();


        if (!d->get_errors()) {
                std::cout << "### \x1b[32mCompilation completed succesfully\x1b[0m ###" << std::endl;
                io->print_code(code);
        } else {
                std::cerr << "### \x1b[31mErrors occured\x1b[0m ###" << std::endl;
        }
        
        return 0;
}

void yyerror(std::string s) {

}