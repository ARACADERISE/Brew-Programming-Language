#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "parser.h"
#include "visitor.h"
#include "lexer.h"

//static bool IsPre = false;
int BrandNeeded_ = 1;
static int isSigned=1; // false by default
static int isUnsigned=0; // true by default

static inline lexer_T* set_token_back(lexer_T* lexer, int ammount) {

    lexer->i -= ammount;
    lexer->c = lexer->contents[lexer->i];

    return lexer;
}
static inline void PrintErr(parser_T* parser) {
    printf("\n\nErr[LINE %d]: Type of %s is not valid for print declaration. Types S, I, C and ~ are for variable declarations.\n\n",parser->lexer->line,parser->current_token->value);
    exit(1);
}
static inline TypeAndValue* UpdTAV(TypeAndValue* TAV) {

    TAV->ForSetup.ValueIndex=0;
    TAV->ForSetup.TypeIndex=0;
    TAV->ForPrint.ValueIndex=0;
    TAV->ForPrint.TypeIndex=0;

    return TAV;
}

parser_T* init_parser(lexer_T* lexer) {
    parser_T* parser = calloc(1,sizeof(struct PARSER_STRUCT));

    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);
    
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    return parser;
}
void parser_eat(TypeAndValue* TAV,parser_T* parser, int token_type) {
    
    if(strcmp(parser->current_token->value,"print")==0) {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);

        if(parser->current_token->type==TOKEN_RSQRBRACK) {
            printf("\n\nErr[LINE %d]: right square bracket(']') found but not left square bracket('[').\n\n",parser->lexer->line);
            exit(1);
        }
        if(parser->current_token->type==TOKEN_LSQRBRACK) {
            parser->current_token = lexer_get_next_token(parser->lexer);
            lexer_collect_print_type(parser->lexer, parser->current_token->value);
        } else {
            printf("\n\nERR[LINE %d]: missing left square bracket('[') for print statement.\n\n",parser->lexer->line);
            exit(1);
        }

        /*
            * S, I, C and ~ are all declaraction types. The print method will have 
            * type Str:String, Char:Char, Int:Integer or empty square brackets 
            for print type any 
        */
    }

    if(parser->current_token->type==TOKEN_PRESET_TYPE_SETVAR){
        parser->current_token = lexer_get_next_token(parser->lexer);
    } else {
        
        if(parser->current_token->type == token_type) {
            if(strcmp(parser->current_token->value,"~")==0) {
                strcpy(TAV->ForSetup.Type[TAV->ForSetup.TypeIndex],"Any");
            }
            parser->current_token = lexer_get_next_token(parser->lexer);
        } else {
            printf("\n\nErr[LINE %d]:Unexpected token '%s' with type %d\n\n",parser->lexer->line,parser->current_token->value,parser->current_token->type);
            exit(1);
        }
    }
}
AST_T* parser_parse(parser_T* parser) {
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    return parser_parse_statements(TAV,parser);
}
AST_T* parser_parse_statement(parser_T* parser) {
    TypeAndValue* TAV = calloc(1, sizeof(TypeAndValue));
    UpdTAV(TAV);

    switch(parser->current_token->type) {
        case TOKEN_ID: return parser_parse_id(parser);
        /*case TOKEN_PRESET_TYPE_SETVAR: {
            //parser_eat(TAV,parser,TOKEN_PRESET_TYPE_SETVAR);
            return parser_parse_id(parser);
        }*/
    }
    /*if(parser->current_token->value[0]=='c') {
        parser->current_token->type=0;
        return parser_parse_id(parser);
    }*/

    AST_T* noop = init_ast(AST_NOOP);
    noop->total_lines = parser->lexer->line;
    return noop;
}
AST_T* parser_parse_memalloc_function_call(TypeAndValue* TAV, parser_T* parser, char* b_v_n) {
    AST_T* brand_var = init_ast(AST_MEMALLOC_FUNCTION_CALL);
    brand_var->bits_to_assign = lexer_get_bit_assignment(parser->lexer);
    brand_var->brand_var_name = b_v_n;
    brand_var->_func_name = "memalloc";

    parser_eat(TAV,parser,TOKEN_LPARENT);

    if(parser->current_token->type==TOKEN_RPARENT)
        parser_eat(TAV,parser,TOKEN_RPARENT);
    else {
        printf("\n\nErr[LINE %d]: Missing ')'.\n\n",parser->lexer->line);
        exit(1);
    }
    if(parser->current_token->type==TOKEN_SEMI)
        parser_eat(TAV,parser,TOKEN_SEMI);
    else {
        printf("\n\nErr[LINE %d]: Abrupt end to function call.\n\n",parser->lexer->line);
        exit(1);
    }
    if(parser->current_token->type==TOKEN_RCURL)
        parser_eat(TAV,parser,TOKEN_RCURL);
    else {
        printf("\n\nErr[LINE %d]: Missing '}'.\n\n",parser->lexer->line);
        exit(1);
    }

    /* Needs to be done here, or else the visitor would never visit. */
    visitor_T* visitor = init_visitor();
    return visitor_visit_memalloc_function_call(visitor,brand_var);
}
AST_T* parser_parse_brand_variable(TypeAndValue* TAV,parser_T* parser,char* variable_definition_variable_name) {
    if(BrandNeeded_==0) {
        parser_eat(TAV,parser,TOKEN_ID);
        char* brand_var_name = parser->current_token->value;
        parser_eat(TAV,parser,TOKEN_ID);
        
        if(strcmp(brand_var_name,variable_definition_variable_name)==0) {
            if(parser->current_token->type==TOKEN_LCURL)
                parser_eat(TAV,parser,TOKEN_LCURL);
            else {
                printf("\n\nErr[LINE %d]: Missing '{'.\n\n",parser->lexer->line);
                exit(1);
            }

            if(parser->current_token->type==TOKEN_RCURL) {
                printf("\n\nErr[LINE %d]: brand body is empty. Example of brand keyword:\nmake [S]name: brand name {\n\tmemalloc(32);\n};\n\n",parser->lexer->line);
                exit(1);
            }
            
            char* mem_function_name = parser->current_token->value;
            parser_eat(TAV,parser,TOKEN_ID);
            
            BrandNeeded_=1;
            if(strcmp(mem_function_name,"memalloc")==0)
                return parser_parse_memalloc_function_call(TAV,parser,brand_var_name);
        } else {
            printf("\n\nErr[LINE %d]: Branding %s instead of %s.\n\n",parser->lexer->line,brand_var_name,variable_definition_variable_name);
            exit(1);
        }
    }
    return init_ast(AST_NOOP);
}
AST_T* parser_parse_preVarConstant(parser_T* parser) {
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);
    strcpy(parser->lexer->type,"Any");

    parser_eat(TAV,parser, TOKEN_ID);

    if(parser->current_token->type==TOKEN_RCURL) {
        printf("\n\nErr[LINE %d]: Found ('}'), but not ('{').\n\n",parser->lexer->line);
        exit(1);
    }

    if(parser->current_token->type==TOKEN_LCURL)
        parser_eat(TAV,parser,TOKEN_LCURL);
    else {
        printf("\n\nErr[LINE %d]: syntax of varconst is incorrect. Needs '{' to start declaration, found %s\n\n",parser->lexer->line,parser->current_token->value);
        exit(1);
    }

    if(parser->current_token->type == TOKEN_LSQRBRACK) {
        parser_eat(TAV,parser,TOKEN_LSQRBRACK);
        printf("\n\n\033[1;35mWarning[LINE %d]:\033[0m varconst does not require the [TYPE] param.\n\033[1;35mWarning:\033[0m The [TYPE] param might cause the varconst variable to be more prone to outcome errors.\n\n",parser->lexer->line);
        if(!(
            parser->current_token->type==TOKEN_TYPE_STRING||
            parser->current_token->type==TOKEN_TYPE_INT||
            parser->current_token->type==TOKEN_TYPE_CHAR||
            parser->current_token->type==TOKEN_TYPE_ANY||
            parser->current_token->type==TOKEN_TYPE_A
        )) {
            if(!(parser->current_token->type==TOKEN_RSQRBRACK)) {
                printf("\n\nErr[LINE %d]: Missing right square bracket(']') for variable declaration.\n\n",parser->lexer->line);
                exit(1);
            }
        }
        if(parser->current_token->type==TOKEN_TYPE_STRING) /* = [S]*/
            parser_eat(TAV,parser,TOKEN_TYPE_STRING);
        else if(parser->current_token->type==TOKEN_TYPE_INT) /* = [I]*/
            parser_eat(TAV,parser,TOKEN_TYPE_INT);
        else if(parser->current_token->type==TOKEN_TYPE_CHAR) /* = [C]*/ {
            parser_eat(TAV,parser,TOKEN_TYPE_CHAR);
            strcpy(TAV->ForSetup.Type[TAV->ForSetup.TypeIndex],"Char");
        }
        else if(parser->current_token->type==TOKEN_TYPE_ANY) /* = ~ or [~]*/ {
            strcpy(TAV->ForSetup.Type[TAV->ForSetup.TypeIndex],"Any");
            parser_eat(TAV,parser,TOKEN_TYPE_ANY);
        }
        else if(parser->current_token->type == TOKEN_TYPE_A)/* = [A]*/ {
            parser_eat(TAV,parser,TOKEN_TYPE_A);
        }
        else {
            printf("\n\nErr[LINE %d]: [TYPE] param is empty.\nThe [TYPE] param needs a type of S(STRING) I(INTEGER) C(CHAR) A(ANY), it cannot be left empty.\n\n",parser->lexer->line);
            exit(1);
        }
        if(!(parser->current_token->type==TOKEN_RSQRBRACK)) {
            printf("\nErr[LINE %d]: [TYPE] param added by user is missing closing square bracket(']').\n\n",parser->lexer->line);
            exit(1);
        }
        else
            parser_eat(TAV,parser,TOKEN_RSQRBRACK);
    }
    /*else if(!(parser->current_token->type == TOKEN_LSQRBRACK))
        if(!(parser->current_token->type==TOKEN_TYPE_ANY)) {
            printf("\n\nErr: [TYPE] param added by user left empty.\nNeeds a specific type. Example:\nvarconst{[S]sayHi}:\"Hi\";\n\n");
            exit(1);
        }*/
    
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(TAV,parser,TOKEN_ID);
    if(parser->current_token->type == TOKEN_RCURL)
        parser_eat(TAV,parser, TOKEN_RCURL);
    else {
        printf("\nErr[LINE %d]: varconst needs closing curly brace('}').\n\n",parser->lexer->line);
        exit(1);
    }

    if(parser->current_token->type==TOKEN_COLON)
        parser_eat(TAV,parser, TOKEN_COLON);
    else {
        printf("\n\nErr[LINE %d]: Unable to find ':'.\n\n",parser->lexer->line);
        exit(1);
    }
    
    if(strcmp(parser->current_token->value,"brand")==0) {
        printf("\n\nErr[LINE %d]: Cannot brand a constant('varconst'). Must have value assignment.\n\n",parser->lexer->line);
        exit(1);
    }
    
    AST_T* variable_definition_value = parser_parse_expr(parser);
    AST_T* variable_definition = init_ast(AST_PREVAR_DEFINITION);

    variable_definition->PreVar_name = variable_definition_variable_name;
    variable_definition->variable_definition_variable_name = variable_definition->PreVar_name;
    variable_definition->variable_definition_value = variable_definition_value;
    return variable_definition;
}
AST_T* parser_parse_statements(TypeAndValue* TAV,parser_T* parser) {

    //if(!(strcmp(parser->current_token->value,"varconst")==0)) {
        AST_T* compound = init_ast(AST_COMPOUND);
        compound->compound_value = calloc(1,sizeof(struct AST_STRUCT));
        AST_T* ast_statement = parser_parse_statement(parser);

        compound->compound_value[0] = ast_statement;
        compound->compound_size++;

        while(parser->current_token->type == TOKEN_SEMI||parser->current_token->type == TOKEN_PREVAR_END_SYMBOL) {
            if(parser->current_token->type == TOKEN_PREVAR_END_SYMBOL)
                parser_eat(TAV,parser, TOKEN_PREVAR_END_SYMBOL);
            else
                parser_eat(TAV,parser, TOKEN_SEMI);

            AST_T* ast_statement = parser_parse_statement(parser);

            if(ast_statement) {
                compound->compound_size++;
                compound->compound_value = realloc(compound->compound_value,compound->compound_size*sizeof(struct AST_STRUCT));
                compound->compound_value[compound->compound_size-1] = ast_statement;
            }
        }
        return compound;
    /*}
    else {
        AST_T* compound = init_ast(AST_PREVAR_DEFINITION);
        compound->compound_value = calloc(1,sizeof(struct AST_STRUCT));
        AST_T* ast_statement = parser_parse_statement(parser);

        compound->compound_value[0] = ast_statement;
        compound->compound_size++;

        parser_parse_preVarConstant(parser);

        while(parser->current_token->type == TOKEN_SEMI) {
            parser_eat(TAV,parser, TOKEN_SEMI);

            AST_T* ast_statement = parser_parse_statement(parser);

            if(ast_statement) {
                compound->compound_size++;
                compound->compound_value = realloc(compound->compound_value,compound->compound_size*sizeof(struct AST_STRUCT));
                compound->compound_value[compound->compound_size-1] = ast_statement;
            }
        }
        return parser_parse_id(parser);
    }
    return parser_parse_id(parser);*/
}
AST_T* parser_parse_expr(parser_T* parser) {
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    switch(parser->current_token->type) {
        case TOKEN_STRING: return parser_parse_string(TAV,parser);
        case TOKEN_INT: return parser_parse_int(TAV,parser);
        case TOKEN_ID: return parser_parse_id(parser);
    }

    return init_ast(AST_NOOP);
}
AST_T* parser_parse_factor(parser_T* parser) {

}
AST_T* parser_parse_term(parser_T* parser) {

}
/* 
    * Function parser_parse_PreVarConstDef used to stabalize the PreVar definitions
    * within the language. If we wouldn't do this,
    * the compiler would not pick up the next token
    * due to the fact it would be stuck on TOKEN_EOF.
*/
/*parser_T* parser_parse_PreVarConstDef(TypeAndValue* TAV,parser_T* parser) {
    parser->current_token->type=0;

    parser_parse_expr(parser);

    return parser;
}*/
AST_T* parser_parse_preVar_function_call(char* function_name,parser_T* parser) {
    AST_T* function_call = init_ast(AST_PREVAR_FUNCTION_CALL);
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    parser_eat(TAV,parser, TOKEN_LPARENT);
    function_call->function_call_name = parser->current_token->value;

    function_call->function_call_arguments = calloc(1,sizeof(struct AST_STRUCT));
    AST_T* ast_expr = parser_parse_expr(parser);
    if(ast_expr) {
        function_call->function_call_arguments[0] = ast_expr;
        function_call->function_call_arguments_size++;
    }

    while(parser->current_token->type == TOKEN_COMMA) {
        parser_eat(TAV,parser, TOKEN_COMMA);

        AST_T* ast_expr = parser_parse_expr(parser);
        function_call->function_call_arguments_size++;
        function_call->function_call_arguments = realloc(function_call->function_call_arguments,function_call->function_call_arguments_size*sizeof(struct AST_STRUCT));
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr;
    }

    parser_eat(TAV, parser, TOKEN_RPARENT);

    return function_call;

    //return function_call;
}
AST_T* parser_parse_function_call(char* function_name,parser_T* parser) {
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    if(parser->current_token->type==TOKEN_ID)
        parser_eat(TAV, parser, TOKEN_ID);

    function_call->function_call_name = parser->prev_token->value;
    if(parser->current_token->type==TOKEN_LPARENT)
        parser_eat(TAV,parser, TOKEN_LPARENT);

    function_call->function_call_arguments = calloc(1,sizeof(struct AST_STRUCT));
    AST_T* ast_expr = parser_parse_expr(parser);
    //if(ast_expr) {
    function_call->function_call_arguments[0] = ast_expr;
    function_call->function_call_arguments_size++;
    //}
    while(parser->current_token->type == TOKEN_COMMA) {
        parser_eat(TAV,parser, TOKEN_COMMA);

        AST_T* ast_expr = parser_parse_expr(parser);
        function_call->function_call_arguments_size++;
        function_call->function_call_arguments = realloc(function_call->function_call_arguments,function_call->function_call_arguments_size*sizeof(struct AST_STRUCT));
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr;
    }

    parser_eat(TAV, parser, TOKEN_RPARENT);

    return function_call;
}
AST_T* parser_parse_variable_definition(parser_T* parser) {
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    parser_eat(TAV,parser, TOKEN_ID);

    if(parser->current_token->type==TOKEN_RSQRBRACK) {
        printf("\n\nErr[LINE %d]: Found right square bracket(']'), but not the left square bracket('[').\n\n",parser->lexer->line);
        exit(1);
    }

    if(parser->current_token->type == TOKEN_LSQRBRACK) {
        parser_eat(TAV,parser,TOKEN_LSQRBRACK);
        if(!(
            parser->current_token->type==TOKEN_TYPE_STRING||
            parser->current_token->type==TOKEN_TYPE_INT||
            parser->current_token->type==TOKEN_TYPE_CHAR||
            parser->current_token->type==TOKEN_TYPE_ANY||
            parser->current_token->type==TOKEN_TYPE_A||
            parser->current_token->type==TOKEN_NO_VALUE
        )) {
            if(!(parser->current_token->type==TOKEN_RSQRBRACK)) {
                printf("\n\nErr[LINE %d]: Missing right square bracket(']') for [TYPE] param.\n\n",parser->lexer->line);
                exit(1);
            }
        }
    }
    else if(!(parser->current_token->type == TOKEN_LSQRBRACK)) {
        if(!(parser->current_token->type==TOKEN_TYPE_ANY)) {
            printf("\n\nErr[LINE %d]: Missing left square bracket('[') for [TYPE] param. Example of [TYPE] param:\nmake [S]sayHi:\"Hello World\";\n\n",parser->lexer->line);
            exit(1);
        }
    }
    
    int type;
    if(parser->current_token->type==TOKEN_TYPE_STRING) /* = [S]*/ 
        parser_eat(TAV,parser,TOKEN_TYPE_STRING);
    else if(parser->current_token->type==TOKEN_TYPE_INT) /* = [I]*/ {
        parser_eat(TAV,parser,TOKEN_TYPE_INT);
        type = TOKEN_TYPE_INT;
    }
    else if(parser->current_token->type==TOKEN_TYPE_CHAR) /* = [C]*/
        parser_eat(TAV,parser,TOKEN_TYPE_CHAR);
    else if(parser->current_token->type==TOKEN_TYPE_ANY) /* = ~*/ {
        parser_eat(TAV,parser,TOKEN_TYPE_ANY);
    }
    else if(parser->current_token->type == TOKEN_TYPE_A)
        parser_eat(TAV,parser,TOKEN_TYPE_A);
    else if(parser->current_token->type == TOKEN_NO_VALUE)
        parser_eat(TAV,parser, TOKEN_NO_VALUE);
    else {
        printf("\n\nErr[LINE %d]: make [TYPE] param is empty.\nThe [TYPE] param needs a type of S(STRING) I(INTEGER) C(CHAR) A(ANY), it cannot be left empty.\n\n",parser->lexer->line);
        exit(1);
    }

    if(parser->current_token->type == TOKEN_RSQRBRACK)
        parser_eat(TAV,parser, TOKEN_RSQRBRACK);
    else {
        printf("\n\nErr[LINE %d]: Missing right square bracket(']') for [TYPE] param.\n\n",parser->lexer->line);
        exit(1);
    }
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(TAV,parser, TOKEN_ID);
    if(parser->current_token->type==TOKEN_COLON)
        parser_eat(TAV,parser, TOKEN_COLON);
    else {
        if(parser->current_token->type==TOKEN_SEMI)
            BrandNeeded_=0;
        else {
            printf("\n\nErr[LINE %d]: Unable to find ':'.\n\n",parser->lexer->line);
            exit(1);
        }
    }
    /*if(strcmp(parser->current_token->value,"n")==0&&type==TOKEN_TYPE_INT) {
        parser_eat(TAV,parser,TOKEN_ID);
    } else parser->lexer->values.isNeg=1;*/

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    if(BrandNeeded_==0||strcmp(parser->current_token->value,"brand")==0) {
        BrandNeeded_=0;
        parser_parse_brand_variable(TAV,parser,variable_definition_variable_name);
    } else {
        AST_T* variable_definition_value = parser_parse_expr(parser);
        variable_definition->variable_definition_value = variable_definition_value;
    }

    variable_definition->variable_definition_variable_name = variable_definition_variable_name;

    if(parser->current_token->type==TOKEN_LCURL) {
        parser->lexer->values.hasDecorator = 0;
        parser_eat(TAV,parser,TOKEN_LCURL);
        re_check:
        if(strcmp(parser->current_token->value,"Tab")==0) {
            parser->lexer->values.isTabbedString = 0;
            parser_eat(TAV,parser,TOKEN_ID);
            lexer_skip_whitespace(parser->lexer);
            parser->lexer->values.tabAmmount = lexer_collect_ammount(parser->lexer);
            parser_eat(TAV,parser,TOKEN_COLON);
            //parser_eat(TAV,parser,TOKEN_SEMI);
            if(!(parser->current_token->type==TOKEN_RCURL)) goto re_check;
            else goto end;
        }
        if(strcmp(parser->current_token->value,"END")==0) {
            parser->lexer->values.isEND = 0;
            parser_eat(TAV,parser,TOKEN_ID);

            if(parser->current_token->type==TOKEN_COLON) parser_eat(TAV,parser,TOKEN_COLON);
            if(parser->current_token->type==TOKEN_LCURL) {
                parser_eat(TAV,parser,TOKEN_LCURL);
                CheckEND:
                if(strcmp(parser->current_token->value,"Wrap")==0) {
                    parser->lexer->values.isWrapped = 0;
                    parser_eat(TAV,parser,TOKEN_ID);
                    parser_eat(TAV,parser,TOKEN_LCURL);
                    parser->lexer->values.wrapStringWith = calloc(3,sizeof(char)); /* 
                        Allocating 3 memory spaces. Will become bigger later on when more wrap decorator
                        types are introduces.
                    */
                    parser->lexer->values.wrapStringWith[0] = '\0';
                    redo:
                    if(strcmp(parser->current_token->value,"breaks")==0) {
                        parser_eat(TAV,parser,TOKEN_ID);
                        if(parser->current_token->type==TOKEN_LSQRBRACK) {
                            parser->lexer->values.breakAmmountOfTimes = lexer_collect_ammount(parser->lexer);
                            parser->lexer->values.wrapStringWith[1] = '\n';
                            parser_eat(TAV,parser,TOKEN_LSQRBRACK);
                            parser_eat(TAV,parser,TOKEN_RSQRBRACK);
                        } else {
                            parser->lexer->values.breakAmmountOfTimes = 1;
                            parser->lexer->values.wrapStringWith[1] = '\n';
                        }
                        if(parser->current_token->type==TOKEN_RCURL) parser_eat(TAV,parser,TOKEN_RCURL);
                        else goto redo;
                    }
                    if(strcmp(parser->current_token->value,"quotes")==0) {
                        parser_eat(TAV,parser,TOKEN_ID);
                        if(parser->current_token->type==TOKEN_LSQRBRACK) {
                            parser->lexer->values.wrapStringWith[2] = '"';
                            parser->lexer->values.ammountOfQuotes = lexer_collect_ammount(parser->lexer);
                            if(parser->lexer->values.ammountOfQuotes%2!=0&&!(parser->lexer->values.ammountOfQuotes==1)) {
                                printf("\n\nErr[LINE %d]: Cannot have odd amounts of quotes.\n\n",parser->lexer->line);
                                exit(1);
                            }
                            parser_eat(TAV,parser,TOKEN_LSQRBRACK);
                            parser_eat(TAV,parser,TOKEN_RSQRBRACK);
                        } else {
                            parser->lexer->values.ammountOfQuotes = 1;
                            parser->lexer->values.wrapStringWith[2] = '"';
                        }
                    }
                    if(!(parser->current_token->type==TOKEN_RCURL)) goto redo;
                    else parser_eat(TAV,parser,TOKEN_RCURL);
                }
                if(strcmp(parser->current_token->value,"reference")==0) {
                    parser->lexer->values.isReference = 0;
                    parser_eat(TAV,parser,TOKEN_ID);

                    if(parser->current_token->type==TOKEN_COLON)
                        parser_eat(TAV,parser,TOKEN_COLON);
                    
                    parser->lexer->values.ref_for_variable = variable_definition->variable_definition_variable_name;
                    parser->lexer->values.ref_var_name = parser->current_token->value;

                    /* Allocating memory for the rest of the referenced variable */
                    parser->lexer->values.size_of_referenced_variable = calloc(2,sizeof(size_t));
                    parser->lexer->values.size_of_referenced_variable[0] = sizeof(parser->lexer->values.ref_var_name);
                    parser->lexer->values.ref_var_value = malloc(sizeof(parser->lexer->values.ref_var_value));

                    parser_eat(TAV,parser,TOKEN_ID);
                    if(!(parser->current_token->type==TOKEN_RCURL)) goto CheckEND;
                }
                parser_eat(TAV,parser,TOKEN_RCURL);
            }
        }
        lexer_skip_whitespace(parser->lexer);
            
        if(!(parser->current_token->type==TOKEN_RCURL)) goto re_check;
        else goto end;

        end:
        if(parser->lexer->values.isWrapped) {
            /* Allocating new memory */
            parser->lexer->values.wrapStringWith = realloc(
                parser->lexer->values.wrapStringWith,
                3*sizeof(char)
            );
        }
        parser_eat(TAV, parser, TOKEN_RCURL);
    }

    return variable_definition;
}
AST_T* parser_parse_prevar(TypeAndValue* TAV,parser_T* parser) {
    char* preVar_token_value = parser->current_token->value;
    parser_eat(TAV,parser, TOKEN_ID);

    if(parser->current_token->type==TOKEN_LPARENT)
        return parser_parse_preVar_function_call(preVar_token_value,parser);

    AST_T* ast_PreVar = init_ast(AST_PREVAR);
    ast_PreVar->variable_name = preVar_token_value;

    return ast_PreVar;
}
AST_T* parser_parse_variable(TypeAndValue* TAV,parser_T* parser) {

    /*char* token_value = parser->current_token->value;
    parser_eat(TAV,parser, TOKEN_ID);
  
    AST_T* ast_PreVariable = init_ast(AST_VARIABLE);
    ast_PreVariable = token_value;

    return parser_parse_function_call(parser);*/
    /*char* token_value = parser->current_token->value;
    parser_eat(TAV,parser, TOKEN_ID);
    
    if(parser->current_token->type==TOKEN_LSQRBRACK) {
        parser_eat(TAV,parser, TOKEN_LSQRBRACK);
    }
    if(parser->current_token->type==TOKEN_RSQRBRACK) {
        parser_eat(TAV,parser,TOKEN_RSQRBRACK);
    }

    if(parser->current_token->type == TOKEN_LPARENT)
        return parser_parse_function_call(parser);
    
    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    return ast_variable;*/
    char* token_value = parser->current_token->value;
    parser_eat(TAV,parser, TOKEN_ID);

    if(parser->current_token->type == TOKEN_LPARENT) {
        return parser_parse_function_call(token_value,parser);
    }
    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    return ast_variable;
}
AST_T* parser_parse_int(TypeAndValue* TAV, parser_T* parser) {
    AST_T* integer = init_ast(AST_INT);
    integer->int_value = parser->lexer->values.int_value;
    parser_eat(TAV,parser,TOKEN_INT);

    if(parser->current_token->type==TOKEN_LCURL)
        parser_eat(TAV,parser,TOKEN_LCURL);
    if(strcmp(parser->current_token->value,"signed")==0) {
        isSigned=0;
        isUnsigned=1;
        parser_eat(TAV,parser,TOKEN_ID); // signed
    } else if(strcmp(parser->current_token->value,"unsigned")==0)
        parser_eat(TAV,parser,TOKEN_ID); // unsigned
    if(parser->current_token->type==TOKEN_RCURL)
        parser_eat(TAV,parser,TOKEN_RCURL);
    
    if(integer->int_value<0&&isSigned==0) {
        printf("\n\nErr[LINE %d]: Cannot have negative value of type signed int.\n\n",parser->lexer->line);
        exit(1);
    }
    
    return integer;
}
AST_T* parser_parse_string(TypeAndValue* TAV,parser_T* parser) {
    if(parser->current_token->type==TOKEN_SEMI)
        parser_eat(TAV, parser, TOKEN_SEMI);

    //if(!(IsPre)) {
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    strcpy(TAV->ForSetup.Value[TAV->ForSetup.ValueIndex],ast_string->string_value);
    strcpy(TAV->ForPrint.Value[TAV->ForPrint.ValueIndex],ast_string->string_value);
    TAV->ForSetup.ValueIndex++;
    TAV->ForPrint.ValueIndex++;
    parser_eat(TAV,parser, TOKEN_STRING);
    return ast_string;
    //}
    /*else {
        AST_T* ast_string = init_ast(AST_STRING);
        ast_string->string_value = parser->current_token->value;

        strcpy(TAV->ForSetup.Value[TAV->ForSetup.ValueIndex],ast_string->string_value);
        strcpy(TAV->ForPrint.Value[TAV->ForPrint.ValueIndex],ast_string->string_value);
        TAV->ForSetup.ValueIndex++;
        TAV->ForPrint.ValueIndex++;
        parser_eat(TAV,parser, TOKEN_STRING);
        return ast_string;
    }*/

    //return init_ast(AST_NOOP);
}
AST_T* parser_parse_id(parser_T* parser) {
    TypeAndValue* TAV = calloc(1,sizeof(TypeAndValue));
    UpdTAV(TAV);

    
    if(strcmp(parser->current_token->value,"varconst")==0)
        return parser_parse_preVarConstant(parser);
    else if(strcmp(parser->current_token->value,"make")==0){
        return parser_parse_variable_definition(parser);
    }
    else {
        if(!(strcmp(parser->current_token->value,"Print")==0)) {
            return parser_parse_variable(TAV,parser);
        }
        else {
            return parser_parse_prevar(TAV,parser);
        }
    }
}