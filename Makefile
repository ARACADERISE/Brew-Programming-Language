.PHONY: all
.PHONY: main.c

all: main.c lexer/lexer.h

main.c:
	gcc -Wall -o main.o main.c lexer/lexer.c lexer/token.c

run: lexer/lexer.c lexer/token.c main.c
	./main.o