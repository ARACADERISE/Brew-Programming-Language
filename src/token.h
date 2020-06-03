#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID,
        TOKEN_COLON,
        TOKEN_STRING,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_CHAR,
        TOKEN_SEMI,
        TOKEN_LSQRBRACK,
        TOKEN_RSQRBRACK,
        TOKEN_LPARENT,
        TOKEN_RPARENT
    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);

#endif