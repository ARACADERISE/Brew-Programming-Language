#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "visitor.h"

static AST_T* print_function(visitor_T* visitor,AST_T** args, int size) {
    for(int i = 0; i < size; i++) {
        AST_T* visited = visitor_visit(visitor,args[i]);
        switch(visited->type) {
            case AST_STRING: {
                if(strcmp(visitor->lexer->values.print_type,"str")==0||strcmp(visitor->lexer->values.print_type,"any")==0)
                    printf("%s\n",visited->string_value);
                else {
                    printf("\n\nErr[LINE %d]: Print is of type %s. Attempted to print type other than char\n\n",visitor->lexer->line,visitor->lexer->values.print_type);
                    exit(1);
                }
                break;
            }

            case AST_INT: {
                if(strcmp(visitor->lexer->values.print_type,"int")==0||strcmp(visitor->lexer->values.print_type,"any")==0)
                    printf("%d",visited->int_value);
                else {
                    printf("\n\nErr[LINE %d]: Print is of type %s. Attempted to print type other than char\n\n",visitor->lexer->line,visitor->lexer->values.print_type);
                    exit(1);
                }
                break;
            }
            default: printf("\n\n%p\n\n",visited);break;
        }
    }
    return init_ast(AST_NOOP);
}

visitor_T* init_visitor(lexer_T* lexer) {
    visitor_T* visitor = calloc(1,sizeof(struct VISITOR_STRUCT));
    visitor->variable_definitions = (void*)0;/*calloc(1,sizeof(visitor->variable_definitions));*/
    visitor->variable_definitions_size = 0;
    visitor->lexer = lexer;

    return visitor;
}
AST_T* visitor_visit(visitor_T* visitor,AST_T* node) {
    switch(node->type) {
        case AST_PREVAR_DEFINITION: return visitor_visit_prevar_definition(visitor,node);break;
        //case AST_MEMALLOC_FUNCTION_CALL: return visitor_visit_memalloc_function_call(node);break;
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor,node);break;
        case AST_PREVAR: return 0;break;
        case AST_VARIABLE: return visitor_visit_variable(visitor,node);break;
        case AST_PREVAR_FUNCTION_CALL: return visitor_visit_prevar_function_call(visitor,node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor,node);break;
        case AST_STRING: return visitor_visit_string(visitor,node);break;
        case AST_INT: return visitor_visit_int(visitor,node);break;
        case AST_COMPOUND: return visitor_visit_compound(visitor,node);break;
        case AST_NOOP: return node; break;
    }

    printf("Err ~ Uncaught type: %d..%s\n",node->type,node->string_value);
    exit(1);

    return init_ast(AST_NOOP);
}
void ALLOCATE_VAR_DEFINITION_MEMORY(visitor_T* visitor) {
    visitor->variable_definitions = calloc(1,sizeof(struct AST_T*));
}
AST_T* visitor_visit_memalloc_function_call(visitor_T* visitor,AST_T* node) {
  ALLOCATE_VAR_DEFINITION_MEMORY(visitor);
  //printf("ALLOCATIN MEMORY FOR %s",node->brand_var_name);
}
AST_T* visitor_visit_prevar_definition(visitor_T* visitor,AST_T* node) {
    if(visitor->variable_definitions==(void*)0)
        ALLOCATE_VAR_DEFINITION_MEMORY(visitor);
}
AST_T* visitor_visit_prevar_function_call(visitor_T* visitor,AST_T* node) {
}
AST_T* visitor_visit_variable_definition(visitor_T* visitor,AST_T* node) {

    if(visitor->variable_definitions==(void*)0) {
        ALLOCATE_VAR_DEFINITION_MEMORY(visitor);
        visitor->variable_definitions[0] = node;
        visitor->variable_definitions_size+=1;
    }
    else {
        visitor->variable_definitions_size+=1;

        visitor->variable_definitions = realloc(visitor->variable_definitions, visitor->variable_definitions_size*sizeof(struct AST_T*));

        visitor->variable_definitions[visitor->variable_definitions_size-1] = node;
    }

    return node;
}
AST_T* visitor_visit_function_call(visitor_T* visitor,AST_T* node) {

    if(strcmp(node->function_call_name,"print")==0) {
        return print_function(visitor,node->function_call_arguments,node->function_call_arguments_size);
    }

    printf("\n\nUnknown method %s\n\n",node->function_call_name);
    exit(1);
}
AST_T* visitor_visit_variable(visitor_T* visitor,AST_T* node) {
    for(int i = 0; i < visitor->variable_definitions_size; i++) {
        AST_T* variable = visitor->variable_definitions[i];
        if(strcmp(variable->variable_definition_variable_name,node->variable_name)==0)
            return visitor_visit(visitor,variable->variable_definition_value);
    }
    printf("\n\nErr: %s not declared/made or is a constant(varconst)\n\n",node->variable_name);
    return node;
}
AST_T* visitor_visit_string(visitor_T* visitor,AST_T* node) {
    return node;
}
AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node) {
    return node;
}
AST_T* visitor_visit_compound(visitor_T* visitor,AST_T* node) {
    for(int i = 0; i < node->compound_size; i++) {
        visitor_visit(visitor,node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}