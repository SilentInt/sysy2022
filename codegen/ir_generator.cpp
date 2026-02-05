#include "ir_generator.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include <stdexcept>
#include <iostream>


IRGenerator::IRGenerator() : builder(context), currentFunction(nullptr) {
    // 构造函数中初始化 IRBuilder
}

// 进入新作用域
void IRGenerator::pushScope() {
    symbolTableStack.push_back(std::map<std::string, SymbolInfo>());
}

// 退出当前作用域
void IRGenerator::popScope() {
    if (symbolTableStack.empty()) {
        throw std::runtime_error("Cannot pop scope: stack is empty");
    }
    symbolTableStack.pop_back();
}

// 查找符号（从内到外查找）
std::optional<IRGenerator::SymbolInfo> IRGenerator::lookupSymbol(const std::string& name) {
    // 从最内层作用域开始查找
    
    for (auto it = symbolTableStack.rbegin(); it != symbolTableStack.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;  // 返回副本
        }
    }
    return std::nullopt;  // 未找到
}

// 在当前作用域添加符号
void IRGenerator::addSymbol(const std::string& name, const SymbolInfo& info) {
    if (symbolTableStack.empty()) {
        throw std::runtime_error("Cannot add symbol: no scope available");
    }
    // 检查当前作用域是否已存在同名符号
    auto& currentScope = symbolTableStack.back();
    if (currentScope.find(name) != currentScope.end()) {
        throw std::runtime_error("Redeclaration of symbol '" + name + "'");
    }
    // 添加到最内层作用域
    symbolTableStack.back()[name] = info;
}

// 将 AST 类型转换为 LLVM 类型
llvm::Type* IRGenerator::getType(TypeAST* typeAST) {
    if (!typeAST) {
        throw std::runtime_error("Type is null");
    }

    switch (typeAST->getKind()) {
        case TypeAST::Kind::INT:
            return llvm::Type::getInt32Ty(context);
        case TypeAST::Kind::FLOAT:
            return llvm::Type::getFloatTy(context);
        case TypeAST::Kind::VOID:
            return llvm::Type::getVoidTy(context);
        case TypeAST::Kind::VECTOR: {
            auto sizeExpr = typeAST->getVectorSizeExpr();
            if (!sizeExpr) {
                throw std::runtime_error("Vector type missing size expression");
            }
            int size = evaluateConstExpr(sizeExpr);
            if (size <= 0) {
                throw std::runtime_error("Vector size must be positive");
            }
            llvm::Type* elemType = nullptr;
            switch (typeAST->getVectorElementKind()) {
                case TypeAST::Kind::INT:
                    elemType = llvm::Type::getInt32Ty(context);
                    break;
                case TypeAST::Kind::FLOAT:
                    elemType = llvm::Type::getFloatTy(context);
                    break;
                default:
                    throw std::runtime_error("Unsupported vector element type");
            }
            return llvm::VectorType::get(elemType, static_cast<unsigned>(size), false);
        }
        default:
            throw std::runtime_error("Unknown type");
    }
}

// 编译期常量求值函数
int IRGenerator::evaluateConstExpr(ExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("Expression is null");
    }

    // 处理整数字面量
    if (auto intExpr = dynamic_cast<IntConstExprAST*>(expr)) {
        // 检查是否是非负整数
        if (intExpr->getValue() < 0) {
            throw std::runtime_error("Array size must be non-negative");
        }
        return intExpr->getValue();
    }

    // 处理左值表达式（必须是常量）
    if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr)) {
        // 查找变量
        const std::string& varName = lvalExpr->getName();
        auto symOpt = lookupSymbol(varName);
        if (!symOpt) {
            throw std::runtime_error("Variable '" + varName + "' not defined");
        }

        // 检查是否是常量
        llvm::Value* varPtr = symOpt.value().value;
        if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(varPtr)) {
            // 全局常量
            if (!globalVar->isConstant()) {
                throw std::runtime_error("Array size must be a constant");
            }

            // 获取初始值
            llvm::Constant* initValue = globalVar->getInitializer();
            if (!initValue) {
                throw std::runtime_error("Array size must be a constant with value");
            }

            // 检查是否是非负整数
            if (auto intConst = llvm::dyn_cast<llvm::ConstantInt>(initValue)) {
                if (intConst->getSExtValue() < 0) {
                    throw std::runtime_error("Array size must be non-negative");
                }
                return intConst->getSExtValue();
            } else {
                throw std::runtime_error("Array size must be an integer constant");
            }
        } else {
            // 局部变量不能作为数组大小
            throw std::runtime_error("Array size must be a constant");
        }
    }

    // 处理二元运算表达式
    if (auto binaryExpr = dynamic_cast<BinaryExprAST*>(expr)) {
        // 递归求值左右操作数
        int lhs = evaluateConstExpr(binaryExpr->getLHS());
        int rhs = evaluateConstExpr(binaryExpr->getRHS());

        // 根据运算符进行计算
        switch (binaryExpr->getOp()) {
            case BinaryExprAST::ADD:
                return lhs + rhs;
            case BinaryExprAST::SUB:
                return lhs - rhs;
            case BinaryExprAST::MUL:
                return lhs * rhs;
            case BinaryExprAST::DIV:
                if (rhs == 0) {
                    throw std::runtime_error("Division by zero in constant expression");
                }
                return lhs / rhs;
            case BinaryExprAST::MOD:
                if (rhs == 0) {
                    throw std::runtime_error("Modulo by zero in constant expression");
                }
                return lhs % rhs;
            default:
                throw std::runtime_error("Unsupported operator in constant expression");
        }
    }

    // 处理一元运算表达式
    if (auto unaryExpr = dynamic_cast<UnaryExprAST*>(expr)) {
        // 递归求值操作数
        int operand = evaluateConstExpr(unaryExpr->getOperand());

        // 根据运算符进行计算
        switch (unaryExpr->getOp()) {
            case UnaryExprAST::PLUS:
                return operand;
            case UnaryExprAST::MINUS:
                return -operand;
            default:
                throw std::runtime_error("Unsupported unary operator in constant expression");
        }
    }

    throw std::runtime_error("Array size must be a constant integer expression");
}

// 生成表达式的 IR
llvm::Value* IRGenerator::generateExprForArraySize(ExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("Expression is null");
    }

    // 处理整数字面量
    if (auto intExpr = dynamic_cast<IntConstExprAST*>(expr)) {
        // 检查是否是非负整数
        if (intExpr->getValue() < 0) {
            throw std::runtime_error("Array size must be non-negative");
        }

        // 创建 32 位整数常量
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            intExpr->getValue(),
            true  // 有符号
        );
    }

    // 处理左值表达式（必须是常量）
    if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr)) {
        // 查找变量
        const std::string& varName = lvalExpr->getName();
        auto symOpt = lookupSymbol(varName);
        if (!symOpt) {
            throw std::runtime_error("Variable '" + varName + "' not defined");
        }

        // 检查是否是常量
        llvm::Value* varPtr = symOpt.value().value;
        if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(varPtr)) {
            // 全局常量
            if (!globalVar->isConstant()) {
                throw std::runtime_error("Array size must be a constant");
            }

            // 获取初始值
            llvm::Constant* initValue = globalVar->getInitializer();
            if (!initValue) {
                throw std::runtime_error("Array size must be a constant with value");
            }

            // 检查是否是非负整数
            if (auto intConst = llvm::dyn_cast<llvm::ConstantInt>(initValue)) {
                if (intConst->getSExtValue() < 0) {
                    throw std::runtime_error("Array size must be non-negative");
                }
                return intConst;
            } else {
                throw std::runtime_error("Array size must be an integer constant");
            }
        } else {
            // 局部变量不能作为数组大小
            throw std::runtime_error("Array size must be a constant");
        }
    }

    // 处理二元运算表达式（必须是常量表达式）
    if (auto binaryExpr = dynamic_cast<BinaryExprAST*>(expr)) {
        // 使用 evaluateConstExpr 在编译时计算常量值
        int value = evaluateConstExpr(binaryExpr);
        // 创建 32 位整数常量
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            value,
            true  // 有符号
        );
    }

    // 处理一元运算表达式（必须是常量表达式）
    if (auto unaryExpr = dynamic_cast<UnaryExprAST*>(expr)) {
        // 使用 evaluateConstExpr 在编译时计算常量值
        int value = evaluateConstExpr(unaryExpr);
        // 创建 32 位整数常量
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            value,
            true  // 有符号
        );
    }

    throw std::runtime_error("Array size must be a constant integer expression");
}

