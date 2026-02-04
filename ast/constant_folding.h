#ifndef CONSTANT_FOLDING_H
#define CONSTANT_FOLDING_H

#include "ast.h"

class ConstantFolder {
public:
    bool fold(CompUnitAST* ast) {
        bool changed = false;
        
        // 处理全局声明
        for (auto& decl : ast->getDecls()) {
            if (auto declItem = dynamic_cast<DeclAST*>(decl.get())) {
                changed |= foldDecl(declItem);
            }
        }
        
        // 遍历所有函数
        for (auto& func : ast->getFunctions()) {
            changed |= foldFunction(func.get());
        }
        
        return changed;
    }
    
private:
    bool foldFunction(FunctionAST* func) {
        return foldBlock(func->getBody());
    }
    
    bool foldBlock(BlockAST* block) {
        bool changed = false;
        
        for (auto& item : block->getItems()) {
            if (auto stmtItem = dynamic_cast<StmtBlockItemAST*>(item.get())) {
                changed |= foldStmt(stmtItem->getStmt());
            }
            else if (auto declItem = dynamic_cast<DeclBlockItemAST*>(item.get())) {
                changed |= foldDecl(declItem->getDecl());
            }
        }
        
        return changed;
    }
    
    bool foldDecl(DeclAST* decl) {
        bool changed = false;
        
        // 处理变量声明
        if (auto varDecl = dynamic_cast<VarDeclAST*>(decl)) {
            for (auto& varDef : varDecl->getVarDefs()) {
                if (varDef->getInitVal()) {
                    changed |= foldInitVal(varDef->getInitVal());
                }
            }
        }
        // 处理常量声明
        else if (auto constDecl = dynamic_cast<ConstDeclAST*>(decl)) {
            for (auto& constDef : constDecl->getConstDefs()) {
                if (constDef->getInitVal()) {
                    changed |= foldInitVal(constDef->getInitVal());
                }
            }
        }
        
        return changed;
    }
    
    bool foldInitVal(InitValAST* initVal) {
        bool changed = false;
        
        // 处理表达式初始化值
        if (auto exprInit = dynamic_cast<ExprInitValAST*>(initVal)) {
            auto foldedExpr = foldAndReplaceExpr(exprInit->getExpr());
            if (foldedExpr.get() != exprInit->getExpr()) {
                exprInit->setExpr(std::move(foldedExpr));
                changed = true;
            }
        }
        // 处理列表初始化值（数组）
        else if (auto listInit = dynamic_cast<ListInitValAST*>(initVal)) {
            // 获取可修改的初始化值列表
            auto& initVals = listInit->getMutableInitVals();
            
            // 遍历并折叠每个初始化值
            for (size_t i = 0; i < initVals.size(); ++i) {
                if (foldInitVal(initVals[i].get())) {
                    changed = true;
                }
            }
        }
        
        return changed;
    }
    
    bool foldStmt(StmtAST* stmt) {
        bool changed = false;
        
        // 递归处理不同类型的语句
        if (auto assignStmt = dynamic_cast<AssignStmtAST*>(stmt)) {
            // 折叠赋值语句的表达式
            auto foldedExpr = foldAndReplaceExpr(assignStmt->getExpr());
            if (foldedExpr.get() != assignStmt->getExpr()) {
                // 替换为折叠后的表达式
                assignStmt->setExpr(std::move(foldedExpr));
                changed = true;
            }
        }
        else if (auto ifStmt = dynamic_cast<IfStmtAST*>(stmt)) {
            // 折叠条件表达式
            auto foldedCond = foldAndReplaceExpr(ifStmt->getCondition());
            if (foldedCond.get() != ifStmt->getCondition()) {
                ifStmt->setCondition(std::move(foldedCond));
                changed = true;
            }
            
            // 递归折叠分支语句
            changed |= foldStmt(ifStmt->getThenStmt());
            if (ifStmt->getElseStmt()) {
                changed |= foldStmt(ifStmt->getElseStmt());
            }
            
            // 如果条件是常量，可以直接简化
            if (auto constCond = dynamic_cast<IntConstExprAST*>(ifStmt->getCondition())) {
                
                changed = true;
            }
        }
        else if (auto whileStmt = dynamic_cast<WhileStmtAST*>(stmt)) {
            // 折叠条件表达式
            auto foldedCond = foldAndReplaceExpr(whileStmt->getCondition());
            if (foldedCond.get() != whileStmt->getCondition()) {
                whileStmt->setCondition(std::move(foldedCond));
                changed = true;
            }
            
            // 递归折叠循环体
            changed |= foldStmt(whileStmt->getBody());
        }
        else if (auto block = dynamic_cast<BlockAST*>(stmt)) {
            changed |= foldBlock(block);
        }
        else if (auto exprStmt = dynamic_cast<ExprStmtAST*>(stmt)) {
            if (exprStmt->getExpr()) {
                auto foldedExpr = foldAndReplaceExpr(exprStmt->getExpr());
                if (foldedExpr.get() != exprStmt->getExpr()) {
                    exprStmt->setExpr(std::move(foldedExpr));
                    changed = true;
                }
            }
        }
        else if (auto returnStmt = dynamic_cast<ReturnStmtAST*>(stmt)) {
            if (returnStmt->getReturnValue()) {
                auto foldedExpr = foldAndReplaceExpr(returnStmt->getReturnValue());
                if (foldedExpr.get() != returnStmt->getReturnValue()) {
                    returnStmt->setReturnValue(std::move(foldedExpr));
                    changed = true;
                }
            }
        }
        
        return changed;
    }
    
