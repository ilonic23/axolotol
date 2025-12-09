#include "token.h"
#include "stdlib.h"

token_t *init_token(int type, char *value) {
    token_t *token = malloc(sizeof (struct Token));
    token->type = type;
    token->value = value;

    return token;
}