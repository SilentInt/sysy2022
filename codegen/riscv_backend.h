#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>  // 修改：使用新的头文件
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/CodeGen.h>
#include <string>

class RISCVBackend {
private:
    llvm::TargetMachine* targetMachine;
    int optLevel;
    
    // 优化 LLVM IR
    void optimizeModule(llvm::Module* module);
    
public:
    explicit RISCVBackend(int optLevel = 0);
    ~RISCVBackend();
    
    // 生成汇编代码
    bool generateAssembly(llvm::Module* module, const std::string& outputFile);
    
    // 生成目标文件
    bool generateObject(llvm::Module* module, const std::string& outputFile);
    
    // 初始化目标
    static bool initializeTarget();
};
