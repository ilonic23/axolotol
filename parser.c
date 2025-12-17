#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static scope_t *get_node_scope(parser_t *parser, ast_t *node) {
    return !node->scope? parser->scope : node->scope;
}

parser_t *init_parser(lexer_t *lexer)
{
    parser_t *parser = malloc(sizeof(parser_t));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(parser->lexer);
    parser->prev_token = parser->current_token;
    parser->scope = init_scope();

    return parser;
}
void *parser_eat(parser_t *parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else {
        printf("Unexpected token '%s' with type '%d', expected '%d'", parser->current_token->value,
            parser->current_token->type, token_type);
        exit(1);
    }
}
ast_t *parser_parse(parser_t *parser, scope_t *scope)
{
    return parser_parse_statements(parser, scope);
}
ast_t *parser_parse_statement(parser_t *parser, scope_t *scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope);
    }

    return init_ast(AST_NOP);
}
ast_t *parser_parse_statements(parser_t *parser, scope_t *scope)
{
    ast_t *compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = malloc(sizeof(ast_t *));

    ast_t *ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMICOLON)
    {
        parser_eat(parser, TOKEN_SEMICOLON);

        ast_t *ast_statement = parser_parse_statement(parser, scope);
        if (ast_statement->type == AST_END)
        {
            parser_eat(parser, TOKEN_ID); // end keyword
            break;
        }

        if (ast_statement)
        {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(ast_t *)
                );
            compound->compound_value[compound->compound_size - 1] = ast_statement;
        }
    }

    return compound;
}

// TODO: Make initialization unnecessary
ast_t *parser_parse_variable_definition(parser_t *parser, scope_t *scope)
{
    parser_eat(parser, TOKEN_ID); // let
    char *variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // Var name
    parser_eat(parser, TOKEN_ASSIGN);
    ast_t *value = parser_parse_expression(parser, scope);

    ast_t *variable_definition = init_ast(AST_VAR_DEFINITION);
    variable_definition->variable_def_var_name = variable_name;
    variable_definition->variable_def_value = value;

    variable_definition->scope = scope;

    return variable_definition;
}

ast_t *parser_parse_variable(parser_t *parser, scope_t *scope)
{
    char *token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // var name or func call name

    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_func_call(parser, scope);

    ast_t *ast_variable = init_ast(AST_VAR);
    ast_variable->variable_name = token_value;

    ast_variable->scope = scope;

    return ast_variable;
}

ast_t *parser_parse_id(parser_t *parser, scope_t *scope)
{
    if (strcmp(parser->current_token->value, "let") == 0)
        return parser_parse_variable_definition(parser, scope);
    if (strcmp(parser->current_token->value, "func") == 0)
        return parser_parse_func_definition(parser, scope);
    if (strcmp(parser->current_token->value, "end") == 0)
        return init_ast(AST_END);

    return parser_parse_variable(parser, scope);
}

ast_t *parser_parse_expression(parser_t *parser, scope_t *scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING:
            return parser_parse_string(parser, scope);
        case TOKEN_ID:
            return parser_parse_id(parser, scope);
    }

    return init_ast(AST_NOP);
}
ast_t *parser_parse_factor(parser_t *parser, scope_t *scope);
ast_t *parser_parse_term(parser_t *parser, scope_t *scope);

ast_t *parser_parse_func_definition(parser_t *parser, scope_t *scope) {
    ast_t *func_def = init_ast(AST_FUNC_DEFINITION);
    parser_eat(parser, TOKEN_ID); // func keyword

    char *func_name = parser->current_token->value;
    func_def->function_def_name = malloc(strlen(func_name) + 1);
    strcpy(func_def->function_def_name, func_name);
    parser_eat(parser, TOKEN_ID); // func name
    parser_eat(parser, TOKEN_LPAREN);
    // TODO: Implement func arguments
    parser_eat(parser, TOKEN_RPAREN);
    func_def->func_def_body = parser_parse_statements(parser, scope);

    func_def->scope = scope;

    return func_def;
}
ast_t *parser_parse_func_call(parser_t *parser, scope_t *scope) {
    ast_t *func_call = init_ast(AST_FUNC_CALL);

    func_call->function_call_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_LPAREN);

    func_call->function_call_args = malloc(sizeof(ast_t *));

    ast_t *ast_expr = parser_parse_expression(parser, scope);
    func_call->function_call_args[0] = ast_expr;
    func_call->function_call_args_len += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        ast_t *ast_expr = parser_parse_expression(parser, scope);
        func_call->function_call_args_len += 1;
        func_call->function_call_args = realloc(
            func_call->function_call_args,
            func_call->function_call_args_len * sizeof(ast_t *)
            );
        func_call->function_call_args[func_call->function_call_args_len - 1] = ast_expr;
    }

    parser_eat(parser, TOKEN_RPAREN);

    func_call->scope = scope;

    return func_call;
}

ast_t *parser_parse_string(parser_t *parser, scope_t *scope) {
    ast_t *ast_string = init_ast(AST_STRING_LITERAL);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}