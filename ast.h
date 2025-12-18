#ifndef AST_H
#define AST_H
#include "stddef.h"

typedef struct Ast
{
    enum
    {
        AST_VAR_DEFINITION,
        AST_FUNC_DEFINITION,
        AST_VAR,
        AST_FUNC_CALL,
        AST_STRING_LITERAL,
        AST_COMPOUND,
        AST_END,
        AST_NOP
    } type;

    struct Scope *scope;

    // AST_VAR_DEFINITION
    char *variable_def_var_name;
    struct Ast *variable_def_value;

    // AST_FUNC_DEFINITION
    struct Ast *func_def_body;
    char *function_def_name;

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

typedef struct Scope
{
    ast_t **function_defs;
    size_t function_defs_size;

    ast_t **vardefs;
    size_t vardefs_len;
} scope_t;

scope_t *init_scope();
ast_t *scope_add_funcdef(scope_t *scope, ast_t *fdef);
ast_t *scope_get_funcdef(scope_t *scope, const char *name);

ast_t *scope_add_vardef(scope_t *scope, ast_t *vdef);
ast_t *scope_get_vardef(scope_t *scope, const char *name);

#endif