llvm::Value* IRGenerator::generateExpr(ExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("Expression is null");
    }
    
    // 处理整数字面量
    if (auto intExpr = dynamic_cast<IntConstExprAST*>(expr)) {
        // 创建 32 位整数常量
       
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            intExpr->getValue(),
            true  // 有符号
        );
    }
    
    // 处理浮点数字面量
    if (auto floatExpr = dynamic_cast<FloatConstExprAST*>(expr)) {
        // 创建浮点数常量
        
        return llvm::ConstantFP::get(
            llvm::Type::getFloatTy(context),
            floatExpr->getValue()
        );
    }
    
    // 处理字符串字面量
    if (auto strExpr = dynamic_cast<StringLiteralExprAST*>(expr)) {
       
        return generateStringLiteral(strExpr);
    }
    
    // 处理左值表达式（变量或数组访问）
    if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr)) {
        
        return generateLVal(lvalExpr);
    }
    
    // 处理函数调用表达式
    if (auto callExpr = dynamic_cast<CallExprAST*>(expr)) {
        
        return generateCallExpr(callExpr);
    }

    // 处理二元运算表达式
    if (auto binaryExpr = dynamic_cast<BinaryExprAST*>(expr)) {
        // 生成左侧表达式
        llvm::Value* lhs = generateExpr(binaryExpr->getLHS());
        // 生成右侧表达式
        llvm::Value* rhs = generateExpr(binaryExpr->getRHS());
        
        // 类型转换逻辑：当两个操作数都是int类型时，进行整数运算；否则，转换为float类型进行浮点运算
        llvm::Type* lhsType = lhs->getType();
        llvm::Type* rhsType = rhs->getType();
        bool lhsIsVector = lhsType->isVectorTy();
        bool rhsIsVector = rhsType->isVectorTy();
        if (lhsIsVector || rhsIsVector) {
            // 向量-向量运算
            if (lhsIsVector && rhsIsVector) {
                if (lhsType != rhsType) {
                    throw std::runtime_error("Vector operands must have the same type");
                }
                auto vecType = llvm::cast<llvm::VectorType>(lhsType);
                llvm::Type* elemType = vecType->getElementType();
                bool elemIsFloat = elemType->isFloatingPointTy();
                switch (binaryExpr->getOp()) {
                    case BinaryExprAST::ADD:
                        return elemIsFloat ? builder.CreateFAdd(lhs, rhs, "vaddtmp")
                                           : builder.CreateAdd(lhs, rhs, "vaddtmp");
                    case BinaryExprAST::SUB:
                        return elemIsFloat ? builder.CreateFSub(lhs, rhs, "vsubtmp")
                                           : builder.CreateSub(lhs, rhs, "vsubtmp");
                    case BinaryExprAST::MUL:
                        return elemIsFloat ? builder.CreateFMul(lhs, rhs, "vmultmp")
                                           : builder.CreateMul(lhs, rhs, "vmultmp");
                    case BinaryExprAST::DIV:
                        return elemIsFloat ? builder.CreateFDiv(lhs, rhs, "vdivtmp")
                                           : builder.CreateSDiv(lhs, rhs, "vdivtmp");
                    default:
                        throw std::runtime_error("Unsupported vector binary operator");
                }
            }

            // 向量-标量广播运算
            auto op = binaryExpr->getOp();
            if (op != BinaryExprAST::ADD && op != BinaryExprAST::SUB &&
                op != BinaryExprAST::MUL && op != BinaryExprAST::DIV &&
                op != BinaryExprAST::MOD) {
                throw std::runtime_error("Unsupported vector-scalar operator");
            }

            bool scalarOnLeft = !lhsIsVector;
            llvm::Value* vecValue = lhsIsVector ? lhs : rhs;
            llvm::Value* scalarValue = lhsIsVector ? rhs : lhs;
            auto vecType = llvm::dyn_cast<llvm::VectorType>(vecValue->getType());
            if (!vecType) {
                throw std::runtime_error("Vector-scalar operation requires a vector operand");
            }
            auto fixedVecType = llvm::dyn_cast<llvm::FixedVectorType>(vecType);
            if (!fixedVecType) {
                throw std::runtime_error("Vector-scalar operation only supports fixed-length vectors");
            }

            llvm::Type* elemType = vecType->getElementType();
            bool elemIsFloat = elemType->isFloatingPointTy();

            // 标量类型适配
            if (elemIsFloat) {
                if (scalarValue->getType()->isIntegerTy()) {
                    scalarValue = builder.CreateSIToFP(scalarValue, elemType, "vsplat.int2float");
                } else if (!scalarValue->getType()->isFloatingPointTy()) {
                    throw std::runtime_error("Vector-scalar multiplication expects int/float scalar");
                }
            } else {
                if (!scalarValue->getType()->isIntegerTy()) {
                    throw std::runtime_error("Vector<int> scalar must be integer");
                }
            }

            auto numElems = fixedVecType->getNumElements();
            llvm::Value* splat = builder.CreateVectorSplat(numElems, scalarValue, "vsplat");
            switch (op) {
                case BinaryExprAST::ADD:
                    return elemIsFloat ? builder.CreateFAdd(vecValue, splat, "vsaddtmp")
                                       : builder.CreateAdd(vecValue, splat, "vsaddtmp");
                case BinaryExprAST::SUB:
                    if (scalarOnLeft) {
                        return elemIsFloat ? builder.CreateFSub(splat, vecValue, "vssubtmp")
                                           : builder.CreateSub(splat, vecValue, "vssubtmp");
                    }
                    return elemIsFloat ? builder.CreateFSub(vecValue, splat, "vssubtmp")
                                       : builder.CreateSub(vecValue, splat, "vssubtmp");
                case BinaryExprAST::MUL:
                    return elemIsFloat ? builder.CreateFMul(vecValue, splat, "vsmultmp")
                                       : builder.CreateMul(vecValue, splat, "vsmultmp");
                case BinaryExprAST::DIV:
                    if (scalarOnLeft) {
                        return elemIsFloat ? builder.CreateFDiv(splat, vecValue, "vsdivtmp")
                                           : builder.CreateSDiv(splat, vecValue, "vsdivtmp");
                    }
                    return elemIsFloat ? builder.CreateFDiv(vecValue, splat, "vsdivtmp")
                                       : builder.CreateSDiv(vecValue, splat, "vsdivtmp");
                case BinaryExprAST::MOD:
                    if (elemIsFloat) {
                        throw std::runtime_error("Vector-scalar modulo does not support float");
                    }
                    if (scalarOnLeft) {
                        return builder.CreateSRem(splat, vecValue, "vsmodtmp");
                    }
                    return builder.CreateSRem(vecValue, splat, "vsmodtmp");
                default:
                    throw std::runtime_error("Unsupported vector-scalar operator");
            }
        }
        bool lhsIsInt = lhsType->isIntegerTy();
        bool rhsIsInt = rhsType->isIntegerTy();
        bool isFloat = !(lhsIsInt && rhsIsInt);
        
        // 转换为float类型（如果需要）
        if (isFloat) {
            if (lhsIsInt) {
                lhs = builder.CreateSIToFP(lhs, llvm::Type::getFloatTy(context), "int2float_lhs");
            }
            if (rhsIsInt) {
                rhs = builder.CreateSIToFP(rhs, llvm::Type::getFloatTy(context), "int2float_rhs");
            }
        }
        
        switch (binaryExpr->getOp()) {
            // 算术运算
            case BinaryExprAST::ADD:
                if (isFloat) {
                    return builder.CreateFAdd(lhs, rhs, "addtmp");
                } else {
                    return builder.CreateAdd(lhs, rhs, "addtmp");
                }
            case BinaryExprAST::SUB:
                if (isFloat) {
                    return builder.CreateFSub(lhs, rhs, "subtmp");
                } else {
                    return builder.CreateSub(lhs, rhs, "subtmp");
                }
            case BinaryExprAST::MUL:
                if (isFloat) {
                    return builder.CreateFMul(lhs, rhs, "multmp");
                } else {
                    return builder.CreateMul(lhs, rhs, "multmp");
                }
            case BinaryExprAST::DIV:
                if (isFloat) {
                    return builder.CreateFDiv(lhs, rhs, "divtmp");
                } else {
                    // 使用有符号除法
                    return builder.CreateSDiv(lhs, rhs, "divtmp");
                }
            case BinaryExprAST::MOD:
                if (isFloat) {
                    // 浮点数不支持取模运算，返回0.0
                    return llvm::Constant::getNullValue(llvm::Type::getFloatTy(context));
                } else {
                    // 使用有符号取模
                    return builder.CreateSRem(lhs, rhs, "modtmp");
                }
            // 关系运算
            case BinaryExprAST::LT:
                if (isFloat) {
                    return builder.CreateFCmpOLT(lhs, rhs, "lttmp");
                } else {
                    return builder.CreateICmpSLT(lhs, rhs, "lttmp");
                }
            case BinaryExprAST::GT:
                if (isFloat) {
                    return builder.CreateFCmpOGT(lhs, rhs, "gttmp");
                } else {
                    return builder.CreateICmpSGT(lhs, rhs, "gttmp");
                }
            case BinaryExprAST::LE:
                if (isFloat) {
                    return builder.CreateFCmpOLE(lhs, rhs, "letmp");
                } else {
                    return builder.CreateICmpSLE(lhs, rhs, "letmp");
                }
            case BinaryExprAST::GE:
                if (isFloat) {
                    return builder.CreateFCmpOGE(lhs, rhs, "getmp");
                } else {
                    return builder.CreateICmpSGE(lhs, rhs, "getmp");
                }
            // 相等性运算
            case BinaryExprAST::EQ:
                if (isFloat) {
                    return builder.CreateFCmpOEQ(lhs, rhs, "eqtmp");
                } else {
                    return builder.CreateICmpEQ(lhs, rhs, "eqtmp");
                }
            case BinaryExprAST::NE:
                if (isFloat) {
                    return builder.CreateFCmpONE(lhs, rhs, "netmp");
                } else {
                    return builder.CreateICmpNE(lhs, rhs, "netmp");
                }
            // 逻辑运算（不能在Exp中出现）
            case BinaryExprAST::AND:
                throw std::runtime_error("Logical AND operator cannot be used in expressions");
            case BinaryExprAST::OR:
                throw std::runtime_error("Logical OR operator cannot be used in expressions");
            default:
                throw std::runtime_error("Unknown binary operator");
        }
    }

    // 处理一元运算表达式
    if (auto unaryExpr = dynamic_cast<UnaryExprAST*>(expr)) {
        // 生成操作数表达式
        llvm::Value* operand = generateExpr(unaryExpr->getOperand());
        
        // 根据运算符类型生成相应的IR指令
        switch (unaryExpr->getOp()) {
            case UnaryExprAST::PLUS:
                // 正号不改变值，直接返回操作数
                return operand;
            case UnaryExprAST::MINUS:
                // 根据操作数类型选择取负指令
                if (operand->getType()->isFloatingPointTy()) {
                    // 浮点类型使用浮点取负指令
                    return builder.CreateFNeg(operand, "negtmp");
                } else {
                    // 整数类型使用整数取负指令
                    return builder.CreateNeg(operand, "negtmp");
                }
            case UnaryExprAST::NOT:
                // 逻辑非运算符只能在Cond中出现，不能在Exp中出现
                throw std::runtime_error("Logical NOT operator cannot be used in expressions");
            default:
                throw std::runtime_error("Unknown unary operator");
        }
    }

    throw std::runtime_error("Unsupported expression type");
}

// 生成条件表达式的 IR
llvm::Value* IRGenerator::generateCondExpr(ExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("Expression is null");
    }

    // 处理整数字面量
    if (auto intExpr = dynamic_cast<IntConstExprAST*>(expr)) {
        // 创建 32 位整数常量
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            intExpr->getValue(),
            true  // 有符号
        );
    }

    // 处理浮点数字面量
    if (auto floatExpr = dynamic_cast<FloatConstExprAST*>(expr)) {
        // 创建浮点数常量
        return llvm::ConstantFP::get(
            llvm::Type::getFloatTy(context),
            floatExpr->getValue()
        );
    }

    // 处理左值表达式（变量或数组访问）
    if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr)) {
        llvm::Value* value = generateLVal(lvalExpr);
        if (value->getType()->isVectorTy()) {
            throw std::runtime_error("Vector value cannot be used as a condition");
        }
        return value;
    }

    // 处理函数调用表达式
    if (auto callExpr = dynamic_cast<CallExprAST*>(expr)) {
        llvm::Value* value = generateCallExpr(callExpr);
        if (value->getType()->isVectorTy()) {
            throw std::runtime_error("Vector value cannot be used as a condition");
        }
        return value;
    }

    // 处理二元运算表达式
    if (auto binaryExpr = dynamic_cast<BinaryExprAST*>(expr)) {
        // 生成左侧表达式
        llvm::Value* lhs = generateCondExpr(binaryExpr->getLHS());
        // 生成右侧表达式
        llvm::Value* rhs = generateCondExpr(binaryExpr->getRHS());

        // 类型转换逻辑：当两个操作数都是int类型时，进行整数运算；否则，转换为float类型进行浮点运算
        llvm::Type* lhsType = lhs->getType();
        llvm::Type* rhsType = rhs->getType();
        if (lhsType->isVectorTy() || rhsType->isVectorTy()) {
            throw std::runtime_error("Vector value cannot be used in conditional expressions");
        }
        bool lhsIsInt = lhsType->isIntegerTy();
        bool rhsIsInt = rhsType->isIntegerTy();
        bool isFloat = !(lhsIsInt && rhsIsInt);
        
        // 转换为float类型（如果需要）
        if (isFloat) {
            if (lhsIsInt) {
                lhs = builder.CreateSIToFP(lhs, llvm::Type::getFloatTy(context), "int2float_lhs");
            }
            if (rhsIsInt) {
                rhs = builder.CreateSIToFP(rhs, llvm::Type::getFloatTy(context), "int2float_rhs");
            }
        }
        
        switch (binaryExpr->getOp()) {
            // 算术运算
            case BinaryExprAST::ADD:
                if (isFloat) {
                    return builder.CreateFAdd(lhs, rhs, "addtmp");
                } else {
                    return builder.CreateAdd(lhs, rhs, "addtmp");
                }
            case BinaryExprAST::SUB:
                if (isFloat) {
                    return builder.CreateFSub(lhs, rhs, "subtmp");
                } else {
                    return builder.CreateSub(lhs, rhs, "subtmp");
                }
            case BinaryExprAST::MUL:
                if (isFloat) {
                    return builder.CreateFMul(lhs, rhs, "multmp");
                } else {
                    return builder.CreateMul(lhs, rhs, "multmp");
                }
            case BinaryExprAST::DIV:
                if (isFloat) {
                    return builder.CreateFDiv(lhs, rhs, "divtmp");
                } else {
                    // 使用有符号除法
                    return builder.CreateSDiv(lhs, rhs, "divtmp");
                }
            case BinaryExprAST::MOD:
                if (isFloat) {
                    // 浮点数不支持取模运算，返回0.0
                    return llvm::Constant::getNullValue(llvm::Type::getFloatTy(context));
                } else {
                    // 使用有符号取模
                    return builder.CreateSRem(lhs, rhs, "modtmp");
                }
            // 关系运算
            case BinaryExprAST::LT:
                if (isFloat) {
                    return builder.CreateFCmpOLT(lhs, rhs, "lttmp");
                } else {
                    return builder.CreateICmpSLT(lhs, rhs, "lttmp");
                }
            case BinaryExprAST::GT:
                if (isFloat) {
                    return builder.CreateFCmpOGT(lhs, rhs, "gttmp");
                } else {
                    return builder.CreateICmpSGT(lhs, rhs, "gttmp");
                }
            case BinaryExprAST::LE:
                if (isFloat) {
                    return builder.CreateFCmpOLE(lhs, rhs, "letmp");
                } else {
                    return builder.CreateICmpSLE(lhs, rhs, "letmp");
                }
            case BinaryExprAST::GE:
                if (isFloat) {
                    return builder.CreateFCmpOGE(lhs, rhs, "getmp");
                } else {
                    return builder.CreateICmpSGE(lhs, rhs, "getmp");
                }
            // 相等性运算
            case BinaryExprAST::EQ:
                if (isFloat) {
                    return builder.CreateFCmpOEQ(lhs, rhs, "eqtmp");
                } else {
                    return builder.CreateICmpEQ(lhs, rhs, "eqtmp");
                }
            case BinaryExprAST::NE:
                if (isFloat) {
                    return builder.CreateFCmpONE(lhs, rhs, "netmp");
                } else {
                    return builder.CreateICmpNE(lhs, rhs, "netmp");
                }
            // 逻辑运算
            case BinaryExprAST::AND:
                return builder.CreateAnd(lhs, rhs, "andtmp");
            case BinaryExprAST::OR:
                return builder.CreateOr(lhs, rhs, "ortmp");
            default:
                throw std::runtime_error("Unknown binary operator");
        }
    }

    // 处理一元运算表达式
    if (auto unaryExpr = dynamic_cast<UnaryExprAST*>(expr)) {
        // 生成操作数表达式
        llvm::Value* operand = generateCondExpr(unaryExpr->getOperand());

        // 根据运算符类型生成相应的IR指令
        switch (unaryExpr->getOp()) {
            case UnaryExprAST::PLUS:
                // 正号不改变值，直接返回操作数
                return operand;
            case UnaryExprAST::MINUS:
                // 根据操作数类型选择取负指令
                if (operand->getType()->isFloatingPointTy()) {
                    // 浮点类型使用浮点取负指令
                    return builder.CreateFNeg(operand, "negtmp");
                } else {
                    // 整数类型使用整数取负指令
                    return builder.CreateNeg(operand, "negtmp");
                }
            case UnaryExprAST::NOT:
                // 逻辑非运算，将操作数与0比较
                return builder.CreateICmpEQ(operand, 
                    llvm::ConstantInt::get(operand->getType(), 0), 
                    "nottmp");
            default:
                throw std::runtime_error("Unknown unary operator");
        }
    }

    throw std::runtime_error("Unsupported expression type");
}

