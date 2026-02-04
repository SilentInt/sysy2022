#ifndef LOOP_OPTIMIZATION_H
#define LOOP_OPTIMIZATION_H

#include "ast.h"

class LoopOptimizer {
public:
    bool optimize(CompUnitAST* ast) {
        bool changed = false;
        
        // 遍历所有函数，对每个函数进行循环优化
        for (auto& func : ast->getFunctions()) {
            changed |= optimizeInFunction(func.get());
        }
        
        return changed;
    }
    
private:
    bool optimizeInFunction(FunctionAST* func) {
        // 在函数体中查找并优化循环
        return optimizeInBlock(func->getBody());
    }
    
    bool optimizeInBlock(BlockAST* block) {
        bool changed = false;
        
        // 遍历块中的所有语句，查找WhileStmtAST
        for (auto& item : block->getItems()) {
            if (auto stmtItem = dynamic_cast<StmtBlockItemAST*>(item.get())) {
                auto stmt = stmtItem->getStmt();
                
                // 递归处理嵌套块
                if (auto nestedBlock = dynamic_cast<BlockAST*>(stmt)) {
                    changed |= optimizeInBlock(nestedBlock);
                }
                // 处理if语句的分支
                else if (auto ifStmt = dynamic_cast<IfStmtAST*>(stmt)) {
                    if (auto thenBlock = dynamic_cast<BlockAST*>(ifStmt->getThenStmt())) {
                        changed |= optimizeInBlock(thenBlock);
                    }
                    if (auto elseStmt = ifStmt->getElseStmt()) {
                        if (auto elseBlock = dynamic_cast<BlockAST*>(elseStmt)) {
                            changed |= optimizeInBlock(elseBlock);
                        }
                    }
                }
                // 处理循环
                else if (auto whileStmt = dynamic_cast<WhileStmtAST*>(stmt)) {
                    changed |= optimizeLoop(whileStmt);
                }
            }
        }
        
        return changed;
    }
    
    bool optimizeLoop(WhileStmtAST* loop) {
        
        
        return false;
    }
};

#endif // LOOP_OPTIMIZATION_H