#ifndef AST_OPTIMIZER_H
#define AST_OPTIMIZER_H

#include "ast.h"
#include "constant_folding.h"

#include "loop_optimization.h"

class ASTOptimizer {
private:
    ConstantFolder constantFolder;
    
    LoopOptimizer loopOptimizer;
    
    bool verbose;
    int passCount;  // 优化轮数
    
public:
    ASTOptimizer(bool verbose = false) 
        : verbose(verbose), passCount(0) {}
    
    // 执行所有优化
    void optimize(CompUnitAST* ast) {
        if (verbose) {
            std::cout << "Starting AST optimization..." << std::endl;
        }
        
        // 多轮优化直到不再变化
        bool changed = true;
        while (changed && passCount < 8) {  // 最多8轮
            passCount++;
            changed = false;
            
            if (verbose) {
                std::cout << "\n=== Optimization Pass " << passCount << " ===" << std::endl;
            }
            
            // 1. 常量折叠
            changed |= constantFolder.fold(ast);
            
           
            // 2. 循环优化
            // changed |= loopOptimizer.optimize(ast);
            
          
        }
        
        if (verbose) {
            std::cout << "\nOptimization completed after " << passCount << " passes" << std::endl;
        }
    }
};

#endif // AST_OPTIMIZER_H