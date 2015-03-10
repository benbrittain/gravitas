#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"
#include "codegen.h"
#include <llvm-c/Core.h>

#define BUFF_SIZE 1024
char buffer[BUFF_SIZE];

int main(int argc, char **argv) {
    while (fgets(buffer, BUFF_SIZE, stdin) != NULL) {
        ast_node *node = NULL;
        int rc = parse(buffer, &node);
        printf("root node: %p\n", node);
        LLVMValueRef value = codegen(node, NULL, NULL);
        LLVMTypeRef type = LLVMTypeOf(value);
    }
    return 0;
}
