#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "visitor.h"

char* break_sequence_(visitor_T* visitor) {
    char* break_sequence = calloc(visitor->lexer->values.breakAmmountOfTimes,sizeof(char));
    for(int i = 0; i < visitor->lexer->values.breakAmmountOfTimes; i++) {
        break_sequence[i] = visitor->lexer->values.wrapStringWith[1]; // '\n'

        if(!(visitor->lexer->values.hasEndAssignment==0))
            break_sequence[i+1] = '\0';
    }

    return break_sequence;
}

void print_with_decorator(AST_T* ast_,visitor_T* visitor) {
    if(visitor->lexer->values.isTabbedString==0) {
        char* tab_sequence = calloc(visitor->lexer->values.tabAmmount+1,sizeof(char));

        for(int i = 0; i < visitor->lexer->values.tabAmmount; i++) {
            tab_sequence[i] = '\t';
        }
        if(visitor->lexer->values.isEND==0) {
            if(visitor->lexer->values.isWrapped==0) {
                if(visitor->lexer->values.breakAmmountOfTimes>0&&visitor->lexer->values.ammountOfQuotes>0) {
                    char* break_sequence = calloc(visitor->lexer->values.breakAmmountOfTimes+1,sizeof(char));
                    char* quote_sequence = calloc(visitor->lexer->values.ammountOfQuotes,sizeof(char));
                    break_sequence = break_sequence_(visitor);
                    if(!(visitor->lexer->values.ammountOfQuotes==1)) {
                        for(int i = 0; i < visitor->lexer->values.ammountOfQuotes; i++) {
                            quote_sequence[i] = visitor->lexer->values.wrapStringWith[2]; // '"'
                        }
                        printf("%s%s%s%s%s%s%c",break_sequence,quote_sequence,ast_->string_value,tab_sequence,quote_sequence,break_sequence,visitor->lexer->values.wrapStringWith[0]);
                        //free(visitor->lexer->values.wrapStringWith);
                    } else {
                        printf("%s%c%s%s%c%s%c",break_sequence,visitor->lexer->values.wrapStringWith[2],ast_->string_value,tab_sequence,visitor->lexer->values.wrapStringWith[2],break_sequence,visitor->lexer->values.wrapStringWith[0]);
                        //free(visitor->lexer->values.wrapStringWith);
                    }
                } else if(visitor->lexer->values.ammountOfQuotes>0) {
                    char* quote_sequence = calloc(visitor->lexer->values.ammountOfQuotes,sizeof(char));

                    for(int i = 0; i < visitor->lexer->values.ammountOfQuotes; i++) {
                        quote_sequence[i] = visitor->lexer->values.wrapStringWith[2];
                    }
                    printf("%s%s%s%s",quote_sequence,ast_->string_value,tab_sequence,quote_sequence);
                } else if(visitor->lexer->values.breakAmmountOfTimes>0) {
                    char* break_sequence = calloc(visitor->lexer->values.breakAmmountOfTimes,sizeof(char));
                    break_sequence = break_sequence_(visitor);
                    printf("%s%s%s%s",break_sequence,ast_->string_value,tab_sequence,break_sequence);
                } else {
                    if(!(visitor->lexer->values.hasEndAssignment==0))printf("%c%s%s%c%c",visitor->lexer->values.wrapStringWith[1],ast_->string_value,tab_sequence,visitor->lexer->values.wrapStringWith[1],visitor->lexer->values.wrapStringWith[0]);
                    else printf("%c%s%s%c",visitor->lexer->values.wrapStringWith[1],ast_->string_value,tab_sequence,visitor->lexer->values.wrapStringWith[1]);
                    //free(visitor->lexer->values.wrapStringWith);
                }
            }
        } else printf("%s%s",ast_->string_value,tab_sequence);
    } else printf("%s\n",ast_->string_value);
}

static AST_T* print_function(visitor_T* visitor,AST_T** args, int size) {
    /* To see if another type was printed for the appendVar decorator*/
    int string_printed=0;
    int int_printed=0;

    for(int i = 0; i < size; i++) {
        AST_T* visited = visitor_visit(visitor,args[i]);
        switch(visited->type) {
            case AST_STRING: {
                if(strcmp(visitor->lexer->values.print_type,"str")==0||strcmp(visitor->lexer->values.print_type,"any")==0) {
                    print_with_decorator(visited,visitor);
                }
                else {
                    printf("\n\nErr[LINE %d]: Print is of type %s. Attempted to print type other than char\n\n",visitor->lexer->line,visitor->lexer->values.print_type);
                    exit(1);
                }
                break;
            }
            case AST_INT: {
                if(strcmp(visitor->lexer->values.print_type,"int")==0||strcmp(visitor->lexer->values.print_type,"any")==0) {
                    /*if(visitor->lexer->values.isTabbed==0) {
                        char tab_sequence[visitor->lexer->values.tabAmmount];
                        for(int i = 0; i < visitor->lexer->values.tabAmmount; i++) {
                            tab_sequence[i] = '\t';
                        }
                        printf("%d%s",visited->int_value,tab_sequence);
                    } else*/ 
                    printf("%d",visited->int_value);
                }
                else {
                    printf("\n\nErr[LINE %d]: Print is of type %s. Attempted to print type other than char\n\n",visitor->lexer->line,visitor->lexer->values.print_type);
                    exit(1);
                }
                break;
            }
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