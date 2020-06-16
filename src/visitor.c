#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parser.h"
#include "visitor.h"

AST_T* visitor_visit(AST_T* node) {
    //printf("%d:",node->type);
    switch(node->type) {
        case AST_PREVAR_DEFINITION: return visitor_visit_prevar_definition(node);break;
        //case AST_MEMALLOC_FUNCTION_CALL: return visitor_visit_memalloc_function_call(node);break;
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(node);break;
        case AST_PREVAR: return 0;break;
        case AST_VARIABLE: return visitor_visit_variable(node);break;
        case AST_PREVAR_FUNCTION_CALL: return visitor_visit_prevar_function_call(node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(node);break;
        case AST_STRING: return visitor_visit_string(node);break;
        case AST_COMPOUND: return visitor_visit_compound(node);break;
        case AST_NOOP: return node; break;
    }

    printf("Err ~ Uncaught type: %d..%s\n",node->type,node->string_value);
    exit(1);

    return init_ast(AST_NOOP);
}
AST_T* visitor_visit_memalloc_function_call(AST_T* node) {
    printf("brand variable %s with %ld bits -> %s\n",node->brand_var_name,node->bits_to_assign,node->_func_name);
}
AST_T* visitor_visit_prevar_definition(AST_T* node) {
    printf("VARCONST DECLARED\n");
}
AST_T* visitor_visit_prevar_function_call(AST_T* node) {
    printf("Print FUNCTION CALLED\n");
}
AST_T* visitor_visit_variable_definition(AST_T* node) {
    printf("make VARIABLE DECLARED\n");
}
AST_T* visitor_visit_function_call(AST_T* node) {
    printf("print[]() FUNCTION CALLED\n");
}
AST_T* visitor_visit_variable(AST_T* node) {
}
AST_T* visitor_visit_string(AST_T* node) {

}
AST_T* visitor_visit_compound(AST_T* node) {
    for(int i = 0; i < node->compound_size; i++) {
        visitor_visit(node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}