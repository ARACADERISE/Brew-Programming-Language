.PHONY: all
.PHONY: clean
.PHONY: main.c

all: main.c src/lexer.c

main.c:
	gcc -Wall -o main.o main.c  src/mem_management.h src/mem_management.c src/file_reader.h src/file_reader.c src/visitor.h src/visitor.c src/parser.h src/parser.c src/ast.h src/ast.c src/lexer.h src/lexer.c src/token.h src/token.c

run: src/mem_management.h src/mem_management.c src/file_reader.h src/file_reader.c src/visitor.h src/visitor.c src/parser.h src/parser.c src/ast.h src/ast.c src/lexer.h src/lexer.c  src/token.h src/token.c main.c
	./main.o

clean:
	-rm *.o