#include <stdio.h>
#include "lexer.h"

int main(void) {
    char *string = "func main() void\n"
                   "println(\"Hello, World!\");\n"
                   "end\n";

    printf("%s\n", string);

    lexer_t *lexer = init_lexer(string);

    token_t *token = NULL;

    while ((token = lexer_get_next_token(lexer)) != NULL) {
        printf("TOKEN(%d, '%s')\n", token->type, token->value);
    }

    return 0;
}
