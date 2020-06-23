#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdbool.h>
#include <stdlib.h>

extern bool BrandNeeded;

typedef struct LEXER_STRUCT {
    int line;
    char c, type[15];
    unsigned int i;
    char *contents;

    struct {
        /* Init start for decorators */
        int hasDecorator;
        /* For Decorators: Reform: PushValue */
        int isPushValue;
        char* pushValue;
        /* integer value */
        int int_value;
        int isNeg;
        /* For Decorators: Tabs */
        int isTabbedString;
        int isTabbedInt;
        int tabAmmount;
        /* For Decorators: Breaks */
        int isNewLine;
        int newLineAmmount;
        /* For Decorators: END: Wrap */
        int isEND;
        int isWrapped;
        char* wrapStringWith;
        int breakAmmountOfTimes;
        int ammountOfQuotes;
        /* For Decorators: END: Wrap: reference*/
        int isReference;
        int isDerived;
        char* ref_for_variable;
        char* ref_var_name;
        char* ref_var_value_POINTER;
        size_t* size_of_referenced_variable; // Array of 2, [0] = old value size, [1] new value size
        /* For Decorators: END: Wrap: reference: working with the memory*/
        int isTerminatedMemory;
        int isDumped_ReAllocatedMemory;
        int isSameMemory;
        /* For \0, either for END or put in by user */
        int hasEndAssignment;
        /* holds the print statements type */
        char* print_type;
    } values;
} lexer_T;

int lexer_get_bit_assignment(lexer_T* lexer);
int lexer_collect_ammount(lexer_T* lexer);
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
char* get_current_as_string(lexer_T* lexer);

#endif