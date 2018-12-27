/*
 * Kod interpretera maszyny rejestrowej do projektu z JFTT2018
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2018-11-14
 * (wersja CLN)
*/
#include<iostream>

#include<tuple>
#include<vector>
#include<map>

#include<cstdlib> 	// rand()
#include<ctime>

#include<cln/cln.h>

#include "instructions.hh"

using namespace cln;

void run_machine( std::vector< std::tuple<int,int,int> > & program )
{
  std::map<cl_I,cl_I> pam;

  cl_I r[8];
  int lr;

  cl_I t;

  std::cout << "Uruchamianie programu." << std::endl;
  lr = 0;
  srand( time(NULL) );
  for(int i = 0; i<8; i++ ) r[i] = rand();
  t = 0;
  while( std::get<0>(program[lr])!=HALT )	// HALT
  {
    switch( std::get<0>(program[lr]) )
    {
      case GET:		std::cout << "? "; std::cin >> r[std::get<1>(program[lr])]; t+=100; lr++; break;
      case PUT:		std::cout << "> " << r[std::get<1>(program[lr])] << std::endl; t+=100; lr++; break;
      case LOAD:	r[std::get<1>(program[lr])] = pam[r[0]]; t+=50; lr++; break;
      case STORE:	pam[r[0]] = r[std::get<1>(program[lr])]; t+=50; lr++; break;
      case COPY:	r[std::get<1>(program[lr])] = r[std::get<2>(program[lr])] ; t+=5; lr++; break;
      case ADD:		r[std::get<1>(program[lr])] += r[std::get<2>(program[lr])] ; t+=5; lr++; break;
      case SUB:		if( r[std::get<1>(program[lr])] >= r[std::get<2>(program[lr])] )
                          r[std::get<1>(program[lr])] -= r[std::get<2>(program[lr])];
                        else
                          r[std::get<1>(program[lr])] = 0;
                        t+=5; lr++; break;
      case HALF:	r[std::get<1>(program[lr])] >>= 1; t+=1; lr++; break;
      case INC:		r[std::get<1>(program[lr])]++ ; t+=1; lr++; break;
      case DEC:		if( r[std::get<1>(program[lr])]>0 ) r[std::get<1>(program[lr])]--; t+=1; lr++; break;
      case JUMP: 	lr = std::get<2>(program[lr]); t+=1; break;
      case JZERO:	if( r[std::get<1>(program[lr])]==0 ) lr = std::get<2>(program[lr]); else lr++; t+=1; break;
      case JODD:	if( oddp(r[std::get<1>(program[lr])]) ) lr = std::get<2>(program[lr]); else lr++; t+=1; break;
      default: break;
    }
    if( lr<0 || lr>=(int)program.size() )
    {
      std::cerr << "Błąd: Wywołanie nieistniejącej instrukcji nr " << lr << "." << std::endl;
      exit(-1);
    }
  }
  std::cout << "Skończono program (koszt: " << t << ")." << std::endl;
}
