#include "riscv_backend.h"
#include <iostream>
#include <llvm/IR/Verifier.h>

bool RISCVBackend::initializeTarget() {
    // 初始化 RISC-V 目标
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    return true;
}

RISCVBackend::RISCVBackend(int optLevel) : optLevel(optLevel) {
    // 设置目标三元组为 RISC-V 64
    std::string targetTriple = "riscv64-unknown-linux-gnu";
    
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    
    if (!target) {
        std::cerr << "Error: " << error << std::endl;
        targetMachine = nullptr;
        return;
    }
    
    // 设置目标选项
    llvm::TargetOptions opt;
    //opt.AllowFPOpFusion = llvm::FPOpFusion::Fast;  // 允许浮点操作融合
    //opt.UnsafeFPMath = true;  // 允许不安全浮点优化
    //opt.NoNaNsFPMath = true;  // 禁用NaN浮点运算
    //opt.NoInfsFPMath = true;  // 禁用无穷大浮点运算
    //opt.EnableIPRA = true;
    //opt.EnableFastISel = true;

    auto RM = std::optional<llvm::Reloc::Model>(llvm::Reloc::PIC_);    

    llvm::CodeGenOpt::Level codeGenOptLevel;
    switch (optLevel) {
        case 0: codeGenOptLevel = llvm::CodeGenOpt::None; break;
        case 1: codeGenOptLevel = llvm::CodeGenOpt::Less; break;
        case 2: codeGenOptLevel = llvm::CodeGenOpt::Default; break;
        case 3: codeGenOptLevel = llvm::CodeGenOpt::Aggressive; break;
        default: codeGenOptLevel = llvm::CodeGenOpt::Default; break;
    }

    llvm::CodeModel::Model codeModel = llvm::CodeModel::Small;
    
    targetMachine = target->createTargetMachine(
        targetTriple,
        "generic-rv64",  // CPU
        "+m,+a,+f,+d,+c,+v",  // 特性：M/A/F/D/C + 向量扩展
        opt,
        RM,
        codeModel,
        codeGenOptLevel
    );
    
    if (!targetMachine) {
        std::cerr << "Error: Could not create target machine" << std::endl;
    }
}

RISCVBackend::~RISCVBackend() {
    if (targetMachine) {
        delete targetMachine;
    }
}


bool RISCVBackend::generateAssembly(llvm::Module* module, const std::string& outputFile) {
    if (!targetMachine) {
        std::cerr << "Error: Target machine not initialized" << std::endl;
        return false;
    }
    
    // 设置模块的目标信息
    module->setDataLayout(targetMachine->createDataLayout());
    module->setTargetTriple("riscv64-unknown-linux-gnu");
    
    
    // 验证模块，确保 IR 有效
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyModule(*module, &errorStream)) {
        errorStream.flush();
        std::cerr << "Module verification failed: " << errorMsg << std::endl;
        return false;
    }
    
    
    
    // 优化后再次验证模块
    if (llvm::verifyModule(*module, &errorStream)) {
        errorStream.flush();
        std::cerr << "Module verification failed after optimization: " << errorMsg << std::endl;
        return false;
    }
    
    // 打开输出文件
    std::error_code ec;
    llvm::raw_fd_ostream dest(outputFile, ec, llvm::sys::fs::OF_None);
    
    if (ec) {
        std::cerr << "Could not open file: " << ec.message() << std::endl;
        return false;
    }
    
    
    // 使用 Legacy Pass Manager 仅用于代码生成，因为它仍然是生成汇编代码的可靠方式
    llvm::legacy::PassManager pass;
    
    // 添加代码生成 Pass
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_AssemblyFile)) {
        std::cerr << "TargetMachine can't emit a file of this type" << std::endl;
        return false;
    }
    
    // 运行 Pass
    pass.run(*module);
    dest.flush();
    
    return true;
}

bool RISCVBackend::generateObject(llvm::Module* module, const std::string& outputFile) {
    if (!targetMachine) {
        std::cerr << "Error: Target machine not initialized" << std::endl;
        return false;
    }
    
    // 设置模块的目标信息
    module->setDataLayout(targetMachine->createDataLayout());
    module->setTargetTriple("riscv64-unknown-linux-gnu");
    
    // 验证模块，确保 IR 有效
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyModule(*module, &errorStream)) {
        errorStream.flush();
        std::cerr << "Module verification failed: " << errorMsg << std::endl;
        return false;
    }
    
    
    
    // 优化后再次验证模块
    if (llvm::verifyModule(*module, &errorStream)) {
        errorStream.flush();
        std::cerr << "Module verification failed after optimization: " << errorMsg << std::endl;
        return false;
    }
    
    // 打开输出文件
    std::error_code ec;
    llvm::raw_fd_ostream dest(outputFile, ec, llvm::sys::fs::OF_None);
    
    if (ec) {
        std::cerr << "Could not open file: " << ec.message() << std::endl;
        return false;
    }
    
    // 创建 Pass Manager
    llvm::legacy::PassManager pass;
    
    // 修改：LLVM 17 中使用 CGFT_ObjectFile
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr,
                                           llvm::CGFT_ObjectFile)) {
        std::cerr << "TargetMachine can't emit a file of this type" << std::endl;
        return false;
    }
    
    // 运行 pass
    pass.run(*module);
    dest.flush();
    
    return true;
}
