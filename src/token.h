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
        TOKEN_TYPE_ANY,
        TOKEN_SEMI,
        TOKEN_LSQRBRACK,
        TOKEN_RSQRBRACK,
        TOKEN_LPARENT,
        TOKEN_RPARENT,
        TOKEN_PRESET, /* 
            This is just for the lexer when checking if the line is a Constant or Macro. 
            It makes the return easier.
        */
        TOKEN_PRESET_TYPE_MACRO,
        TOKEN_PRESET_TYPE_SETVAR,
        TOKEN_USE_KEYWORD, /* To-Do: Give meaning to this */
        TOKEN_EOF,
    } type;

    char *value;
} token_T;

token_T *init_token(int type, char *value);

#endif