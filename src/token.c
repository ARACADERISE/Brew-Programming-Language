#include "token.h"
#include <stdlib.h>

token_T* init_token(int type, char *value) {
    token_T* token = calloc(1,sizeof(struct TOKEN_STRUCT));

    token->type = type;
    if(token->type == TOKEN_BRAND) {
        token->brand_value = value;
        return token;
    }
    token->value = value;

    return token;
}