#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"



LLVMValueRef codegen_number(ast_node *node) {
    return LLVMConstReal(LLVMDoubleType(), node->number.value);
}

LLVMValueRef codegen(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    switch(node->type) {
        case AST_TYPE_NUMBER:
            return codegen_number(node);
            break;
        case AST_TYPE_BINARY_EXPR:
            break;
    }

    return NULL;
}
