#ifndef AST_H
#define AST_H
#include "stddef.h"

typedef struct Ast
{
    enum
    {
        AST_VAR_DEFINITION,
        AST_VAR,
        AST_FUNC_CALL,
        AST_STRING_LITERAL,
        AST_COMPOUND,
        AST_NOP
    } type;

    // AST_VAR_DEFINITION
    char *variable_def_var_name;
    struct Ast *variable_def_value;

    // AST_VAR
    char *variable_name;

    // AST_FUNC_CALL
    char *function_call_name;
    struct Ast **function_call_args;
    size_t function_call_args_len;

    // AST_STRING_LITERAL
    char *string_value;

    // AST_COMPOUND
    struct Ast **compound_value;
    size_t compound_size;
} ast_t;

ast_t *init_ast(int type);

#endif