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
void yyerror(char const *s);
extern FILE *yyin;
extern int yylineno;

const std::string error_alert = "\x1b[31merror\x1b[0m: ";

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
    struct for_label *lfor;
    struct cond_label *cond;
    struct label *lbl;
    struct variable *var;
    struct variable *id;
}

//Tokens
%start program
%token DECLARE IN END
%token <lbl> IF WHILE DO
%token <lfor> FOR 
%token THEN ELSE ENDIF FROM TO DOWNTO ENDFOR ENDWHILE ENDDO
%token READ WRITE       
%token LT GT LEQ GEQ EQ NEQ
%token ASSIGN
%token ERROR
%token <pidentifier> pidentifier
%token <num> num

//Types
%type <var> value
%type <id> identifier
%type <cond> condition;

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

declarations:   declarations pidentifier';'                                             { 
                                                                                                d->set_line(yylineno);
                                                                                                d->put_symbol(*$2); 
                                                                                        }
                | declarations pidentifier'('num':'num')'';'                            { 
                                                                                                d->set_line(yylineno);
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


command:        identifier                                                              { $1 = d->init_variable($1, current_id); }
                ASSIGN expression';'                                                    { cg->assign($1); }             

                /*### IF_ELSE ###*/
                | IF condition THEN commands                                            { 
                                                                                                $1 = d->get_label(0, 0);
                                                                                                cg->if_else_block_first($1, $2->go_to);
                                                                                        }                   
                  ELSE commands ENDIF                                                   { cg->if_else_block_second($1->go_to); }               

                /*### IF ###*/
                | IF condition THEN commands ENDIF                                      { cg->if_block($2->go_to); }             

                /*### WHILE ###*/
                | WHILE condition DO                                                                                                                                                             
                  commands ENDWHILE                                                     { cg->while_block($2); }    

                /*### DO_WHILE ###*/
                | DO                                                                    { 
                                                                                                $1 = d->get_label(0, 0); 
                                                                                                cg->do_while_block_first($1);
                                                                                        }
                  commands WHILE condition ENDDO                                        { cg->do_while_block_second($1, $5->go_to); }

                /*### FOR_FROM_TO ###*/
                | FOR pidentifier FROM value TO value DO                                {  
                                                                                                d->put_symbol_iterator(*$2);
                                                                                                $1 = d->get_for_label(*$2, $4, $6);
                                                                                                cg->for_to_block_first($1);
                                                                                        }
                        commands 
                  ENDFOR                                                                { 
                                                                                                cg->for_to_block_second($1); 
                                                                                                d->remove_iterator_symbol(*$2);
                                                                                        }

                /*### FOR_FROM_DOWNTO ###*/
                | FOR pidentifier FROM value DOWNTO value DO                            { 
                                                                                                d->put_symbol_iterator(*$2);
                                                                                                $1 = d->get_for_label(*$2, $4, $6);
                                                                                                cg->for_downto_block_first($1);
                                                                                        }
                        commands 
                  ENDFOR                                                                { 
                                                                                                cg->for_downto_block_second($1); 
                                                                                                d->remove_iterator_symbol(*$2);
                                                                                        }


                | READ identifier';'                                                    { 
                                                                                                $2 = d->init_variable($2, current_id); 
                                                                                                cg->read($2); 
                                                                                        }
                | WRITE value';'                                                        { cg->write($2); }
;

expression:     value                                                                   { cg->constant($1); }
                | value ADD value                                                       { cg->add($1, $3); }
                | value SUB value                                                       { cg->sub($1, $3); }
                | value MUL value                                                       { cg->mul($1, $3); }
                | value DIV value                                                       { cg->div($1, $3); }
                | value MOD value                                                       { cg->mod($1, $3); }
;

condition:      value EQ value                                                          { $$ = cg->eq($1, $3); }
                | value NEQ value                                                       { $$ = cg->neq($1, $3); }
                | value LT value                                                        { $$ = cg->lt($1, $3); }
                | value GT value                                                        { $$ = cg->gt($1, $3); }
                | value LEQ value                                                       { $$ = cg->leq($1, $3); }
                | value GEQ value                                                       { $$ = cg->geq($1, $3); }
;

value:          num                                                                     { $$ = d->get_value_num($1); }
                | identifier                                                            { $$ = d->get_value($1, current_id); }
;

identifier:     pidentifier                                                             { 
                                                                                                d->set_line(yylineno);
                                                                                                $$ = d->get_variable(*$1); 
                                                                                                current_id = *$1; 
                                                                                        }
                | pidentifier'('pidentifier')'                                          {  
                                                                                                d->set_line(yylineno);
                                                                                                $$ = d->get_variable_array_var(*$1, *$3); 
                                                                                                current_id = *$3; 
                                                                                        }
                | pidentifier'('num')'                                                  { 
                                                                                                d->set_line(yylineno);
                                                                                                $$ = d->get_variable_array_num(*$1, $3); 
                                                                                        }
;


%%

int main(int argc, char** argv) {
        std::shared_ptr<io_handler> io = std::make_shared<io_handler>(argc, argv);
        yyin = io->read_code();

        if(yyin == nullptr) {
                return -1;
        }

        std::cout << "### \x1b[34mCompiler is running...\x1b[0m ###" << std::endl;

        yyparse();


        if (!d->get_errors()) {
                std::cout << "### \x1b[32mCompilation completed successfully\x1b[0m ###" << std::endl;
                io->print_code(code);
        } else {
                std::cerr << "### \x1b[31mErrors occured\x1b[0m ###" << std::endl;
        }
        
        return 0;
}

void yyerror(char const *s) {
        std::cerr << error_alert << s << " - unrecognized token (line " << yylineno << ")" << std::endl;
        std::cerr << "### \x1b[31mErrors occured\x1b[0m ###" << std::endl;
        exit(0);
}