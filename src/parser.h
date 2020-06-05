#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct {
    struct{
        int ValueIndex,TypeIndex;
        char Type[8][0xFFFF];
        char Value[0xFFF][0xFFFF];
    } ForPrint;
    struct{
        int ValueIndex,TypeIndex;
        char Type[8][0xFFFF];
        char Value[0xFFF][0xFFFF];
    } ForSetup;
} TypeAndValue;

typedef struct PARSER_STRUCT {
    lexer_T* lexer;
    token_T* current_token;
    token_T* prev_token;
} parser_T;

parser_T* init_parser(lexer_T* lexer);
void parser_eat(TypeAndValue* TAV,parser_T* parser, int token_type);
AST_T* parser_parse(parser_T* parser);
AST_T* parser_parse_preVar(parser_T* parser);
AST_T* parser_parse_statement(parser_T* parser);
AST_T* parser_parse_statements(TypeAndValue* TAV,parser_T* parser);
AST_T* parser_parse_expr(parser_T* parser);
AST_T* parser_parse_factor(parser_T* parser);
AST_T* parser_parse_term(parser_T* parser);
AST_T* parser_parse_function_call(parser_T* parser);
AST_T* parser_parse_variable_definition(parser_T* parser);
AST_T* parser_parse_variable(TypeAndValue* TAV,parser_T* parser);
AST_T* parser_parse_string(TypeAndValue* TAV,parser_T* parser);
AST_T* parser_parse_id(parser_T* parser);
lexer_T* parser_parser_pre_variable(lexer_T* lexer);

#endif