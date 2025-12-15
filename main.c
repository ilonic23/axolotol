#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "visitor.h"

int main(void) {
    char *string = "let hi = \"Hello, \";\n"
                   "let hi2 = \"World!\";\n"
                   "print(hi);\n"
                   "println(hi2);\n";

    printf("%s\n", string);

    parser_t *parser = init_parser(init_lexer(string));

    ast_t *root = parser_parse(parser);

    visitor_t *visitor = init_visitor();
    visitor_visit(visitor, root);
    return 0;
}
