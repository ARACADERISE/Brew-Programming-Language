#include "ast.h"

AST_T* init_ast(int type) {
    AST_T* ast = calloc(1,sizeof(struct AST_STRUCT));
    ast->type = type;

    /* For AST_VARIBALE_DEFINITION */
    ast->variable_definition_variable_name = (void*)0;
    ast->variable_definition_value = (void*)0;

    /* For AST_PREVAR_DEFINITION */
    ast->PreVar_name = (void*)0;
    ast->PreVar_value = (void*)9;

    /* For AST_VARIABLE */
    ast->variable_name = (void*)0;

    /* For AST_PREVAR */
    ast->PreVar_variable_name = (void*)0;

    /* For AST_FUNCTION_CALL */
    ast->function_call_name = (void*)0;
    ast->function_call_arguments = (void*)0;
    ast->function_call_arguments_size = 0;

    /* For AST_STRING */
    ast->string_value = (void*)0;

    /* For AST_COMPOUND */
    ast->compound_value = (void*)0;
    ast->compound_size = 0;

    return ast;
}