    // 核心：折叠表达式
    // 返回是否发生了折叠
    bool foldExpr(ExprAST* expr) {
        if (!expr) {
            return false;
        }
        
        bool changed = false;
        
        return changed;
    }
    
    // 折叠表达式并返回折叠后的新表达式
    std::unique_ptr<ExprAST> foldAndReplaceExpr(ExprAST* expr) {
        if (!expr) {
            return nullptr;
        }
        
        // 如果已经是常量表达式，直接返回克隆
        if (expr->isConstant()) {
            return std::unique_ptr<ExprAST>(static_cast<ExprAST*>(expr->clone().release()));
        }
        
        // 先处理二元表达式
        if (auto binExpr = dynamic_cast<BinaryExprAST*>(expr)) {
            // 递归折叠子表达式
            auto lhs = foldAndReplaceExpr(binExpr->getLHS());
            auto rhs = foldAndReplaceExpr(binExpr->getRHS());
            
            // 检查折叠后的子表达式是否都是整数常量
            auto lhsIntConst = dynamic_cast<IntConstExprAST*>(lhs.get());
            auto rhsIntConst = dynamic_cast<IntConstExprAST*>(rhs.get());
            
            // 检查折叠后的子表达式是否都是浮点数常量
            auto lhsFloatConst = dynamic_cast<FloatConstExprAST*>(lhs.get());
            auto rhsFloatConst = dynamic_cast<FloatConstExprAST*>(rhs.get());
            
            if (lhsIntConst && rhsIntConst) {
                // 计算整数常量结果
                int result = evaluateBinaryOp(
                    binExpr->getOp(), 
                    lhsIntConst->getValue(), 
                    rhsIntConst->getValue()
                );
                
                // 返回整数常量表达式
                return std::make_unique<IntConstExprAST>(result, binExpr->getLineNumber());
            }
            else if (lhsFloatConst && rhsFloatConst) {
                // 计算浮点数常量结果
                float result = evaluateBinaryOp(
                    binExpr->getOp(), 
                    lhsFloatConst->getValue(), 
                    rhsFloatConst->getValue()
                );
                
                // 返回浮点数常量表达式
                return std::make_unique<FloatConstExprAST>(result, binExpr->getLineNumber());
            }
            
            // 如果不是常量表达式，返回新的二元表达式
            return std::make_unique<BinaryExprAST>(
                binExpr->getOp(), 
                std::move(lhs), 
                std::move(rhs), 
                binExpr->getLineNumber()
            );
        }
        // 处理一元表达式
        else if (auto unaryExpr = dynamic_cast<UnaryExprAST*>(expr)) {
            // 递归折叠子表达式
            auto operand = foldAndReplaceExpr(unaryExpr->getOperand());
            
            // 检查折叠后的操作数是否是整数常量
            if (auto intConstOperand = dynamic_cast<IntConstExprAST*>(operand.get())) {
                // 计算整数常量结果
                int result = evaluateUnaryOp(unaryExpr->getOp(), intConstOperand->getValue());
                
                // 返回整数常量表达式
                return std::make_unique<IntConstExprAST>(result, unaryExpr->getLineNumber());
            }
            // 检查折叠后的操作数是否是浮点数常量
            else if (auto floatConstOperand = dynamic_cast<FloatConstExprAST*>(operand.get())) {
                // 计算浮点数常量结果
                float result = evaluateUnaryOp(unaryExpr->getOp(), floatConstOperand->getValue());
                
                // 返回浮点数常量表达式
                return std::make_unique<FloatConstExprAST>(result, unaryExpr->getLineNumber());
            }
            
            // 如果不是常量表达式，返回新的一元表达式
            return std::make_unique<UnaryExprAST>(
                unaryExpr->getOp(), 
                std::move(operand), 
                unaryExpr->getLineNumber()
            );
        }
        // 处理函数调用表达式
        else if (auto callExpr = dynamic_cast<CallExprAST*>(expr)) {
            // 递归折叠参数表达式
            auto newCall = std::make_unique<CallExprAST>(callExpr->getCallee(), callExpr->getLineNumber());
            for (auto& arg : callExpr->getArgs()) {
                newCall->addArg(foldAndReplaceExpr(arg.get()));
            }
            return newCall;
        }
        // 处理左值表达式
        else if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr)) {
            // 递归折叠数组下标
            auto newLVal = std::make_unique<LValExprAST>(lvalExpr->getName(), lvalExpr->getLineNumber());
            for (auto& idx : lvalExpr->getIndices()) {
                newLVal->addIndex(foldAndReplaceExpr(idx.get()));
            }
            return newLVal;
        }
        // 如果是其他类型的表达式，返回原表达式的克隆
        return std::unique_ptr<ExprAST>(static_cast<ExprAST*>(expr->clone().release()));
    }
    
    // 整数版本的二元运算
    int evaluateBinaryOp(BinaryExprAST::Operator op, int lhs, int rhs) {
        switch (op) {
            case BinaryExprAST::ADD: return lhs + rhs;
            case BinaryExprAST::SUB: return lhs - rhs;
            case BinaryExprAST::MUL: return lhs * rhs;
            case BinaryExprAST::DIV: return rhs != 0 ? lhs / rhs : 0;
            case BinaryExprAST::MOD: return rhs != 0 ? lhs % rhs : 0;
            case BinaryExprAST::LT:  return lhs < rhs;
            case BinaryExprAST::GT:  return lhs > rhs;
            case BinaryExprAST::LE:  return lhs <= rhs;
            case BinaryExprAST::GE:  return lhs >= rhs;
            case BinaryExprAST::EQ:  return lhs == rhs;
            case BinaryExprAST::NE:  return lhs != rhs;
            case BinaryExprAST::AND: return lhs && rhs;
            case BinaryExprAST::OR:  return lhs || rhs;
            default: return 0;
        }
    }
    
    // 浮点数版本的二元运算
    float evaluateBinaryOp(BinaryExprAST::Operator op, float lhs, float rhs) {
        switch (op) {
            case BinaryExprAST::ADD: return lhs + rhs;
            case BinaryExprAST::SUB: return lhs - rhs;
            case BinaryExprAST::MUL: return lhs * rhs;
            case BinaryExprAST::DIV: return rhs != 0.0f ? lhs / rhs : 0.0f;
            // 浮点数不支持取模运算，返回0
            case BinaryExprAST::MOD: return 0.0f;
            case BinaryExprAST::LT:  return lhs < rhs ? 1.0f : 0.0f;
            case BinaryExprAST::GT:  return lhs > rhs ? 1.0f : 0.0f;
            case BinaryExprAST::LE:  return lhs <= rhs ? 1.0f : 0.0f;
            case BinaryExprAST::GE:  return lhs >= rhs ? 1.0f : 0.0f;
            case BinaryExprAST::EQ:  return lhs == rhs ? 1.0f : 0.0f;
            case BinaryExprAST::NE:  return lhs != rhs ? 1.0f : 0.0f;
            case BinaryExprAST::AND: return lhs && rhs ? 1.0f : 0.0f;
            case BinaryExprAST::OR:  return lhs || rhs ? 1.0f : 0.0f;
            default: return 0.0f;
        }
    }
    
    // 整数版本的一元运算
    int evaluateUnaryOp(UnaryExprAST::Operator op, int operand) {
        switch (op) {
            case UnaryExprAST::PLUS:  return +operand;
            case UnaryExprAST::MINUS: return -operand;
            case UnaryExprAST::NOT:   return !operand;
            default: return operand;
        }
    }
    
    // 浮点数版本的一元运算
    float evaluateUnaryOp(UnaryExprAST::Operator op, float operand) {
        switch (op) {
            case UnaryExprAST::PLUS:  return +operand;
            case UnaryExprAST::MINUS: return -operand;
            case UnaryExprAST::NOT:   return !operand ? 1.0f : 0.0f;
            default: return operand;
        }
    }
};

#endif // CONSTANT_FOLDING_H