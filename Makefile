.PHONY: all
.PHONY: clean
.PHONY: main.c

all: main.c src/lexer.c

main.c:
	gcc -Wall -o main.o main.c  src/mem_management.c src/file_reader.c src/visitor.c src/parser.c src/ast.c src/lexer.h src/lexer.c src/token.c

run: src/mem_management.c src/file_reader.c src/visitor.c src/parser.c src/ast.c src/lexer.h src/lexer.c src/token.c main.c
	./main.o

clean:
	-rm *.o
