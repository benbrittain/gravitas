#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <inttypes.h>

#include "ast.h"
#include "parser.h"
#include "codegen.h"

#define BUFF_SIZE 4096
char buffer[BUFF_SIZE];

int main(int argc, char **argv) {

    // Make an LLVM Module
    LLVMModuleRef module = LLVMModuleCreateWithName("gravitas");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMExecutionEngineRef engine;

    char *error = NULL;
    LLVMLinkInJIT();
    LLVMInitializeNativeTarget();
    if (LLVMCreateExecutionEngineForModule(&engine, module, &error) != 0) {
        fprintf(stderr, "Failed to create execution engine: %s\n", error);
        LLVMDisposeMessage(error);
        return 1;
    }

    if (error) {
        fprintf(stderr, "Error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, BUFF_SIZE, stdin) != NULL) {
        ast_node *node = NULL;

        int rc = parse(buffer, &node);
        if (rc != 0) {
             fprintf(stderr, "Parse error\n");
             return 1;
        }

        LLVMValueRef value = codegen(node, module, builder);
        if (value == NULL) {
            fprintf(stderr, "Codegen error\n");
            return 1;
        }

        if (LLVMWriteBitcodeToFile(module, "out.bc") != 0) {
            fprintf(stderr, "Couldn't write out bitcode");
        }

        ast_node_free(node);
    }

    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    return 0;
}
