#include <stdio.h>
#include <string.h>
#include "src/lexer.h"
#include "src/parser.h"
#include "src/visitor.h"

int main(void) {

    lexer_T* lexer = init_lexer("#>MAKE \"HEY\";print[]()");
    token_T* token = (void*)0;

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);
    //visitor_T* visitor = init_visitor();
    visitor_visit(root);

    return 0;
}