// 生成语句的 IR
void IRGenerator::generateStmt(StmtAST* stmt) {
    if (!stmt) {
        throw std::runtime_error("Statement is null");
    }

    // 处理 return 语句
    if (auto retStmt = dynamic_cast<ReturnStmtAST*>(stmt)) {
        // 检查是否有返回值
        if (retStmt->getReturnValue()) {
            // 有返回值，生成表达式并返回
            llvm::Value* retValue = generateExpr(retStmt->getReturnValue());

            // 检查函数返回类型是否为void
            llvm::Type* retType = currentFunction->getReturnType();
            if (retType->isVoidTy()) {
                throw std::runtime_error("Void function cannot return a value");
            }

            // 类型检查和转换：确保返回值类型与函数返回类型一致
            llvm::Type* retValueType = retValue->getType();
            if (retValueType != retType) {
                // 类型转换逻辑
                if (retType->isFloatingPointTy() && retValueType->isIntegerTy()) {
                    // 整数转换为浮点
                    retValue = builder.CreateSIToFP(retValue, retType, "int2float_ret");
                } else if (retType->isIntegerTy() && retValueType->isFloatingPointTy()) {
                    // 浮点转换为整数
                    retValue = builder.CreateFPToSI(retValue, retType, "float2int_ret");
                } else {
                    // 其他类型转换不支持
                    throw std::runtime_error("Unsupported return type conversion");
                }
            }

            builder.CreateRet(retValue);
        } else {
            // 无返回值，检查函数返回类型
            if (!currentFunction->getReturnType()->isVoidTy()) {
                throw std::runtime_error("Non-void function must return a value");
            }

            builder.CreateRetVoid();
        }
        return;
    }

    // 处理 if 语句
    if (auto ifStmt = dynamic_cast<IfStmtAST*>(stmt)) {
        // 生成条件表达式
        llvm::Value* condValue = generateCondExpr(ifStmt->getCondition());

        // 将条件转换为布尔值（非零为真，零为假）
        llvm::Value* boolValue = builder.CreateICmpNE(
            condValue, 
            llvm::ConstantInt::get(condValue->getType(), 0), 
            "ifcond"
        );

        // 获取当前函数
        llvm::Function* theFunction = builder.GetInsertBlock()->getParent();

        // 创建then块和end块
        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context, "then", theFunction);
        llvm::BasicBlock* endBB = llvm::BasicBlock::Create(context, "endif");

        // 如果有else部分，创建else块
        llvm::BasicBlock* elseBB = nullptr;
        if (ifStmt->getElseStmt()) {
            elseBB = llvm::BasicBlock::Create(context, "else");
            // 创建条件跳转
            builder.CreateCondBr(boolValue, thenBB, elseBB);
        } else {
            // 没有else部分，直接跳转到then块或end块
            builder.CreateCondBr(boolValue, thenBB, endBB);
        }

        // 生成then部分
        builder.SetInsertPoint(thenBB);
        generateStmt(ifStmt->getThenStmt());

        // 如果then块没有终止指令，跳转到end块
        if (!builder.GetInsertBlock()->getTerminator()) {
            builder.CreateBr(endBB);
        }

        // 如果有else部分，生成else部分
        if (ifStmt->getElseStmt()) {
            theFunction->insert(theFunction->end(), elseBB);
            builder.SetInsertPoint(elseBB);
            generateStmt(ifStmt->getElseStmt());

            // 如果else块没有终止指令，跳转到end块
            if (!builder.GetInsertBlock()->getTerminator()) {
                builder.CreateBr(endBB);
            }
        }

        // 设置插入点到end块
        theFunction->insert(theFunction->end(), endBB);
        builder.SetInsertPoint(endBB);

        return;
    }

    // 处理表达式语句
    if (auto exprStmt = dynamic_cast<ExprStmtAST*>(stmt)) {
        // 如果有表达式，生成并丢弃结果
        if (exprStmt->getExpr()) {
            generateExpr(exprStmt->getExpr());
        }
        return;
    }
    
    // 处理赋值语句
    if (auto assignStmt = dynamic_cast<AssignStmtAST*>(stmt)) {
        // 检查是否是数组名直接赋值
        auto lval = assignStmt->getLVal();
        const std::string& varName = lval->getName();
        
        // 查找变量
        auto symOpt = lookupSymbol(varName);
        if (symOpt) {
            // 如果是数组且没有使用索引，则是数组名直接赋值，这是非法的
            if (symOpt.value().isArray && lval->getIndices().empty()) {
                throw std::runtime_error("Cannot assign to array name '" + varName + "' directly, use array indexing");
            }
        }
        
        // 向量索引赋值：v[i] = x
        if (symOpt) {
            llvm::Value* varPtr = symOpt.value().value;
            llvm::Type* allocatedType = nullptr;
            if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(varPtr)) {
                allocatedType = allocaInst->getAllocatedType();
            } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(varPtr)) {
                allocatedType = globalVar->getValueType();
            }

            if (allocatedType && allocatedType->isVectorTy() && !lval->getIndices().empty()) {
                if (lval->getIndices().size() != 1) {
                    throw std::runtime_error("Vector index must be one-dimensional");
                }
                llvm::Value* vecValue = builder.CreateLoad(allocatedType, varPtr, "vecload");
                llvm::Value* indexValue = generateExpr(lval->getIndices()[0].get());
                if (!indexValue->getType()->isIntegerTy()) {
                    throw std::runtime_error("Vector index must be integer");
                }
                if (indexValue->getType() != llvm::Type::getInt32Ty(context)) {
                    indexValue = builder.CreateIntCast(indexValue, llvm::Type::getInt32Ty(context), true, "vidxcast");
                }

                llvm::Value* rval = generateExpr(assignStmt->getExpr());
                llvm::Type* elemType = llvm::cast<llvm::VectorType>(allocatedType)->getElementType();
                if (rval->getType() != elemType) {
                    if (elemType->isFloatingPointTy() && rval->getType()->isIntegerTy()) {
                        rval = builder.CreateSIToFP(rval, elemType, "int2float_elem");
                    } else if (elemType->isIntegerTy() && rval->getType()->isFloatingPointTy()) {
                        rval = builder.CreateFPToSI(rval, elemType, "float2int_elem");
                    } else {
                        throw std::runtime_error("Type mismatch in vector element assignment");
                    }
                }

                llvm::Value* newVec = builder.CreateInsertElement(vecValue, rval, indexValue, "vecins");
                builder.CreateStore(newVec, varPtr);
                return;
            }
        }

        // 获取左值地址
        llvm::Value* lvalAddr = generateLValAddress(lval);
        // 生成右值表达式
        llvm::Value* rval = generateExpr(assignStmt->getExpr());
        // 存储值
        builder.CreateStore(rval, lvalAddr);
        return;
    }
    
    // 处理while语句
    if (auto whileStmt = dynamic_cast<WhileStmtAST*>(stmt)) {
        // 获取当前函数
        llvm::Function* theFunction = builder.GetInsertBlock()->getParent();
        
        // 创建循环的基本块
        llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context, "whilecond", theFunction);
        llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(context, "whileloop");
        llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(context, "whileafter");
        
        // 将break和continue目标压入栈
        breakTargets.push_back(afterBB);
        continueTargets.push_back(condBB);
        
        // 跳转到条件块
        builder.CreateBr(condBB);
        
        // 设置插入点到条件块
        builder.SetInsertPoint(condBB);
        
        // 生成条件表达式
        llvm::Value* condValue = generateCondExpr(whileStmt->getCondition());
        
        // 将条件转换为布尔值
        llvm::Value* boolValue = builder.CreateICmpNE(
            condValue,
            llvm::ConstantInt::get(condValue->getType(), 0),
            "whilecond"
        );
        
        // 创建条件跳转
        builder.CreateCondBr(boolValue, loopBB, afterBB);
        
        // 设置插入点到循环体
        theFunction->insert(theFunction->end(), loopBB);
        builder.SetInsertPoint(loopBB);
        
        // 生成循环体
        generateStmt(whileStmt->getBody());
        
        // 如果循环体没有终止指令，跳转回条件块
        if (!builder.GetInsertBlock()->getTerminator()) {
            builder.CreateBr(condBB);
        }
        
        // 设置插入点到after块
        theFunction->insert(theFunction->end(), afterBB);
        builder.SetInsertPoint(afterBB);
        
        // 弹出break和continue目标
        breakTargets.pop_back();
        continueTargets.pop_back();
        
        return;
    }
    
    // 处理break语句
    if (dynamic_cast<BreakStmtAST*>(stmt)) {
        // 检查是否在循环中
        if (breakTargets.empty()) {
            throw std::runtime_error("Break statement outside of loop");
        }
        // 跳转到最近的break目标
        builder.CreateBr(breakTargets.back());
        return;
    }
    
    // 处理continue语句
    if (dynamic_cast<ContinueStmtAST*>(stmt)) {
        // 检查是否在循环中
        if (continueTargets.empty()) {
            throw std::runtime_error("Continue statement outside of loop");
        }
        // 跳转到最近的continue目标
        builder.CreateBr(continueTargets.back());
        return;
    }
    
    // 处理代码块语句
    if (auto blockStmt = dynamic_cast<BlockAST*>(stmt)) {
        generateBlock(blockStmt);
        return;
    }

    throw std::runtime_error("Unsupported statement type");
}

