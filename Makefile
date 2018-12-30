# TODO: !!!!!!!!!!!!!!!!
CC = g++
FLAGS = -std=c++17 --pedantic -Wall

all: kompilator

kompilator: parser.o lexer.o actions.o 
	$(CC) $(FLAGS) -o kompilator actions.o parser.o lexer.o

actions.o: actions.cpp
	$(CC) $(FLAGS) -c actions.cpp

lexer.o: lexer.c	
	$(CC) $(FLAGS) -c lexer.c

parser.o: parser.tab.c
	$(CC) $(FLAGS) -c parser.tab.c -o parser.o

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm -f *.o *~ *.c *.tab.c *.tab.h
	rm -f kompilator