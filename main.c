#include <stdio.h>
#include <string.h>
#include "src/lexer.h"
#include "src/parser.h"

int main(void) {

    lexer_T* lexer = init_lexer("make [S]myName:\"AIDAN\";\n""print[S](myName);");
    token_T* token = (void*)0;

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);

    printf("%d:%ld\n",root->type,root->compound_size);

    return 0;
}