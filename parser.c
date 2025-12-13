#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

parser_t *init_parser(lexer_t *lexer)
{
    parser_t *parser = malloc(sizeof(parser_t));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(parser->lexer);

    return parser;
}
void *parser_eat(parser_t *parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else {
        printf("Unexpected token '%s' with type '%d'", parser->current_token->value, parser->current_token->type);
        exit(1);
    }
}
ast_t *parser_parse(parser_t *parser)
{
    return parser_parse_statements(parser);
}
ast_t *parser_parse_statement(parser_t *parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
    }
}
ast_t *parser_parse_statements(parser_t *parser)
{
    ast_t *compound = init_ast(AST_COMPOUND);
    compound->compound_value = malloc(sizeof(ast_t *));

    ast_t *ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;

    while (parser->current_token->type == TOKEN_SEMICOLON)
    {
        parser_eat(parser, TOKEN_SEMICOLON);

        ast_t *ast_statement = parser_parse_statement(parser);
        compound->compound_size += 1;
        compound->compound_value = realloc(
            compound->compound_value,
            compound->compound_size * sizeof(ast_t *)
            );
        compound->compound_value[compound->compound_size - 1] = ast_statement;
    }

    return compound;
}

// TODO: Make initialization unnecessary
ast_t *parser_parse_variable_definition(parser_t *parser)
{
    parser_eat(parser, TOKEN_ID); // let
    char *variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // Var name
    parser_eat(parser, TOKEN_ASSIGN);
    ast_t *value = parser_parse_expression(parser);

    ast_t *variable_definition = init_ast(AST_VAR_DEFINITION);
    variable_definition->variable_name = variable_name;
    variable_definition->variable_def_value = value;

    return variable_definition;
}

ast_t *parser_parse_variable(parser_t *parser)
{
    char *token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // var name or func call name

    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_func_call(parser);

    ast_t *ast_variable = init_ast(AST_VAR);
    ast_variable->variable_name = token_value;

    return ast_variable;
}

ast_t *parser_parse_id(parser_t *parser)
{
    if (strcmp(parser->current_token->value, "let") == 0)
        return parser_parse_variable_definition(parser);

    return parser_parse_variable(parser);
}

ast_t *parser_parse_expression(parser_t *parser)
{
    switch (parser->current_token->type)
    {
    case TOKEN_STRING:
        return parser_parse_string(parser);
    }
}
ast_t *parser_parse_factor(parser_t *parser);
ast_t *parser_parse_term(parser_t *parser);
ast_t *parser_parse_func_call(parser_t *parser)
{
    printf("Fake Parse FuncCall\n");
    return NULL;
}

ast_t *parser_parse_string(parser_t *parser)
{
    ast_t *ast_string = init_ast(AST_STRING_LITERAL);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);
    return ast_string;
}