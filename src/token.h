#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_STRING,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_CHAR,
        TOKEN_TYPE_ANY, /* For the make statement */
        TOKEN_TYPE_A, /* For the make statement */        
        TOKEN_SEMI,
        TOKEN_LSQRBRACK,
        TOKEN_RSQRBRACK,
        TOKEN_LPARENT,
        TOKEN_RPARENT,
        TOKEN_LCURL, // {
        TOKEN_RCURL, // }
        TOKEN_PREVAR_END_SYMBOL,
        TOKEN_PRESET_TYPE_SETVAR,
        TOKEN_USE_KEYWORD, /* To-Do: Give meaning to this */
        TOKEN_EOF,
    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);

#endif