// 生成代码块的 IR
void IRGenerator::generateBlock(BlockAST* block) {
    if (!block) {
        throw std::runtime_error("Block is null");
    }
    

    // 进入新作用域
    pushScope();

    // 遍历所有块项
    for (const auto& item : block->getItems()) {
        // 如果是声明块项，生成声明
        if (auto declItem = dynamic_cast<DeclBlockItemAST*>(item.get())) {
            // 生成声明
            generateDecl(declItem->getDecl());
        }
        // 如果是语句块项，生成语句
        else if (auto stmtItem = dynamic_cast<StmtBlockItemAST*>(item.get())) {
            generateStmt(stmtItem->getStmt());
        }
    }

    // 退出当前作用域
    popScope();
    
}

// 生成函数的 IR
llvm::Function* IRGenerator::generateFunction(FunctionAST* func) {
    if (!func) {
        throw std::runtime_error("Function is null");
    }

    // 检查函数名是否与全局符号冲突
    const std::string& funcName = func->getName();
    auto existingSym = lookupSymbol(funcName);
    if (existingSym) {
        throw std::runtime_error("Redeclaration of function '" + funcName + "'");
    }

    // 进入函数作用域
    pushScope();

    // 获取返回类型
    llvm::Type* retType = getType(func->getReturnType());
    
    // 准备参数类型列表和参数元素类型信息
    std::vector<llvm::Type*> paramTypes;
    std::vector<llvm::Type*> paramElementTypes; // 保存每个参数的元素类型
    
    for (const auto& param : func->getParams()) {
        // 获取参数类型
        llvm::Type* paramType = getType(param->getType());
        llvm::Type* elementType = paramType; // 默认元素类型就是参数类型

        // 检查是否是数组参数
        if (param->getIsArray()) {  // 使用 getIsArray() 而不是检查 getArraySizes()
            // 数组参数转换为指针类型
            elementType = paramType;

            // 处理后续维度（第一维已经被"降级"为指针）
            // 对于 int arr[]，arraySizes 为空，elementType 保持为 i32
            // 对于 int arr[][3]，arraySizes 有一个元素 3，elementType 变为 [3 x i32]
            for (const auto& sizeExpr : param->getArraySizes()) {
                int size = evaluateConstExpr(sizeExpr.get());
                if (size <= 0) {
                    throw std::runtime_error("Array dimension must be positive");
                }
                elementType = llvm::ArrayType::get(elementType, size);
            }

            // 数组参数实际上是指向元素类型的指针
            paramType = llvm::PointerType::get(elementType, 0);
        }

        paramTypes.push_back(paramType);
        paramElementTypes.push_back(elementType); // 保存元素类型信息
    }
    
    // 创建函数类型
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        retType,           // 返回类型
        paramTypes,        // 参数类型列表
        false              // 不是可变参数函数
    );

    // 创建函数
    // 设置链接类型
    llvm::Function::LinkageTypes linkageType;
    
    if (func->getName() == "main") {
        // main函数必须是全局可见的，作为程序入口点
        linkageType = llvm::Function::ExternalLinkage;
    } else {
        // 所有其他自定义函数都使用InternalLinkage，只在当前模块可见
        // 这样可以避免与标准库或运行时库的同名函数冲突
        linkageType = llvm::Function::InternalLinkage;
    }
    
    llvm::Function* llvmFunc = llvm::Function::Create(
        funcType,
        linkageType,  // 链接类型
        func->getName(),                   // 函数名
        module.get()                       // 所属模块
    );

    // 创建入口基本块
    llvm::BasicBlock* entryBB = llvm::BasicBlock::Create(
        context,
        "entry",
        llvmFunc
    );

    // 设置插入点到入口基本块
    builder.SetInsertPoint(entryBB);
    
    // 保存当前函数
    llvm::Function* prevFunction = currentFunction;
    currentFunction = llvmFunc;
    
    // 第一步：为所有参数创建 alloca（但不加载数组指针）
    size_t idx = 0;
    std::vector<std::tuple<std::string, llvm::AllocaInst*, bool, llvm::Type*, llvm::Type*>> paramInfo;
    
    for (auto& arg : llvmFunc->args()) {
        const std::string& paramName = func->getParams()[idx]->getName();
        arg.setName(paramName);

        bool isArray = func->getParams()[idx]->getIsArray();
        
        // 创建 alloca 存储参数值
        llvm::AllocaInst* alloca = builder.CreateAlloca(
            arg.getType(),
            nullptr,
            paramName + "_addr"
        );
        
        builder.CreateStore(&arg, alloca);
        
        // 保存参数信息，稍后处理
        paramInfo.push_back(std::make_tuple(
            paramName,
            alloca,
            isArray,
            arg.getType(),
            paramElementTypes[idx]
        ));

        idx++;
    }
    
    // 第二步：现在加载所有数组参数的指针
    // 确保插入点仍在入口块
    if (builder.GetInsertBlock() != entryBB) {
        throw std::runtime_error("Insert point moved from entry block");
    }

    for (const auto& info : paramInfo) {
        const std::string& paramName = std::get<0>(info);
        llvm::AllocaInst* alloca = std::get<1>(info);
        bool isArray = std::get<2>(info);
        llvm::Type* argType = std::get<3>(info);
        llvm::Type* elemType = std::get<4>(info);
        
        llvm::Value* loadedPtr = nullptr;
        
        if (isArray) {
            // 在入口块立即加载数组指针
            loadedPtr = builder.CreateLoad(argType, alloca, paramName + "_loaded");
        }
        
        // 创建 SymbolInfo
        SymbolInfo symInfo(alloca, false, isArray, elemType);
        symInfo.loadedArrayPtr = loadedPtr;
        addSymbol(paramName, symInfo);
    }

    // 再次确认插入点仍在入口块
    if (builder.GetInsertBlock() != entryBB) {
        throw std::runtime_error("Insert point moved from entry block after loading array pointers");
    }
    
    // 第三步：现在生成函数体
    // 此时所有数组参数的指针都已在入口块加载完毕
    bool isVoidReturn = func->getReturnType()->getKind() == TypeAST::Kind::VOID;

    generateBlock(func->getBody());

    // 对于非void返回类型，检查所有路径是否都有返回值
    if (!isVoidReturn) {
        // 检查当前基本块是否以return语句结束
        if (!builder.GetInsertBlock()->getTerminator()) {
            // 如果没有终止指令，添加一个未定义返回值
            builder.CreateRet(llvm::UndefValue::get(retType));
        }
    } else {
        // 对于void返回类型，如果没有return语句，添加一个
        if (!builder.GetInsertBlock()->getTerminator()) {
            builder.CreateRetVoid();
        }
    }
    
    // 退出函数作用域
    popScope();
    // 恢复当前函数
    currentFunction = prevFunction;


    // 验证函数
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyFunction(*llvmFunc, &errorStream)) {
        errorStream.flush();
        throw std::runtime_error("Function verification failed: " + errorMsg);
    }

    return llvmFunc;
}

// 生成声明的 IR
void IRGenerator::generateDecl(DeclAST* decl) {
    if (!decl) {
        throw std::runtime_error("Declaration is null");
    }
    
    // 检查是否是变量声明
    if (auto varDecl = dynamic_cast<VarDeclAST*>(decl)) {
        // 获取变量类型
        llvm::Type* varType = getType(varDecl->getType());
        
        // 处理每个变量定义
        for (const auto& varDef : varDecl->getVarDefs()) {
            const std::string& varName = varDef->getName();
            
            // 初始化元素类型和数组维度
            llvm::Type* elementType = varType;
            std::vector<int> arraySizes;

            // 检查是否是数组变量
            if (varType->isVectorTy() && !varDef->getArraySizes().empty()) {
                throw std::runtime_error("Vector type cannot be combined with array dimensions");
            }
            if (!varDef->getArraySizes().empty()) {
                // 处理数组维度
                for (const auto& sizeExpr : varDef->getArraySizes()) {
                    // 计算数组维度大小
                    llvm::Value* sizeValue = generateExprForArraySize(sizeExpr.get());

                    // 检查是否是常量表达式
                    if (!llvm::isa<llvm::ConstantInt>(sizeValue)) {
                        throw std::runtime_error("Array size must be a constant expression");
                    }

                    // 获取数组大小
                    int size = llvm::cast<llvm::ConstantInt>(sizeValue)->getSExtValue();
                    if (size < 0) {
                        throw std::runtime_error("Array size must be non-negative");
                    }

                    arraySizes.push_back(size);
                }
                
                // 正确构建多维数组类型：从内到外构建
                // 例如：int arr[3][2] 应该构建为 [3 x [2 x i32]]
                // 所以需要从最后一个维度开始构建
                for (auto it = arraySizes.rbegin(); it != arraySizes.rend(); ++it) {
                    elementType = llvm::ArrayType::get(elementType, *it);
                }
            }

            // 判断是全局变量还是局部变量
            if (currentFunction == nullptr) {
                // 全局变量：在创建之前检查是否已存在同名符号
                auto existingSym = lookupSymbol(varName);
                if (existingSym) {
                    throw std::runtime_error("Redeclaration of global variable '" + varName + "'");
                }
                
                // 全局变量
                llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
                    *module,                    // 模块
                    elementType,                // 类型（可能是数组类型）
                    false,                      // 不是常量
                    llvm::GlobalValue::ExternalLinkage,  // 链接类型
                    nullptr,                    // 初始值（稍后设置）
                    varName                     // 变量名
                );
            
            // 如果有初始化值，设置它
            if (varDef->getInitVal()) {
                // 生成初始化值
                generateInitVal(varDef->getInitVal(), globalVar,
                              static_cast<int>(arraySizes.size()), arraySizes);
            } else {
                // 没有初始化值，使用 ConstantAggregateZero，这样 LLVM 会将其放在 .bss 段
                globalVar->setInitializer(llvm::ConstantAggregateZero::get(elementType));
                // 为大型全局变量添加属性，优化内存布局
                globalVar->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
                globalVar->setAlignment(llvm::Align(4));
            }
            
            // 将变量添加到符号表
            addSymbol(varName, SymbolInfo(globalVar, false, !arraySizes.empty()));
        } else {
            // 局部变量
            // 在栈上分配空间
            llvm::AllocaInst* alloca = builder.CreateAlloca(
                elementType,
                nullptr,
                varName
            );

            // 如果有初始化值，设置它
            if (varDef->getInitVal()) {
                // 生成初始化值
                generateInitVal(varDef->getInitVal(), alloca,
                              static_cast<int>(arraySizes.size()), arraySizes);
            }
            // 注意：局部变量没有初始化值时，不需要设置默认值，因为其值是未定义的

            // 将变量添加到符号表
            addSymbol(varName, SymbolInfo(alloca, false, !arraySizes.empty()));
            }
        }
    }
    // 检查是否是常量声明
    else if (auto constDecl = dynamic_cast<ConstDeclAST*>(decl)) {
        // 获取常量类型
        llvm::Type* constType = getType(constDecl->getType());
        
        // 处理每个常量定义
        for (const auto& constDef : constDecl->getConstDefs()) {
            const std::string& constName = constDef->getName();
            
            // 初始化元素类型和数组维度
            llvm::Type* elementType = constType;
            std::vector<int> arraySizes;
            
            // 检查是否是数组常量
            if (constType->isVectorTy() && !constDef->getArraySizes().empty()) {
                throw std::runtime_error("Vector type cannot be combined with array dimensions");
            }
            if (!constDef->getArraySizes().empty()) {
                // 处理数组维度
                for (const auto& sizeExpr : constDef->getArraySizes()) {
                // 计算数组维度大小
                llvm::Value* sizeValue = generateExpr(sizeExpr.get());
                
                // 检查是否是常量表达式
                if (!llvm::isa<llvm::ConstantInt>(sizeValue)) {
                    throw std::runtime_error("Array size must be a constant expression");
                }
                
                // 获取数组大小
                int size = llvm::cast<llvm::ConstantInt>(sizeValue)->getSExtValue();
                if (size < 0) {
                    throw std::runtime_error("Array size must be non-negative");
                }
                
                arraySizes.push_back(size);
                }
                
                // 正确构建多维数组类型：从内到外构建
                // 例如：int arr[3][2] 应该构建为 [3 x [2 x i32]]
                // 所以需要从最后一个维度开始构建
                for (auto it = arraySizes.rbegin(); it != arraySizes.rend(); ++it) {
                    elementType = llvm::ArrayType::get(elementType, *it);
                }
            }
            
            // 全局常量：在创建之前检查是否已存在同名符号
            auto existingSym = lookupSymbol(constName);
            if (existingSym) {
                throw std::runtime_error("Redeclaration of global constant '" + constName + "'");
            }
            
            // 创建全局常量
            llvm::GlobalVariable* globalConst = new llvm::GlobalVariable(
                *module,                    // 模块
                elementType,                 // 类型（可能是数组类型）
                true,                       // 是常量
                llvm::GlobalValue::ExternalLinkage,  // 链接类型
                nullptr,                    // 初始值（稍后设置）
                constName                   // 常量名
            );
            
            // 设置初始化值（常量必须有初始化值）
            if (constDef->getInitVal()) {
                // 生成初始化值
                generateInitVal(constDef->getInitVal(), globalConst, 
                              static_cast<int>(arraySizes.size()), arraySizes);
            } else {
                throw std::runtime_error("Constant '" + constName + "' must have an initializer");
            }
            
            // 将常量添加到符号表
            addSymbol(constName, SymbolInfo(globalConst, true, !arraySizes.empty()));
        }
    } else {
        throw std::runtime_error("Unknown declaration type");
    }
}

