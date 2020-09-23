//
// Created by 金韬 on 2020/9/22.
//

#ifndef LLVM_KALEIDOSCOPE_ERR_HELPER_H
#define LLVM_KALEIDOSCOPE_ERR_HELPER_H

#include <memory>
#include "ExprAST.h"

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str);

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

llvm::Value *LogErrorV(const char *Str);

#endif //LLVM_KALEIDOSCOPE_ERR_HELPER_H