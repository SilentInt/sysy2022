#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ast/ast.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <memory>
#include <map>
#include <string>
#include <functional>
#include <optional>

class IRGenerator {
private:
    // LLVM 核心组件
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    
    // 符号表项结构
    struct SymbolInfo {
        llvm::Value* value;           // 变量的地址或值
        bool isConst;                 // 是否是常量
        bool isArray;                 // 是否是数组
        llvm::Type* arrayElementType; // 数组元素类型
        llvm::Value* loadedArrayPtr;  // 已加载的数组指针（用于数组参数）
      
        SymbolInfo() : value(nullptr), isConst(false), isArray(false), 
                       arrayElementType(nullptr), loadedArrayPtr(nullptr) {}
      
        SymbolInfo(llvm::Value* v, bool c, bool a, llvm::Type* elemType = nullptr)
            : value(v), isConst(c), isArray(a), arrayElementType(elemType), 
              loadedArrayPtr(nullptr) {}
    };
    
    // 符号表栈：支持嵌套作用域
    std::vector<std::map<std::string, SymbolInfo>> symbolTableStack;
    
    // 作用域管理函数
    void pushScope();
    void popScope();
    std::optional<SymbolInfo> lookupSymbol(const std::string& name);
    void addSymbol(const std::string& name, const SymbolInfo& info);
    
    // 当前函数
    llvm::Function* currentFunction;
    
    // 循环上下文（用于break和continue）
    std::vector<llvm::BasicBlock*> breakTargets;
    std::vector<llvm::BasicBlock*> continueTargets;
    
    // 库函数声明和管理
    struct LibraryFunction {
        std::string name;
        llvm::FunctionType* type;
        bool isVariadic;  // 是否为可变参数函数
        
        LibraryFunction() : name(""), type(nullptr), isVariadic(false) {}  // 默认构造函数
        LibraryFunction(const std::string& n, llvm::FunctionType* t, bool var = false)
            : name(n), type(t), isVariadic(var) {}
    };
    
    // 库函数映射表
    std::map<std::string, LibraryFunction> libraryFunctions;
    
    // 检查是否是库函数
    bool isLibraryFunction(const std::string& name);
    
    // 辅助方法
    llvm::Type* getType(TypeAST* typeAST);
    llvm::Value* generateExpr(ExprAST* expr);
    llvm::Value* generateExprForArraySize(ExprAST* expr);
    llvm::Value* generateCondExpr(ExprAST* expr);
    void generateStmt(StmtAST* stmt);
    void generateBlock(BlockAST* block);
    llvm::Function* generateFunction(FunctionAST* func);
    void generateDecl(DeclAST* decl);
    llvm::Value* generateLVal(LValExprAST* lval);
    llvm::Value* generateLValAddress(LValExprAST* lval);
    void generateInitVal(InitValAST* initVal, llvm::Value* ptr, int dimensions, const std::vector<int>& sizes);
    llvm::Value* generateBinaryExpr(BinaryExprAST* expr);
    llvm::Value* generateUnaryExpr(UnaryExprAST* expr);
    llvm::Value* generateCallExpr(CallExprAST* expr);
    
    // 字符串处理
    llvm::Value* generateStringLiteral(StringLiteralExprAST* expr);
    llvm::Constant* createGlobalString(const std::string& str);
    
    // 编译期常量求值函数
    int evaluateConstExpr(ExprAST* expr);

public:
    IRGenerator();
    
    // 生成完整程序的 IR
    std::unique_ptr<llvm::Module> generate(CompUnitAST* compUnit);
    
    // 声明库函数
    void declareLibraryFunctions();
    
    // 输入函数声明
    void declareGetintFunction();
    void declareGetchFunction();
    void declareGetfloatFunction();
    void declareGetarrayFunction();
    void declareGetfarrayFunction();
    
    // 输出函数声明
    void declarePutintFunction();
    void declarePutchFunction();
    void declarePutfloatFunction();
    void declarePutarrayFunction();
    void declarePutfarrayFunction();
    void declarePutfFunction();
    
    // 计时函数声明
    void declareStarttimeFunction();
    void declareStoptimeFunction();
    
    // 获取模块（用于测试）
    llvm::Module* getModule() { return module.get(); }
};

#endif // IR_GENERATOR_H

