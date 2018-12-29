# TODO: !!!!!!!!!!!!!!!!
CC = g++
FLAGS = -std=c++17

all: main


actions.o: actions.cpp
	$(CC) $(FLAGS) -c actions.cpp

lexer.c: lexer.l
	flex -o lexer.c lexer.l

bison.

clean:
