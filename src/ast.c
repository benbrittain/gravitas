#include <stdlib.h>
#include <string.h>
#include "ast.h"

ast_node *ast_number_create(double value) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_NUMBER;
    node->number.value = value;
    return node;
}

ast_node *ast_prototype_create(char *name, char **args, int arg_count) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_PROTOTYPE;
    node->prototype.name = strdup(name);
    node->prototype.args = malloc(sizeof(char*) * arg_count);
    int i;
    for(i=0; i<arg_count; i++) {
        node->prototype.args[i] = strdup(args[i]);
    }
    node->prototype.arg_count = arg_count;
    return node;
}

ast_node *ast_function_create(ast_node *proto, ast_node *body) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_FUNCTION;
    node->function.prototype = proto;
    node->function.body = body;
    return node;
}

ast_node *ast_variable_create(char *name) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_VARIABLE;
    node->variable.name = strdup(name);
    return node;
}

ast_node *ast_binary_expr_create(char op, ast_node *lhs, ast_node *rhs) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_BINARY_EXPR;
    node->binary_expr.op = op;
    node->binary_expr.lhs = lhs;
    node->binary_expr.rhs = rhs;
    return node;
}

ast_node *ast_method_call_create(char *name, ast_node **args, int count) {
    return NULL;
}



void ast_node_free(ast_node *node)
{
    int i;
    if(!node) return;
}
