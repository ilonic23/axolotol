#include "ast.h"
#include "stdlib.h"

ast_t *init_ast(int type)
{
    ast_t *ast = malloc(sizeof(ast_t));
    ast->type = type;

    // AST_VAR_DEFINITION
    ast->variable_def_var_name = NULL;
    ast->variable_def_value = NULL;

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
