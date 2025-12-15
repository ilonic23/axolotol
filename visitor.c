#include "visitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ast_t *builtin_println(visitor_t *visitor, ast_t **args, int args_len) {
    for (int i = 0; i < args_len; i++)
    {
        ast_t *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING_LITERAL:
                printf("%s\n", visited_ast->string_value); break;
            default:
            printf("%p\n", visited_ast);
                break;
        }
    }

    return init_ast(AST_NOP);
}

static ast_t *builtin_print(visitor_t *visitor, ast_t **args, int args_len) {
    for (int i = 0; i < args_len; i++)
    {
        ast_t *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRING_LITERAL:
            printf("%s", visited_ast->string_value); break;
        default:
            printf("%p\n", visited_ast);
            break;
        }
    }

    return init_ast(AST_NOP);
}

visitor_t *init_visitor()
{
    visitor_t *visitor = malloc(sizeof(visitor_t));
    visitor->vardefs = NULL;
    visitor->vardefs_len = 0;

    return visitor;
}

ast_t *visitor_visit(visitor_t *visitor, ast_t *node) {
    switch (node->type)
    {
        case AST_VAR_DEFINITION:
            return visitor_visit_vardef(visitor, node);
        case AST_VAR:
            return visitor_visit_var(visitor, node);
        case AST_FUNC_CALL:
            return visitor_visit_func_call(visitor, node);
        case AST_STRING_LITERAL:
            return visitor_visit_string(visitor, node);
        case AST_COMPOUND:
            return visitor_visit_compound(visitor, node);
        case AST_NOP:
            return node;
    }

    printf("Unknown statement of type '%d'\n", node->type);
    exit(1);
}

ast_t *visitor_visit_vardef(visitor_t *visitor, ast_t *node) {

    if (!visitor->vardefs)
    {
        visitor->vardefs = malloc(sizeof(ast_t));
        visitor->vardefs[0] = node;
        visitor->vardefs_len += 1;
    }
    else
    {
        visitor->vardefs_len += 1;
        visitor->vardefs = realloc(visitor->vardefs,
            visitor->vardefs_len * sizeof(ast_t *));
        visitor->vardefs[visitor->vardefs_len - 1] = node;
    }

    return node;
}
ast_t *visitor_visit_var(visitor_t *visitor, ast_t *node) {
    for (int i = 0; i < visitor->vardefs_len; i++)
    {
        ast_t *vardef = visitor->vardefs[i];

        if (strcmp(vardef->variable_def_var_name,
            node->variable_name) == 0)
        {
            return visitor_visit(visitor, vardef->variable_def_value);
        }
    }

    printf("Undeclared variable '%s'\n",
        node->variable_name);
    return node;
}
ast_t *visitor_visit_func_call(visitor_t *visitor, ast_t *node) {
    if (strcmp(node->function_call_name, "println") == 0)
    {
        return builtin_println(visitor, node->function_call_args, node->function_call_args_len);
    }
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_print(visitor, node->function_call_args, node->function_call_args_len);
    }

    printf("Unknown function '%s'\n", node->function_call_name);
    exit(1);
}
ast_t *visitor_visit_string(visitor_t *visitor, ast_t *node) {
    return node;
}
ast_t *visitor_visit_compound(visitor_t *visitor, ast_t *node) {
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOP);
}