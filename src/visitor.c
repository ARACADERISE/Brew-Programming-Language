#include <stdio.h>
#include "visitor.h"

AST_T* visitor_visit(visitor_T* visitor,AST_T* node) {
    switch(node->type) {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor,node);break;
        case AST_VARIABLE: return visitor_visit_variable(visitor,node);break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor,node);break;
        case AST_STRING: return visitor_visit_string(visitor,node);break;
        case AST_COMPOUND: return visitor_visit_compound(visitor,node);break;
        case AST_NOOP: return node; break;
    }

    printf("Err ~ Uncaught type: %d..%s\n",node->type,node->string_value);
    exit(1);

    return init_ast(AST_NOOP);
}
AST_T* visitor_visit_variable_definition(visitor_T* visitor,AST_T* node) {
    printf("AY");
}
AST_T* visitor_visit_function_call(visitor_T* visitor,AST_T* node) {

}
AST_T* visitor_visit_variable(visitor_T* visitor,AST_T* node) {

}
AST_T* visitor_visit_string(visitor_T* visitor,AST_T* node) {

}
AST_T* visitor_visit_compound(visitor_T* visitor,AST_T* node) {
    for(int i = 0; i < node->compound_size; i++) {
        visitor_visit(visitor,node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}