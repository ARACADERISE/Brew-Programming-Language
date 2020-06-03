#include <stdio.h>
#include <string.h>
#include "lexer/lexer.h"

int main(void) {

    lexer_T* lexer = init_lexer("[STRING]myName:\"AIDAN\";\n");
    token_T* token = (void*)0;

    while((token=lexer_get_next_token(lexer)) != (void*)0) {
        printf("TOKEN(%d,%s)\n",token->type,token->value);
    }

    return 0;
}