// 生成初始化值的 IR
void IRGenerator::generateInitVal(InitValAST* initVal, llvm::Value* ptr, int dimensions, const std::vector<int>& sizes) {
    if (!initVal) {
        throw std::runtime_error("InitVal is null");
    }

    // 先确定初始化目标类型
    llvm::Type* targetType = nullptr;
    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
        targetType = allocaInst->getAllocatedType();
    } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
        targetType = globalVar->getValueType();
    } else if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
        targetType = gepInst->getResultElementType();
    } else {
        throw std::runtime_error("Unable to determine pointer element type");
    }

    // 处理向量初始化
    if (targetType->isVectorTy()) {
        auto vecType = llvm::cast<llvm::VectorType>(targetType);
        llvm::Type* elemType = vecType->getElementType();
        unsigned vecSize = vecType->getElementCount().getKnownMinValue();

        // 表达式初始化：要求类型为向量
        if (auto exprInit = dynamic_cast<ExprInitValAST*>(initVal)) {
            llvm::Value* value = generateExpr(exprInit->getExpr());
            if (value->getType() != targetType) {
                throw std::runtime_error("Vector initializer must be a vector value");
            }
            if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                if (!llvm::isa<llvm::Constant>(value)) {
                    throw std::runtime_error("Global vector initializer must be a constant");
                }
                globalVar->setInitializer(llvm::cast<llvm::Constant>(value));
            } else {
                builder.CreateStore(value, ptr);
            }
            return;
        }

        // 列表初始化
        if (auto listInit = dynamic_cast<ListInitValAST*>(initVal)) {
            const auto& initVals = listInit->getInitVals();
            if (initVals.size() > vecSize) {
                throw std::runtime_error("Vector initializer has too many elements");
            }

            if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                std::vector<llvm::Constant*> elements;
                elements.reserve(vecSize);
                for (unsigned i = 0; i < vecSize; ++i) {
                    llvm::Constant* elemConst = llvm::Constant::getNullValue(elemType);
                    if (i < initVals.size()) {
                        auto exprVal = dynamic_cast<ExprInitValAST*>(initVals[i].get());
                        if (!exprVal) {
                            throw std::runtime_error("Vector initializer elements must be expressions");
                        }
                        llvm::Value* val = generateExpr(exprVal->getExpr());
                        if (!llvm::isa<llvm::Constant>(val)) {
                            throw std::runtime_error("Global vector initializer must be a constant");
                        }
                        llvm::Constant* cval = llvm::cast<llvm::Constant>(val);
                        if (cval->getType() != elemType) {
                            if (elemType->isIntegerTy() && cval->getType()->isFloatTy()) {
                                cval = llvm::ConstantExpr::getFPToSI(cval, elemType);
                            } else if (elemType->isFloatTy() && cval->getType()->isIntegerTy()) {
                                cval = llvm::ConstantExpr::getSIToFP(cval, elemType);
                            } else {
                                throw std::runtime_error("Type mismatch in vector initializer");
                            }
                        }
                        elemConst = cval;
                    }
                    elements.push_back(elemConst);
                }
                globalVar->setInitializer(llvm::ConstantVector::get(elements));
            } else {
                llvm::Value* vecValue = llvm::UndefValue::get(vecType);
                for (unsigned i = 0; i < vecSize; ++i) {
                    llvm::Value* elemValue = llvm::Constant::getNullValue(elemType);
                    if (i < initVals.size()) {
                        auto exprVal = dynamic_cast<ExprInitValAST*>(initVals[i].get());
                        if (!exprVal) {
                            throw std::runtime_error("Vector initializer elements must be expressions");
                        }
                        elemValue = generateExpr(exprVal->getExpr());
                        if (elemValue->getType() != elemType) {
                            if (elemType->isIntegerTy() && elemValue->getType()->isFloatTy()) {
                                elemValue = builder.CreateFPToSI(elemValue, elemType, "fptosi");
                            } else if (elemType->isFloatTy() && elemValue->getType()->isIntegerTy()) {
                                elemValue = builder.CreateSIToFP(elemValue, elemType, "sitofp");
                            } else {
                                throw std::runtime_error("Type mismatch in vector initializer");
                            }
                        }
                    }
                    vecValue = builder.CreateInsertElement(
                        vecValue,
                        elemValue,
                        llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i),
                        "vecins"
                    );
                }
                builder.CreateStore(vecValue, ptr);
            }
            return;
        }

        throw std::runtime_error("Unknown vector initializer type");
    }
    
    // 处理表达式初始化值
    if (auto exprInit = dynamic_cast<ExprInitValAST*>(initVal)) {
        // 单个表达式，直接设置值
        llvm::Value* value = generateExpr(exprInit->getExpr());
        
        // 对于全局变量，检查是否是常量
        if (llvm::isa<llvm::GlobalVariable>(ptr)) {
            if (!llvm::isa<llvm::Constant>(value)) {
                throw std::runtime_error("Global variable initializer must be a constant");
            }
        }
        
        // 检查类型匹配
        llvm::Type* varType;
        if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
            varType = allocaInst->getAllocatedType();
        } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
            varType = globalVar->getValueType();
        } else {
            // 对于其他情况，尝试通过上下文推断类型
            // 如果是GEP指令的结果，可以尝试从操作数推断
            if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
                varType = gepInst->getResultElementType();
            } else {
                // 如果无法推断，抛出错误提示
                throw std::runtime_error("Unable to determine pointer element type");
            }
        }
        if (value->getType() != varType) {
            // 对于全局变量，不允许类型转换
            if (llvm::isa<llvm::GlobalVariable>(ptr)) {
                throw std::runtime_error("Type mismatch in global variable initializer");
            }
            // 对于局部变量，允许类型转换
            if (varType->isIntegerTy() && value->getType()->isFloatTy()) {
                // 浮点数转整数
                value = builder.CreateFPToSI(value, varType, "fptosi");
            } else if (varType->isFloatTy() && value->getType()->isIntegerTy()) {
                // 整数转浮点数
                value = builder.CreateSIToFP(value, varType, "sitofp");
            } else {
                throw std::runtime_error("Type mismatch in variable initializer");
            }
        }

        // 设置初始化值
        if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
            globalVar->setInitializer(llvm::cast<llvm::Constant>(value));
        } else {
            // 局部变量可以使用变量表达式初始化
            builder.CreateStore(value, ptr);
        }
    }
    // 处理列表初始化值（数组）
    else if (auto listInit = dynamic_cast<ListInitValAST*>(initVal)) {
        // 如果是空列表，表示所有元素初始化为0
        if (listInit->getInitVals().empty()) {
            llvm::Type* elementType;
            if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
                elementType = allocaInst->getAllocatedType();
            } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                elementType = globalVar->getValueType();
            } else {
                // 对于其他情况，尝试通过上下文推断类型
                if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
                    elementType = gepInst->getResultElementType();
                } else {
                    throw std::runtime_error("Unable to determine pointer element type");
                }
            }
            llvm::Constant* zero = llvm::Constant::getNullValue(elementType);
            
            if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                // 如果是全局变量，需要计算整个数组的零初始化值
                if (dimensions > 0) {
                    // 计算总元素数
                    int totalElements = 1;
                    for (int i = 0; i < dimensions; i++) {
                        totalElements *= sizes[i];
                    }
                    
                    // 创建零初始化的数组
                    llvm::Type* elementType;
                    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
                        elementType = allocaInst->getAllocatedType();
                    } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                        elementType = globalVar->getValueType();
                    } else {
                        // 对于其他情况，尝试通过上下文推断类型
                        if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
                            elementType = gepInst->getResultElementType();
                        } else {
                            throw std::runtime_error("Unable to determine pointer element type");
                        }
                    }
                    while (llvm::isa<llvm::ArrayType>(elementType)) {
                        elementType = llvm::cast<llvm::ArrayType>(elementType)->getElementType();
                    }
                    
                    llvm::Constant* zeroElement = llvm::Constant::getNullValue(elementType);
                    std::vector<llvm::Constant*> zeroElements(totalElements, zeroElement);
                    
                    // 构建多维数组的零初始化
                    llvm::Type* arrayType;
                    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
                        arrayType = allocaInst->getAllocatedType();
                    } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
                        arrayType = globalVar->getValueType();
                    } else {
                        // 对于其他情况，尝试通过上下文推断类型
                        if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
                            arrayType = gepInst->getResultElementType();
                        } else {
                            throw std::runtime_error("Unable to determine pointer element type");
                        }
                    }
                    llvm::Constant* zeroArray = llvm::ConstantArray::get(
                        llvm::cast<llvm::ArrayType>(arrayType),
                        zeroElements
                    );
                    
                    globalVar->setInitializer(zeroArray);
                } else {
                    globalVar->setInitializer(zero);
                }
            } else {
                // 如果是局部变量，使用memset或循环初始化
                // 这里简化处理，实际可能需要更复杂的逻辑
                builder.CreateStore(zero, ptr);
            }
            return;
        }
        
        // 处理非空列表
        if (dimensions == 0) {
            // 没有维度，但是有列表，这是错误的
            throw std::runtime_error("Scalar initializer cannot be a list");
        }
        
        
        // 获取数组类型
        llvm::Type* arrayType;
        if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
            arrayType = allocaInst->getAllocatedType();
        } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
            arrayType = globalVar->getValueType();
        } else {
            // 对于其他情况，尝试通过上下文推断类型
            if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(ptr)) {
                arrayType = gepInst->getResultElementType();
            } else {
                throw std::runtime_error("Unable to determine pointer element type");
            }
        }
        
        // 计算数组元素类型
        llvm::Type* elementType = arrayType;
        while (llvm::isa<llvm::ArrayType>(elementType)) {
            elementType = llvm::cast<llvm::ArrayType>(elementType)->getElementType();
        }
        
        // 计算总元素数
        int totalElements = 1;
        for (int i = 0; i < dimensions; i++) {
            totalElements *= sizes[i];
        }
        
        // 生成全局变量初始化值
        if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(ptr)) {
            // 递归构建多维数组初始化值
            std::function<llvm::Constant*(llvm::Type*, const std::vector<int>&, const std::vector<std::unique_ptr<InitValAST>>&, size_t&)> buildArrayInit;
            buildArrayInit = [&](llvm::Type* type, const std::vector<int>& dims, const std::vector<std::unique_ptr<InitValAST>>& initVals, size_t& index) -> llvm::Constant* {
                if (dims.empty()) {         // 到达最内层元素，返回单个初始化值
                    // 到达最内层元素
                    if (index < initVals.size()) {
                        if (auto exprVal = dynamic_cast<ExprInitValAST*>(initVals[index].get())) {
                            llvm::Value* val = generateExpr(exprVal->getExpr());
                            if (!llvm::isa<llvm::Constant>(val)) {
                                throw std::runtime_error("Global variable initializer must be a constant");
                            }
                            if (val->getType() != elementType) {
                                throw std::runtime_error("Type mismatch in global variable initializer");
                            }
                            index++;
                            return llvm::cast<llvm::Constant>(val);
                        }
                    }
                    return llvm::Constant::getNullValue(elementType);
                }
                
                // 处理当前维度
                std::vector<llvm::Constant*> subInits;
                int currentDimSize = dims[0];
                std::vector<int> subDims(dims.begin() + 1, dims.end());
                
                for (int i = 0; i < currentDimSize; i++) {
                    if (index < initVals.size()) {
                        if (auto listVal = dynamic_cast<ListInitValAST*>(initVals[index].get())) {
                            // 处理嵌套列表初始化
                            size_t subIndex = 0;
                            llvm::Constant* subInit = buildArrayInit(
                                llvm::cast<llvm::ArrayType>(type)->getElementType(),
                                subDims,
                                listVal->getInitVals(),
                                subIndex
                            );
                            subInits.push_back(subInit);
                            index++;
                        } else {
                            // 处理扁平化初始化
                            llvm::Constant* subInit = buildArrayInit(
                                llvm::cast<llvm::ArrayType>(type)->getElementType(),
                                subDims,
                                initVals,
                                index
                            );
                            subInits.push_back(subInit);
                        }
                    } else {
                        // 填充默认值
                        size_t dummyIndex = 0;
                        llvm::Constant* subInit = buildArrayInit(
                            llvm::cast<llvm::ArrayType>(type)->getElementType(),
                            subDims,
                            std::vector<std::unique_ptr<InitValAST>>(),
                            dummyIndex
                        );
                        subInits.push_back(subInit);
                    }
                }
                
                return llvm::ConstantArray::get(
                    llvm::cast<llvm::ArrayType>(type),
                    subInits
                );
            };
            
            size_t index = 0;
            llvm::Constant* arrayInit = buildArrayInit(arrayType, sizes, listInit->getInitVals(), index);
            globalVar->setInitializer(arrayInit);
        } else {
            // 局部变量初始化
            // 递归初始化多维数组元素
            std::function<void(llvm::Value*, llvm::Type*, const std::vector<int>&, const std::vector<std::unique_ptr<InitValAST>>&, size_t&)> initArrayElements;
            initArrayElements = [&](llvm::Value* basePtr, llvm::Type* type, const std::vector<int>& dims, const std::vector<std::unique_ptr<InitValAST>>& initVals, size_t& index) {
                if (dims.empty()) {
                    // 到达最内层元素
                    if (index < initVals.size()) {
                        if (auto exprVal = dynamic_cast<ExprInitValAST*>(initVals[index].get())) {
                            llvm::Value* val = generateExpr(exprVal->getExpr());
                            if (val->getType() != elementType) {
                                // 允许类型转换
                                if (elementType->isIntegerTy() && val->getType()->isFloatTy()) {
                                    val = builder.CreateFPToSI(val, elementType, "fptosi");
                                } else if (elementType->isFloatTy() && val->getType()->isIntegerTy()) {
                                    val = builder.CreateSIToFP(val, elementType, "sitofp");
                                } else {
                                    throw std::runtime_error("Type mismatch in array initializer");
                                }
                            }
                            builder.CreateStore(val, basePtr);
                            index++;
                        }
                    }
                    return;
                }
                
                // 处理当前维度
                int currentDimSize = dims[0];
                std::vector<int> subDims(dims.begin() + 1, dims.end());
                
                for (int i = 0; i < currentDimSize; i++) {
                    std::vector<llvm::Value*> indices;
                    indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));
                    indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i));
                    
                    llvm::Value* elementPtr = builder.CreateGEP(type, basePtr, indices, "elementptr");
                    
                    if (index < initVals.size()) {
                        if (auto listVal = dynamic_cast<ListInitValAST*>(initVals[index].get())) {
                            // 处理嵌套列表初始化
                            size_t subIndex = 0;
                            initArrayElements(
                                elementPtr,
                                llvm::cast<llvm::ArrayType>(type)->getElementType(),
                                subDims,
                                listVal->getInitVals(),
                                subIndex
                            );
                            index++;
                        } else {
                            // 处理扁平化初始化
                            initArrayElements(
                                elementPtr,
                                llvm::cast<llvm::ArrayType>(type)->getElementType(),
                                subDims,
                                initVals,
                                index
                            );
                        }
                    } else {
                        // 填充默认值
                        size_t dummyIndex = 0;
                        initArrayElements(
                            elementPtr,
                            llvm::cast<llvm::ArrayType>(type)->getElementType(),
                            subDims,
                            std::vector<std::unique_ptr<InitValAST>>(),
                            dummyIndex
                        );
                    }
                }
            };
            
            size_t index = 0;
            initArrayElements(ptr, arrayType, sizes, listInit->getInitVals(), index);
        }
    } else {
        throw std::runtime_error("Unknown initializer type");
    }
}

