#include "ast.h"

#include <string.h>

#include "stdlib.h"

ast_t *init_ast(int type)
{
    ast_t *ast = malloc(sizeof(ast_t));
    ast->type = type;
    ast->scope = NULL;

    // AST_VAR_DEFINITION
    ast->variable_def_var_name = NULL;
    ast->variable_def_value = NULL;

    // AST_FUNC_DEFINITION
    ast->func_def_body = NULL;
    ast->function_def_name = NULL;

    // AST_VAR
    ast->variable_name = NULL;

    // AST_FUNC_CALL
    ast->function_call_name = NULL;
    ast->function_call_args = NULL;
    ast->function_call_args_len = 0;

    // AST_STRING_LITERAL
    ast->string_value = NULL;

    // AST_COMPOUND
    ast->compound_value = NULL;
    ast->compound_size = 0;
    return ast;
}

scope_t *init_scope() {
    scope_t *scope = malloc(sizeof(scope_t));
    scope->function_defs = NULL;
    scope->function_defs_size = 0;
    return scope;
}

ast_t *scope_add_funcdef(scope_t *scope, ast_t *fdef) {
    scope->function_defs_size += 1;

    if (!scope->function_defs)
        scope->function_defs = malloc(sizeof(ast_t *));
    else
    {
        scope->function_defs = realloc(scope->function_defs,
            scope->function_defs_size * sizeof(ast_t **));
    }

    scope->function_defs[scope->function_defs_size - 1] = fdef;

    return fdef;
}

ast_t *scope_get_funcdef(scope_t *scope, const char *name) {
    for (int i = 0; i < scope->function_defs_size; i++)
    {
        ast_t *fdef = scope->function_defs[i];

        if (strcmp(fdef->function_def_name, name) == 0)
        {
            return fdef;
        }
    }

    return NULL;
}
