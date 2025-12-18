#include "visitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

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
    // visitor->vardefs = NULL;
    // visitor->vardefs_len = 0;

    return visitor;
}

ast_t *visitor_visit(visitor_t *visitor, ast_t *node) {
    switch (node->type)
    {
        case AST_VAR_DEFINITION:
            return visitor_visit_vardef(visitor, node);
        case AST_FUNC_DEFINITION:
            return visitor_visit_func_def(visitor, node);
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
    scope_add_vardef(node->scope, node);
    return node;
}

ast_t *visitor_visit_func_def(visitor_t *visitor, ast_t *node) {
    scope_add_funcdef(node->scope, node);
    return node;
}

ast_t *visitor_visit_var(visitor_t *visitor, ast_t *node) {
    ast_t *vdef = scope_get_vardef(node->scope, node->variable_name);

    if (vdef)
    {
        return visitor_visit(visitor, vdef->variable_def_value);
    }

    printf("Undeclared variable '%s'\n",
        node->variable_name);
    exit(1);
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
    ast_t *fdef = scope_get_funcdef(node->scope, node->function_call_name);

    if (fdef)
    {
        return visitor_visit(visitor, fdef->func_def_body);
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