// 生成左值表达式的 IR
llvm::Value* IRGenerator::generateLVal(LValExprAST* lval) {
    
    if (!lval) {
        throw std::runtime_error("LVal is null");
    }
    const std::string& varName = lval->getName();
    
    // 查找变量
    auto symOpt = lookupSymbol(varName);
    if (!symOpt) {
        throw std::runtime_error("Variable '" + varName + "' not defined");
    }
    
    // 立即获取需要的信息到局部变量
    SymbolInfo symInfo = symOpt.value();
    llvm::Value* varPtr = symInfo.value;
    bool isArray = symInfo.isArray;
    llvm::Value* loadedArrayPtr = symInfo.loadedArrayPtr;
    llvm::Type* arrayElementType = symInfo.arrayElementType;
    
    // 首先检查是否是数组参数，如果是则直接使用预加载指针
    llvm::Type* allocatedType = nullptr;
    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(varPtr)) {
        allocatedType = allocaInst->getAllocatedType();
    } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(varPtr)) {
        allocatedType = globalVar->getValueType();
    }
    
    bool isArrayParam = isArray && allocatedType && allocatedType->isPointerTy();
    if (isArrayParam) {
        if (!loadedArrayPtr) {
            throw std::runtime_error("Array parameter pointer not preloaded");
        }
        
        
        if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(loadedArrayPtr)) {
            // 检查这个 load 指令是否在当前函数中
            if (loadInst->getParent()->getParent() != currentFunction) {
                throw std::runtime_error(
                    "Array parameter '" + varName + "' has loadedArrayPtr from another function. "
                    "This indicates a symbol table scope management error."
                );
            }
        }
    
        
        // 如果是数组参数，直接使用预加载指针
        llvm::Value* basePtr = loadedArrayPtr;
        llvm::Type* baseType = arrayElementType;
        
        // 如果是数组访问
        if (!lval->getIndices().empty()) {
            // 计算数组的维度
            int arrayDimensions = 0;
            
            // 对于维度计算部分，使用保存的类型信息
            llvm::Type* dimCalcType = baseType;
            
            llvm::Type* currentType = dimCalcType;
            
            // 计算数组维度
            while (currentType->isArrayTy()) {
                arrayDimensions++;
                currentType = currentType->getArrayElementType();
            }
            
            // 对于数组参数，实际可访问的维度
            int accessibleDimensions = arrayDimensions + 1;

            // 检查索引数量
            // 允许部分索引访问（如 arr2d[1] 访问二维数组的第一维）
            if (static_cast<int>(lval->getIndices().size()) > accessibleDimensions) {
                throw std::runtime_error("Array index count exceeds array dimensions");
            }
            
            // 创建GEP指令
            std::vector<llvm::Value*> indices;
            
            // 添加用户提供的索引
            for (const auto& idx : lval->getIndices()) {
                llvm::Value* indexValue = generateExpr(idx.get());
                indices.push_back(indexValue);
            }

            // 创建GEP指令
            llvm::Value* elementPtr = builder.CreateGEP(baseType, basePtr, indices, "arrayptr");

            // 确定要加载的类型
            llvm::Type* loadType = baseType;
            // 如果是多维数组，需要找到最终的元素类型
            while (loadType->isArrayTy()) {
                loadType = loadType->getArrayElementType();
            }

            return builder.CreateLoad(loadType, elementPtr, "loadtmp");
        } else {
            // 无索引访问，直接返回预加载的指针
            return basePtr;
        }
    }

    // 非数组参数的处理
    // 如果是数组访问
    if (!lval->getIndices().empty()) {
        if (allocatedType && allocatedType->isVectorTy()) {
            if (lval->getIndices().size() != 1) {
                throw std::runtime_error("Vector index must be one-dimensional");
            }
            llvm::Value* vecValue = builder.CreateLoad(allocatedType, varPtr, "vecload");
            llvm::Value* indexValue = generateExpr(lval->getIndices()[0].get());
            if (!indexValue->getType()->isIntegerTy()) {
                throw std::runtime_error("Vector index must be integer");
            }
            if (indexValue->getType() != llvm::Type::getInt32Ty(context)) {
                indexValue = builder.CreateIntCast(indexValue, llvm::Type::getInt32Ty(context), true, "vidxcast");
            }
            return builder.CreateExtractElement(vecValue, indexValue, "vecelem");
        }
        // 获取变量的存储类型
        if (!allocatedType) {
            throw std::runtime_error("Unable to determine variable type");
        }

        llvm::Type* baseType = allocatedType;
        llvm::Value* basePtr = varPtr;

        // 计算数组的维度
        int arrayDimensions = 0;
        
        llvm::Type* currentType = baseType;
        
        // 计算数组维度
        while (currentType->isArrayTy()) {
            arrayDimensions++;
            currentType = currentType->getArrayElementType();
        }
        
        // 检查索引数量
        if (static_cast<int>(lval->getIndices().size()) > arrayDimensions) {
            throw std::runtime_error("Array index count exceeds array dimensions");
        }

        // 创建GEP指令
        std::vector<llvm::Value*> indices;
        
        // 对于非数组参数（真正的数组），需要第一个索引0
        indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));

        // 添加用户提供的索引
        for (const auto& idx : lval->getIndices()) {
            llvm::Value* indexValue = generateExpr(idx.get());
            indices.push_back(indexValue);
        }

        // 创建GEP指令
        llvm::Value* elementPtr = builder.CreateGEP(baseType, basePtr, indices, "arrayptr");

        // 确定要加载的类型
        llvm::Type* loadType = currentType;

        return builder.CreateLoad(loadType, elementPtr, "loadtmp");
    } else {
        // 无索引访问
        if (!allocatedType) {
            throw std::runtime_error("Unable to determine load type");
        }

        // 如果是数组，返回指针而不是加载整个数组
        if (isArray) {
            // 数组名直接转换为指针
            return builder.CreateBitCast(varPtr, llvm::PointerType::get(allocatedType->getArrayElementType(), 0), "arraycast");
        } else {
            // 对于普通变量，正常加载
            return builder.CreateLoad(allocatedType, varPtr, "loadtmp");
        }
    }
}

