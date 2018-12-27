/*
 * Parser interpretera maszyny rejestrowej do projektu z JFTT2018
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2018-11-14
*/
%{
  #define YYSTYPE int

  #include<iostream>
  #include<tuple>
  #include<vector>

  #include "instructions.hh"

  extern int yylineno;
  int yylex( void );
  void yyset_in( FILE * in_str );
  void yyerror( std::vector< std::tuple<int,int,int> > & program, char const *s );
%}
%parse-param { std::vector< std::tuple<int,int,int> > & program }
%token COM_1
%token COM_2
%token JUMP_1
%token JUMP_2
%token STOP
%token REG
%token LABEL
%token ERROR
%%
input :
    input line
  | %empty
  ;

line :
    COM_1 REG	{ program.push_back( std::make_tuple($1,$2,0) ); }
  | COM_2 REG REG { program.push_back( std::make_tuple($1,$2,$3) ); }
  | JUMP_1 LABEL { program.push_back( std::make_tuple($1,0,$2) ); }
  | JUMP_2 REG LABEL { program.push_back( std::make_tuple($1,$2,$3) ); }
  | STOP { program.push_back( std::make_tuple($1,0,0) ); }
  | ERROR { yyerror( program, "Nierozpoznany symbol" ); }
  ;
%%
void yyerror( std::vector< std::tuple<int,int,int> > & program, char const *s )
{
  std::cerr << "Linia " << yylineno << ": " << s << std::endl;
  exit(-1);
}

void run_parser( std::vector< std::tuple<int,int,int> > & program, FILE * data ) 
{
  std::cout << "Czytanie kodu." << std::endl;
  yyset_in( data );
  yyparse( program );
  std::cout << "Skończono czytanie kodu (liczba rozkazów: " << program.size() << ")." << std::endl;
}
