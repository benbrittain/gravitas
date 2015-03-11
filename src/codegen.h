#ifndef _codegen_h
#define _codegen_h

#include <llvm-c/Core.h>
#include "ast.h"
#include "uthash.h"

LLVMValueRef codegen(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder);

typedef struct named_value {
    const char *name;
    LLVMValueRef value;
    UT_hash_handle hh;
} named_value;

named_value *codegen_named_value(const char *name);

#endif
