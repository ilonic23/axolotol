#include "lexer.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

lexer_t *init_lexer(char *contents){
    lexer_t *lexer = malloc(sizeof (struct Lexer));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->current = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_t *lexer){
    if (lexer->current != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->current = lexer->contents[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_t *lexer){
    while(lexer->current == ' ' || lexer->current == '\n')
        lexer_advance(lexer);
}

token_t *lexer_get_next_token(lexer_t *lexer){
    while (lexer->current != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer_skip_whitespace(lexer);

        if (lexer->current == '"')
            return lexer_collect_string(lexer);
        if (isalnum(lexer->current))
            return lexer_collect_id(lexer);

        switch (lexer->current) {
            case '=':
                return lexer_advance_with_token(lexer, init_token(TOKEN_ASSIGN, lexer_get_current_char_as_string(lexer)));
            case ';':
                return lexer_advance_with_token(lexer, init_token(TOKEN_SEMICOLON, lexer_get_current_char_as_string(lexer)));
            case '(':
                return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            case ')':
                return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            case ',':
                return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
        }
    }

    return init_token(TOKEN_EOF, "\0");
}

token_t *lexer_collect_string(lexer_t *lexer){
    lexer_advance(lexer); // Skip '"' symbol

    char *value = malloc(sizeof(char));
    value[0] = '\0';

    while (lexer->current != '"') {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer); // Skip '"' symbol

    return init_token(TOKEN_STRING, value);
}

token_t *lexer_collect_id(lexer_t *lexer) {
    char *value = malloc(sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->current)) {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value);
}

token_t *lexer_advance_with_token(lexer_t *lexer, token_t *token) {
    lexer_advance(lexer);
    return token;
}

char *lexer_get_current_char_as_string(lexer_t *lexer){
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->current;
    str[1] = '\0';

    return str;
}