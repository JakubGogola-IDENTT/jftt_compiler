CC = g++
FLAGS = -std=c++17 --pedantic -Wall

all: kompilator

kompilator: parser.o lexer.o code_generator.o data.o
	$(CC) $(FLAGS) -o kompilator code_generator.o parser.o lexer.o data.o

code_generator.o: code_generator.cpp
	$(CC) $(FLAGS) -c code_generator.cpp

data.o: data.cpp
	$(CC) $(FLAGS) -c data.cpp

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