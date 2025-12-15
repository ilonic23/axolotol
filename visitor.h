#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

typedef struct Visitor
{
    ast_t **vardefs;
    size_t vardefs_len;
} visitor_t;

visitor_t *init_visitor();

ast_t *visitor_visit(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_vardef(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_var(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_func_call(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_string(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_compound(visitor_t *visitor, ast_t *node);

#endif