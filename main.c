#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "visitor.h"

int main(void) {
    FILE *example_file = fopen("example.axo", "r");
    if (!example_file)
    {
        perror("Can't open example.axo");
        return 1;
    }

    fseek(example_file, 0, SEEK_END);
    uint64_t size = ftell(example_file);
    rewind(example_file);

    char *string = malloc(size + 1);
    if (!string)
    {
        printf("Memory allocation failed for file buffer.\n");
        return 1;
    }

    fread(string, 1, size, example_file);
    string[size] = 0;

    printf("%s\n\n", string);

    parser_t *parser = init_parser(init_lexer(string));

    ast_t *root = parser_parse(parser);

    visitor_t *visitor = init_visitor();
    visitor_visit(visitor, root);
    return 0;
}
