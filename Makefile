# TODO: !!!!!!!!!!!!!!!!
CC = g++
FLAGS = -std=c++17 --pedantic -Wall

all: kompilator

kompilator: actions.o parser.o lexer.o
	$(CC) $(FLAGS) -o kompilator actions.o parser.o lexer.o

actions.o: actions.cpp
	$(CC) $(FLAGS) -c actions.cpp

lexer.o: lexer.c	
	$(CC) $(FLAGS) -c lexer.c

parser.o: parser.c
	$(CC) $(FLAGS) -c parser.c

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser.c: parser.y
	bison -o parser.c parser.y

clean:
	rm -f *.o *~
	rm -f kompilator