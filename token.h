#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token {
    enum
    {
        TOKEN_ID,
        TOKEN_ASSIGN,
        TOKEN_STRING,
        TOKEN_SEMICOLON,
        TOKEN_LPAREN,
        TOKEN_RPAREN
    } type;

    char *value;
} token_t;

token_t *init_token(int type, char *value);
#endif
