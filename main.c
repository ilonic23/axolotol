#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void) {
    char *string = "let hi = \"Hello, World!\";\n"
                   "println(hi);\n";

    printf("%s\n", string);

    parser_t *parser = init_parser(init_lexer(string));

    ast_t *root = parser_parse(parser);

    printf("AST(%d)\n", root->type);
    printf("AST_Compound_Size(%d)", root->compound_size);

    return 0;
}
