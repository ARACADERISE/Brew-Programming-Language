#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID,
        TOKEN_COLON,
        TOKEN_EQUALS,
        TOKEN_COMMA,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_string,
        TOKEN_TYPE_STRING_VALUE,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_INT_VALUE,
        TOKEN_TYPE_CHAR,
        TOKEN_TYPE_CHAR_VALUE,
        TOKEN_TYPE_ANY, /* For the make statement */
        TOKEN_TYPE_A, /* For the make statement */
        TOKEN_SEMI,
        TOKEN_NEGATIVE_SYMBOL,
        TOKEN_ADVANCEMENT_OPERATOR,
        TOKEN_LSQRBRACK,
        TOKEN_RSQRBRACK,
        TOKEN_LPARENT,
        TOKEN_RPARENT,
        TOKEN_BRAND, /* ALLOCATE MEMORY FOR VARIABLES */
        TOKEN_LCURL, // {
        TOKEN_RCURL, // }
        TOKEN_NO_VALUE,
        TOKEN_PREVAR_END_SYMBOL,
        TOKEN_PRESET_TYPE_SETVAR,
        TOKEN_TAB_VALUE,
        TOKEN_USE_KEYWORD, /* To-Do: Give meaning to this */
        TOKEN_EOF
    } type;

    char *value;
    char *brand_value;
} token_T;

token_T *init_token(int type, char *value);

#endif