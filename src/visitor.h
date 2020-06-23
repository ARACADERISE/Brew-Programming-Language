#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"
#include "ast.h"

typedef struct VISITOR_STRUCT {
    AST_T** variable_definitions;
    lexer_T* lexer;
    parser_T* parser;
    size_t variable_definitions_size;
} visitor_T;

visitor_T* init_visitor();
AST_T* visitor_visit_memalloc_function_call(visitor_T* visitor,AST_T* node);
visitor_T* init_visitor();
AST_T* visitor_visit_prevar_definition(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_prevar_function_call(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_variable_definition(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_function_call(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node);
AST_T* visitor_visit_variable(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_string(visitor_T* visitor,AST_T* node);
AST_T* visitor_visit_compound(visitor_T* visitor,AST_T* node);

#endif