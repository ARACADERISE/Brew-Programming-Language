#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdbool.h>

extern bool BrandNeeded;

typedef struct LEXER_STRUCT {
    int line;
    char c, type[15];
    unsigned int i;
    char *contents;

    struct {
        /* integer value */
        int int_value;
        int isNeg;

        /* For Decorators: Tabs */
        int isTabbed;
        int tabAmmount;
        /* For Decorators: Breaks */
        int isNewLine;
        int newLineAmmount;
        /* For Decorators: AppendVar */
        int is_appended;
        char* var_name_appended;
        char* prev_string_value;
        /* AppendVar type holders */
        int int__value;
        char* string_value;
        char* appended_type;
        /* holds the print statements type */
        char* print_type;
    } values;
} lexer_T;

int lexer_get_bit_assignment(lexer_T* lexer);
int lexer_collect_tab_ammount(lexer_T* lexer);
lexer_T* init_lexer(char *contents);
void lexer_advance(lexer_T *lexer);
token_T* lexer_collect_integer_value(lexer_T* lexer);
token_T* lexer_collect_char_value(lexer_T* lexer);
lexer_T* lexer_collect_print_type(lexer_T* lexer, char* type);
void lexer_skip_whitespace(lexer_T *lexer);
token_T* lexer_get_next_token(lexer_T* lexer);
token_T* lexer_collect_string(lexer_T* lexer);
token_T* lexer_collect_id(lexer_T* lexer);
token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token);
char* lexer_get_current_char_as_string(lexer_T* lexer);

#endif