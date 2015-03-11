#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "ast.h"
#include "codegen.h"


// TODO: helper file?
named_value *named_values = NULL;

char* makeFunctionName(const char *prototype_name) {
    static int i = 0;
    char new_name[16];
    if (strlen(prototype_name) == 0) {
        sprintf(new_name, "anon_func_%d", i++);
    }
    // TODO: not anon funcs
    return strdup(new_name);
}

LLVMValueRef codegen_number(ast_node *node) {
    return LLVMConstReal(LLVMDoubleType(), node->number.value);
}

LLVMValueRef codegen_binary_expr(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    // Only has + right now :D
    LLVMValueRef lhs = codegen(node->binary_expr.lhs, module, builder);
    LLVMValueRef rhs = codegen(node->binary_expr.rhs, module, builder);

    return LLVMBuildFAdd(builder, lhs, rhs, "addtmp");
}

LLVMValueRef codegen_prototype(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    //fprintf(stdout, "function name length: %d", strlen(node->prototype.name));
    //fprintf(stdout, "function name: %s", node->prototype.name);
    char* func_name = makeFunctionName(node->prototype.name);
    fprintf(stdout, "function name: %s\n", func_name);

    int i;
    int arg_count = node->prototype.arg_count;

    LLVMValueRef func = LLVMGetNamedFunction(module, func_name);
    if (func != NULL) {
        if (LLVMCountParams(func) != arg_count) {
            fprintf(stderr, "Function already exists with different number of parameters");
            return NULL;
        }
        if(LLVMCountBasicBlocks(func) != 0) {
            fprintf(stderr, "Function already has a defined body");
            return NULL;
        }
    } else {
        LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * arg_count);
        for (i=0; i<arg_count; i++) {
            params[i] = LLVMDoubleType();
        }
        // TODO: change this. things are more than doubles, silly kaleidescope
        LLVMTypeRef funcType = LLVMFunctionType(LLVMDoubleType(), params, arg_count, 0);
        func = LLVMAddFunction(module, node->prototype.name, funcType);
        LLVMSetLinkage(func, LLVMExternalLinkage);
    }

    for (i=0; i<arg_count; i++) {
        named_value *val = malloc(sizeof(named_value));
        val->name = strdup(node->prototype.args[i]);
        val->value = LLVMGetParam(func, i);
        HASH_ADD_KEYPTR(hh, named_values, val->name, strlen(val->name), val);
    }

    return func;
}

LLVMValueRef codegen_function(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {

    HASH_CLEAR(hh, named_values);

    LLVMValueRef func = codegen(node->function.prototype, module, builder);
    if (func == NULL) {
        return NULL;
    }

    LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");
    LLVMPositionBuilderAtEnd(builder, block);
    LLVMValueRef body = codegen(node->function.body, module, builder);

    if (body == NULL) {
        LLVMDeleteFunction(func);
        return NULL;
    }

    LLVMBuildRet(builder, body);

    return func;
}

LLVMValueRef codegen(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    switch(node->type) {
        case AST_TYPE_FUNCTION:
            fprintf(stdout, "function\n");
            return codegen_function(node, module, builder);
            break;
        case AST_TYPE_PROTOTYPE:
            fprintf(stdout, "prototype\n");
            return codegen_prototype(node, module, builder);
            break;
        case AST_TYPE_NUMBER:
            return codegen_number(node);
            break;
        case AST_TYPE_BINARY_EXPR:
            return codegen_binary_expr(node, module, builder);
            break;
    }

    return NULL;
}

