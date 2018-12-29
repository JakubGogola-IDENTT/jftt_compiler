%{


%}
%union semantic_record {
    std::string pidentifier;
    unsigned long long num;

}

%start program
%token DECLARE IN END
%token IF WHILE
%token FOR
%token THEN ELSE ENDIF FROM TO DOWNTO 
%token READ WRITE
%token LESS GREATER LEQ GEQ EQ NEQ
%left ADD SUB
%left MUL DIV MOD

%%

program:        DECLARE
                        declarations
                IN
                        commands
                END
;

declarations:   declarations pidentifier
                | declarations pidentifier'('num ':' num')'
                | /* empty */
;

commands:       commands command
                | command       
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