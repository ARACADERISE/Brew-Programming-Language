#include <stdio.h>
#include <string.h>
#include "src/lexer.h"
#include "src/parser.h"
#include "src/visitor.h"

int main(void) {

    lexer_T* lexer = init_lexer("make ~myName:\"AIDAN\";\n""print[Str](myName);\n");
    token_T* token = (void*)0;

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);
    visitor_visit(root);

    return 0;
}