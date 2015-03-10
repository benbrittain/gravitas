#ifndef _codegen_h
#define _codegen_h

#include <llvm-c/Core.h>
#include "ast.h"

LLVMValueRef codegen(ast_node *node, LLVMModuleRef module, LLVMBuilderRef builder);

#endif
