#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "ast.h"
#include "codegen.h"

LLVMValueRef codegen_number(ast_node *node) {
    return LLVMConstReal(LLVMDoubleType(), node->number.value);
}

LLVMValueRef codegen_binary_expr(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    // Only has + right now :D
    LLVMValueRef lhs = codegen(node->binary_expr.lhs, module, builder);
    LLVMValueRef rhs = codegen(node->binary_expr.rhs, module, builder);

    return LLVMBuildFAdd(builder, lhs, rhs, "addtmp");
}

LLVMValueRef codegen(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    switch(node->type) {
        case AST_TYPE_NUMBER:
            return codegen_number(node);
            break;
        case AST_TYPE_BINARY_EXPR:
            return codegen_binary_expr(node, module, builder);
            break;
    }

    return NULL;
}

