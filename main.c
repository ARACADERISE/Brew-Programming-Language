#include <stdio.h>
#include <string.h>
#include "src/lexer.h"
#include "src/parser.h"
#include "src/visitor.h"

int main(void) {

    //lexer_T* lexer = init_lexer("varconst{NAME}:\"Hi\" Print();");
    lexer_T* lexer = init_lexer(
        "#HEY\n"
        "make [I]age: brand age {\nmemalloc(32);\n};\n"
        //"make [I]age: 15;\n"
        "print[any]();\n"
        "varconst{HELLO}: \"HI\";\n"
        "/*Print is used to print varconst variables*/"
        "Print();"
        );
    /* This will be used for testing the "brand" keyword. */
    //lexer_T* lexer = init_lexer("varconst{sayHi};");
    token_T* token = (void*)0;

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);
    
    //visitor_T* visitor = init_visitor();
    visitor_visit(root);

    return 0;
}