// 生成函数调用表达式的 IR
llvm::Value* IRGenerator::generateCallExpr(CallExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("CallExpr is null");
    }

    // 获取函数名
    std::string funcName = expr->getCallee();

    // 内建：向量求和 vsum(vector)
    if (funcName == "vsum") {
        if (expr->getArgs().size() != 1) {
            throw std::runtime_error("vsum expects exactly one argument");
        }
        llvm::Value* vecValue = generateExpr(expr->getArgs()[0].get());
        auto vecType = llvm::dyn_cast<llvm::VectorType>(vecValue->getType());
        if (!vecType) {
            throw std::runtime_error("vsum expects a vector argument");
        }
        auto fixedVecType = llvm::dyn_cast<llvm::FixedVectorType>(vecType);
        if (!fixedVecType) {
            throw std::runtime_error("vsum only supports fixed-length vectors");
        }

        llvm::Type* elemType = vecType->getElementType();
        unsigned numElems = fixedVecType->getNumElements();
        llvm::Value* acc = nullptr;

        if (elemType->isFloatingPointTy()) {
            acc = llvm::ConstantFP::get(elemType, 0.0);
            for (unsigned i = 0; i < numElems; ++i) {
                llvm::Value* idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i);
                llvm::Value* elem = builder.CreateExtractElement(vecValue, idx, "vsum.elem");
                acc = builder.CreateFAdd(acc, elem, "vsum.acc");
            }
        } else if (elemType->isIntegerTy()) {
            acc = llvm::ConstantInt::get(elemType, 0, true);
            for (unsigned i = 0; i < numElems; ++i) {
                llvm::Value* idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i);
                llvm::Value* elem = builder.CreateExtractElement(vecValue, idx, "vsum.elem");
                acc = builder.CreateAdd(acc, elem, "vsum.acc");
            }
        } else {
            throw std::runtime_error("vsum only supports int/float element vectors");
        }

        return acc;
    }
    
    // 检查是否是starttime或stoptime函数调用
    bool isStartTimeCall = (funcName == "starttime");
    bool isStopTimeCall = (funcName == "stoptime");
    
    // 获取函数（对于starttime和stoptime，实际使用的是_sysy_starttime和_sysy_stoptime）
    std::string actualFuncName = funcName;
    if (isStartTimeCall) {
        actualFuncName = "_sysy_starttime";
    } else if (isStopTimeCall) {
        actualFuncName = "_sysy_stoptime";
    }
    
    llvm::Function* calleeFunc = module->getFunction(actualFuncName);
    
    // 如果函数不存在，检查是否是库函数
    if (!calleeFunc) {
        if (isLibraryFunction(funcName)) {
            // 库函数已经在declareLibraryFunctions中声明，应该能找到
            throw std::runtime_error("Library function '" + funcName + "' not properly declared");
        } else {
            throw std::runtime_error("Unknown function referenced: " + funcName);
        }
    }

    // 检查参数数量
    // 对于可变参数函数，只检查固定参数的数量
    auto libFuncIt = libraryFunctions.find(funcName);
    
    // 对于starttime和stoptime，忽略原参数检查，因为我们会自动添加行号参数
    // 对于其他函数，正常检查参数数量
    if (!(isStartTimeCall || isStopTimeCall)) {
        if (libFuncIt != libraryFunctions.end() && libFuncIt->second.isVariadic) {
            // 可变参数函数，检查参数数量是否不少于固定参数数量
            if (calleeFunc->arg_size() > expr->getArgs().size()) {
                throw std::runtime_error("Insufficient arguments passed to variadic function: " + funcName);
            }
        } else {
            // 非可变参数函数，检查参数数量是否完全匹配
            if (calleeFunc->arg_size() != expr->getArgs().size()) {
                throw std::runtime_error("Incorrect number of arguments passed to function: " + funcName);
            }
        }
    }

    // 准备参数
    std::vector<llvm::Value*> args;
    
    // 对于starttime和stoptime，添加行号参数
    if (isStartTimeCall || isStopTimeCall) {
        // 获取当前行号
        int lineNumber = expr->getLineNumber();
        
        // 创建行号常量
        llvm::Value* lineArg = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(context),
            lineNumber,
            true  // 有符号整数
        );
        
        // 添加行号参数
        args.push_back(lineArg);
    } else {
        for (size_t i = 0; i < expr->getArgs().size(); ++i) {
        llvm::Value* argValue = generateExpr(expr->getArgs()[i].get());

        // 对于非可变参数函数，检查参数类型
        if (libFuncIt == libraryFunctions.end() || !libFuncIt->second.isVariadic) {
            // 检查参数类型
            if (i < calleeFunc->arg_size()) {
                llvm::Type* expectedType = calleeFunc->getArg(i)->getType();
                if (argValue->getType() != expectedType) {
                    // 如果期望的是指针类型，且当前值是数组或指针，尝试转换
                    if (expectedType->isPointerTy()) {
                        // 如果是左值表达式，获取其地址而不是值
                        if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr->getArgs()[i].get())) {
                            // 生成左值地址
                            argValue = generateLValAddress(lvalExpr);
                        } else {
                            // 如果期望的是i8*（字符串指针），且当前是字符串字面量
                            if (expectedType->isPointerTy() && 
                                expectedType == llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0) && 
                                argValue->getType()->isPointerTy() && 
                                argValue->getType() == expectedType) {
                                // 字符串字面量已经是i8*类型，不需要转换
                                // 但需要确保类型匹配
                                if (argValue->getType() != expectedType) {
                                    argValue = builder.CreateBitCast(argValue, expectedType, "strcast");
                                }
                            } else {
                                throw std::runtime_error("Type mismatch in function argument " + std::to_string(i));
                            }
                        }
                    } else {
                        // 尝试类型转换
                        if (expectedType->isIntegerTy() && argValue->getType()->isFloatTy()) {
                            // 浮点数转整数
                            argValue = builder.CreateFPToSI(argValue, expectedType, "fptosi");
                        } else if (expectedType->isFloatTy() && argValue->getType()->isIntegerTy()) {
                            // 整数转浮点数
                            argValue = builder.CreateSIToFP(argValue, expectedType, "sitofp");
                        } else {
                            throw std::runtime_error("Type mismatch in function argument " + std::to_string(i));
                        }
                    }
                }
            }
        } else {
            // 对于可变参数函数（如putf），需要特殊处理字符串和数组参数
            if (i == 0 && libFuncIt->second.name == "putf") {
                // putf的第一个参数必须是字符串
                if (!argValue->getType()->isPointerTy() || 
                    argValue->getType() != llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)) {
                    throw std::runtime_error("First argument to putf must be a string");
                }
                // 确保类型是i8*
                if (argValue->getType() != llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)) {
                    argValue = builder.CreateBitCast(argValue, 
                        llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), "strcast");
                }
            }
            // 对于putarray函数，第二个参数必须是数组指针
            if (i == 1 && libFuncIt->second.name == "putarray") {
                // 如果是左值表达式，获取其地址而不是值
                if (auto lvalExpr = dynamic_cast<LValExprAST*>(expr->getArgs()[i].get())) {
                    // 生成左值地址
                    argValue = generateLValAddress(lvalExpr);
                } else {
                    throw std::runtime_error("Second argument to putarray must be an array");
                }
                // 确保类型是int*
                if (argValue->getType() != llvm::PointerType::get(llvm::Type::getInt32Ty(context), 0)) {
                    argValue = builder.CreateBitCast(argValue, 
                        llvm::PointerType::get(llvm::Type::getInt32Ty(context), 0), "arraycast");
                }
            }
            // 对于可变参数函数，float参数必须提升为double（C语言调用约定）
            if (argValue->getType()->isFloatTy()) {
                argValue = builder.CreateFPExt(argValue, llvm::Type::getDoubleTy(context), "fpext");
            }
        }

        args.push_back(argValue);
        }
    }

    // 创建函数调用
    // 对于void函数，不要为调用指令生成名称
    if (calleeFunc->getReturnType()->isVoidTy()) {
        return builder.CreateCall(calleeFunc, args);
    } else {
        return builder.CreateCall(calleeFunc, args, "calltmp");
    }
}

// 生成左值地址的 IR
llvm::Value* IRGenerator::generateLValAddress(LValExprAST* lval) {
    if (!lval) {
        throw std::runtime_error("LValExpr is null");
    }

    const std::string& varName = lval->getName();

    // 查找变量
    auto symOpt = lookupSymbol(varName);
    if (!symOpt) {
        throw std::runtime_error("Variable '" + varName + "' not defined");
    }

    // 立即获取需要的信息到局部变量
    SymbolInfo symInfo = symOpt.value();

    // 检查是否是常量赋值
    if (symInfo.isConst) {
        throw std::runtime_error("Cannot assign to constant '" + varName + "'");
    }

    // 检查是否是数组名直接赋值（没有使用索引）
    // 注意：函数调用中的数组参数传递是合法的，真正的赋值检查在赋值语句中进行
    if (symInfo.isArray && lval->getIndices().empty()) {
        // 这里不进行赋值检查，因为真正的数组名直接赋值检查在赋值语句中处理
        // 函数调用中的数组参数传递是合法的，应该允许
    }

    llvm::Value* varPtr = symInfo.value;
    bool isArray = symInfo.isArray;
    llvm::Value* loadedArrayPtr = symInfo.loadedArrayPtr;
    llvm::Type* arrayElementType = symInfo.arrayElementType;

    // 首先检查是否是数组参数，如果是则直接使用预加载指针
    llvm::Type* allocatedType = nullptr;
    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(varPtr)) {
        allocatedType = allocaInst->getAllocatedType();
    } else if (auto globalVar = llvm::dyn_cast<llvm::GlobalVariable>(varPtr)) {
        allocatedType = globalVar->getValueType();
    }
    
    bool isArrayParam = isArray && allocatedType && allocatedType->isPointerTy();
    if (isArrayParam) {
        if (!loadedArrayPtr) {
            throw std::runtime_error("Array parameter pointer not preloaded");
        }
        
        // 如果是数组参数，直接使用预加载指针
        llvm::Value* basePtr = loadedArrayPtr;
        llvm::Type* baseType = arrayElementType;
        
        // 如果是数组访问
        if (!lval->getIndices().empty()) {
            // 计算数组的维度
            int arrayDimensions = 0;
            
            // 对于维度计算部分，使用保存的类型信息
            llvm::Type* dimCalcType = baseType;
            
            llvm::Type* currentType = dimCalcType;
            
            // 计算数组维度
            while (currentType->isArrayTy()) {
                arrayDimensions++;
                currentType = currentType->getArrayElementType();
            }
            
            // 对于数组参数，实际可访问的维度
            int accessibleDimensions = arrayDimensions + 1;

            // 检查索引数量
            // 允许部分索引访问（如 arr2d[1] 访问二维数组的第一维）
            if (static_cast<int>(lval->getIndices().size()) > accessibleDimensions) {
                throw std::runtime_error("Array index count exceeds array dimensions");
            }
            
            // 创建GEP指令
            std::vector<llvm::Value*> indices;
            
            // 添加用户提供的索引
            for (const auto& idx : lval->getIndices()) {
                llvm::Value* indexValue = generateExpr(idx.get());
                indices.push_back(indexValue);
            }

            // 创建GEP指令并返回地址
            return builder.CreateGEP(baseType, basePtr, indices, "arrayptr");
        } else {
            // 无索引访问，直接返回预加载的指针
            return basePtr;
        }
    }

    // 非数组参数的处理
    // 如果是数组访问
    if (!lval->getIndices().empty()) {
        if (allocatedType && allocatedType->isVectorTy()) {
            if (lval->getIndices().size() != 1) {
                throw std::runtime_error("Vector index must be one-dimensional");
            }
            llvm::Value* vecValue = builder.CreateLoad(allocatedType, varPtr, "vecload");
            llvm::Value* indexValue = generateExpr(lval->getIndices()[0].get());
            if (!indexValue->getType()->isIntegerTy()) {
                throw std::runtime_error("Vector index must be integer");
            }
            if (indexValue->getType() != llvm::Type::getInt32Ty(context)) {
                indexValue = builder.CreateIntCast(indexValue, llvm::Type::getInt32Ty(context), true, "vidxcast");
            }

            llvm::Value* elem = builder.CreateExtractElement(vecValue, indexValue, "vecelem");
            llvm::Type* elemType = llvm::cast<llvm::VectorType>(allocatedType)->getElementType();
            llvm::AllocaInst* tmp = builder.CreateAlloca(elemType, nullptr, "vec.elem.tmp");
            builder.CreateStore(elem, tmp);
            return tmp;
        }
        // 获取变量的存储类型
        if (!allocatedType) {
            throw std::runtime_error("Unable to determine variable type");
        }

        llvm::Type* baseType = allocatedType;
        llvm::Value* basePtr = varPtr;

        // 计算数组的维度
        int arrayDimensions = 0;
        
        llvm::Type* currentType = baseType;
        
        // 计算数组维度
        while (currentType->isArrayTy()) {
            arrayDimensions++;
            currentType = currentType->getArrayElementType();
        }
        
        // 检查索引数量
        if (static_cast<int>(lval->getIndices().size()) > arrayDimensions) {
            throw std::runtime_error("Array index count exceeds array dimensions");
        }

        // 创建GEP指令
        std::vector<llvm::Value*> indices;
        
        // 对于非数组参数（真正的数组），需要第一个索引0
        indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));

        // 添加用户提供的索引
        for (const auto& idx : lval->getIndices()) {
            llvm::Value* indexValue = generateExpr(idx.get());
            indices.push_back(indexValue);
        }

        // 创建GEP指令并返回地址
        return builder.CreateGEP(baseType, basePtr, indices, "arrayptr");
    }

    // 非数组参数，无索引访问，直接返回变量指针
    return varPtr;
}

