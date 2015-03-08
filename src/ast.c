#include <stdlib.h>
#include <string.h>
#include "ast.h"

ast_node *ast_number_create(double value) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_NUMBER;
    node->number.value = value;
    return node;
}

ast_node *ast_variable_create(char *name) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = AST_TYPE_VARIABLE;
    node->variable.name = strdup(name);
    return node;
}


void ast_node_free(ast_node *node)
{
    int i;
    if(!node) return;

}
