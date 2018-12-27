/*
 * Kod maszyny rejestrowej do projektu z JFTT2018
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2018-11-14
*/
#include<iostream>

#include<tuple>
#include<vector>
#include<map>

extern void run_parser( std::vector< std::tuple<int,int,int> > & program, FILE * data );
extern void run_machine( std::vector< std::tuple<int,int,int> > & program );

int main( int argc, char const * argv[] )
{
  std::vector< std::tuple<int,int,int> > program;
  FILE * data;

  if( argc!=2 )
  {
    std::cerr << "Sposób użycia programu: interpreter kod" << std::endl;
    return -1;
  }

  data = fopen( argv[1], "r" );
  if( !data )
  {
    std::cerr << "Błąd: Nie można otworzyć pliku " << argv[1] << std::endl;
    return -1;
  }

  run_parser( program, data );

  fclose( data );

  run_machine( program );

  return 0;
}