// 生成完整程序的 IR
std::unique_ptr<llvm::Module> IRGenerator::generate(CompUnitAST* compUnit) {
    if (!compUnit) {
        throw std::runtime_error("CompUnit is null");
    }

    // 创建新模块
    module = std::make_unique<llvm::Module>("SysY_Module", context);

    // 初始化作用域栈
    symbolTableStack.clear();
    pushScope();  // 创建全局作用域
    
    // 声明库函数
    declareLibraryFunctions();
    
    // 用于检查main函数是否存在且唯一
    bool hasMainFunc = false;
    
    // 处理全局声明
    for (const auto& decl : compUnit->getDecls()) {
        generateDecl(decl.get());
    }
    
    // 处理函数定义
    for (const auto& func : compUnit->getFunctions()) {
        // 检查是否是main函数
        if (func->getName() == "main") {
            if (hasMainFunc) {
                throw std::runtime_error("Multiple main functions defined");
            }
            // 检查main函数是否符合要求：无参数、返回类型为int
            if (func->getReturnType()->getKind() != TypeAST::Kind::INT || !func->getParams().empty()) {
                throw std::runtime_error("main function must have no parameters and return int");
            }
            hasMainFunc = true;
        }
        generateFunction(func.get());
    }
    
    // 检查是否有main函数
    if (!hasMainFunc) {
        throw std::runtime_error("No main function defined");
    }

    // 验证整个模块
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyModule(*module, &errorStream)) {
        errorStream.flush();
        throw std::runtime_error("Module verification failed: " + errorMsg);
    }
    
    // 退出全局作用域
    popScope();
    
    return std::move(module);
}

// 声明库函数
void IRGenerator::declareLibraryFunctions() {
    // 输入函数声明
    declareGetintFunction();
    declareGetchFunction();
    declareGetfloatFunction();
    declareGetarrayFunction();
    declareGetfarrayFunction();
    
    // 输出函数声明
    declarePutintFunction();
    declarePutchFunction();
    declarePutfloatFunction();
    declarePutarrayFunction();
    declarePutfarrayFunction();
    declarePutfFunction();
    
    // 计时函数声明
    declareStarttimeFunction();
    declareStoptimeFunction();
}

// 声明putf函数
void IRGenerator::declarePutfFunction() {
    // putf函数签名: void putf(const char* format, ...)
    // 这是一个可变参数函数，第一个参数是格式字符串
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 第一个参数是i8* (char*)
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
    
    // 创建函数类型: void (i8*, ...)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                      // 参数类型
        true                             // 可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putf",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putf"] = LibraryFunction("putf", funcType, true);
}

// 声明putch函数
void IRGenerator::declarePutchFunction() {
    // putch函数签名: void putch(int ch)
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 参数是int
    paramTypes.push_back(llvm::Type::getInt32Ty(context));
    
    // 创建函数类型: void (int)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                      // 参数类型
        false                            // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putch",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putch"] = LibraryFunction("putch", funcType, false);
}

// 声明putarray函数
void IRGenerator::declarePutarrayFunction() {
    // putarray函数签名: void putarray(int n, int arr[])
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 第一个参数是int
    paramTypes.push_back(llvm::Type::getInt32Ty(context));
    // 第二个参数是int* (int[])
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getInt32Ty(context), 0));
    
    // 创建函数类型: void (int, int*)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                      // 参数类型
        false                            // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putarray",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putarray"] = LibraryFunction("putarray", funcType, false);
}

// 检查是否是库函数
bool IRGenerator::isLibraryFunction(const std::string& name) {
    return libraryFunctions.find(name) != libraryFunctions.end();
}

// 生成字符串字面量
llvm::Value* IRGenerator::generateStringLiteral(StringLiteralExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("StringLiteralExpr is null");
    }
    
    // 创建全局字符串常量
    return createGlobalString(expr->getValue());
}

// 创建全局字符串
llvm::Constant* IRGenerator::createGlobalString(const std::string& str) {
    // 创建字符串常量
    llvm::Constant* strConstant = llvm::ConstantDataArray::getString(context, str, true);
    
    // 为字符串常量创建全局变量
    llvm::GlobalVariable* globalStr = new llvm::GlobalVariable(
        *module,
        strConstant->getType(),
        true,  // 是常量
        llvm::GlobalValue::PrivateLinkage,
        strConstant,
        ".str"  // 变量名前缀
    );
    
    // 创建指向字符串第一个字符的指针 (i8*)
    llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Constant* indices[] = { zero, zero };
    
    return llvm::ConstantExpr::getGetElementPtr(
        strConstant->getType(),
        globalStr,
        indices,
        true
    );
}

// 声明getint函数
void IRGenerator::declareGetintFunction() {
    // getint函数签名: int getint()
    
    // 创建函数类型: int ()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),  // 返回类型: int
        {},                               // 无参数
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "getint",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["getint"] = LibraryFunction("getint", funcType, false);
}

// 声明getch函数
void IRGenerator::declareGetchFunction() {
    // getch函数签名: int getch()
    
    // 创建函数类型: int ()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),  // 返回类型: int
        {},                               // 无参数
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "getch",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["getch"] = LibraryFunction("getch", funcType, false);
}

// 声明getfloat函数
void IRGenerator::declareGetfloatFunction() {
    // getfloat函数签名: float getfloat()
    
    // 创建函数类型: float ()
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getFloatTy(context),  // 返回类型: float
        {},                               // 无参数
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "getfloat",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["getfloat"] = LibraryFunction("getfloat", funcType, false);
}

// 声明getarray函数
void IRGenerator::declareGetarrayFunction() {
    // getarray函数签名: int getarray(int arr[])
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 参数是int* (int[])
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getInt32Ty(context), 0));
    
    // 创建函数类型: int (int*)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),  // 返回类型: int
        paramTypes,                       // 参数类型
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "getarray",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["getarray"] = LibraryFunction("getarray", funcType, false);
}

// 声明getfarray函数
void IRGenerator::declareGetfarrayFunction() {
    // getfarray函数签名: int getfarray(float arr[])
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 参数是float* (float[])
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getFloatTy(context), 0));
    
    // 创建函数类型: int (float*)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),  // 返回类型: int
        paramTypes,                       // 参数类型
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "getfarray",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["getfarray"] = LibraryFunction("getfarray", funcType, false);
}

// 声明putint函数
void IRGenerator::declarePutintFunction() {
    // putint函数签名: void putint(int value)
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 参数是int
    paramTypes.push_back(llvm::Type::getInt32Ty(context));
    
    // 创建函数类型: void (int)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                       // 参数类型
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putint",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putint"] = LibraryFunction("putint", funcType, false);
}

// 声明putfloat函数
void IRGenerator::declarePutfloatFunction() {
    // putfloat函数签名: void putfloat(float value)
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 参数是float
    paramTypes.push_back(llvm::Type::getFloatTy(context));
    
    // 创建函数类型: void (float)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                       // 参数类型
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putfloat",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putfloat"] = LibraryFunction("putfloat", funcType, false);
}

// 声明putfarray函数
void IRGenerator::declarePutfarrayFunction() {
    // putfarray函数签名: void putfarray(int n, float arr[])
    
    // 创建参数类型列表
    std::vector<llvm::Type*> paramTypes;
    // 第一个参数是int
    paramTypes.push_back(llvm::Type::getInt32Ty(context));
    // 第二个参数是float* (float[])
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getFloatTy(context), 0));
    
    // 创建函数类型: void (int, float*)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                       // 参数类型
        false                             // 非可变参数函数
    );
    
    // 创建函数声明
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "putfarray",
        module.get()
    );
    
    // 添加到库函数映射表
    libraryFunctions["putfarray"] = LibraryFunction("putfarray", funcType, false);
}

// 声明_sysy_starttime函数（实际静态库中的函数名）
void IRGenerator::declareStarttimeFunction() {
    // _sysy_starttime函数签名: void _sysy_starttime(int lineno)
    
    // 创建参数类型列表：一个int参数（行号）
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(llvm::Type::getInt32Ty(context));  // 行号参数
    
    // 创建函数类型: void (int)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                       // 参数类型：int（行号）
        false                             // 非可变参数函数
    );
    
    // 创建函数声明（使用正确的函数名）
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "_sysy_starttime",
        module.get()
    );
    
    // 添加到库函数映射表（同时保留原函数名映射，用于处理用户代码中的调用）
    libraryFunctions["starttime"] = LibraryFunction("_sysy_starttime", funcType, false);
    libraryFunctions["_sysy_starttime"] = LibraryFunction("_sysy_starttime", funcType, false);
}

// 声明_sysy_stoptime函数（实际静态库中的函数名）
void IRGenerator::declareStoptimeFunction() {
    // _sysy_stoptime函数签名: void _sysy_stoptime(int lineno)
    
    // 创建参数类型列表：一个int参数（行号）
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(llvm::Type::getInt32Ty(context));  // 行号参数
    
    // 创建函数类型: void (int)
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context),  // 返回类型: void
        paramTypes,                       // 参数类型：int（行号）
        false                             // 非可变参数函数
    );
    
    // 创建函数声明（使用正确的函数名）
    llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "_sysy_stoptime",
        module.get()
    );
    
    // 添加到库函数映射表（同时保留原函数名映射，用于处理用户代码中的调用）
    libraryFunctions["stoptime"] = LibraryFunction("_sysy_stoptime", funcType, false);
    libraryFunctions["_sysy_stoptime"] = LibraryFunction("_sysy_stoptime", funcType, false);
}
