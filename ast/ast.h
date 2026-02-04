// ast.h
#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include <iostream>

// 前向声明
class ASTNode;
class ExprAST;
class StmtAST;
class DeclAST;
class FunctionAST;
class CompUnitAST;

// ==================== 基础节点 ====================

// AST 基类
class ASTNode {
private:
    int lineNumber; // 行号信息
protected:
     bool marked_for_deletion = false;  // 用于死代码消除

public:
    ASTNode(int line = -1) : lineNumber(line) {}
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
    
    // 克隆方法，用于函数内联等优化
    virtual std::unique_ptr<ASTNode> clone() const = 0;
    
    // 获取行号
    int getLineNumber() const { return lineNumber; }
    // 设置行号
    void setLineNumber(int line) { lineNumber = line; }
    
    // 标记删除
    void markForDeletion() { marked_for_deletion = true; }
    bool isMarkedForDeletion() const { return marked_for_deletion; }
    
protected:
    void printIndent(int indent) const {
        for (int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
    }
};

// ==================== 表达式节点 ====================

class ExprAST : public ASTNode {
public:
    ExprAST(int line = -1) : ASTNode(line) {}
    virtual ~ExprAST() = default;
    
    // 判断是否为常量表达式
    virtual bool isConstant() const { return false; }
    
    // 获取常量值(如果是常量)
    virtual int getIntValue() const { return 0; }
    virtual float getFloatValue() const { return 0.0f; }
};

// ==================== 类型节点 ====================

class TypeAST : public ASTNode {
public:
    enum class Kind {
        INT,
        FLOAT,
        VOID,
        VECTOR
    };
    
private:
    Kind kind;
    Kind vectorElementKind;
    std::unique_ptr<ExprAST> vectorSizeExpr;
    
public:
    TypeAST(Kind k) : kind(k), vectorElementKind(Kind::INT) {}
    TypeAST(Kind elemKind, std::unique_ptr<ExprAST> sizeExpr)
        : kind(Kind::VECTOR), vectorElementKind(elemKind), vectorSizeExpr(std::move(sizeExpr)) {}
    
    Kind getKind() const { return kind; }
    bool isVector() const { return kind == Kind::VECTOR; }
    Kind getVectorElementKind() const { return vectorElementKind; }
    ExprAST* getVectorSizeExpr() const { return vectorSizeExpr.get(); }
    
    std::string getVectorElementTypeName() const {
        switch (vectorElementKind) {
            case Kind::INT: return "int";
            case Kind::FLOAT: return "float";
            default: return "unknown";
        }
    }
    
    std::string getTypeName() const {
        switch (kind) {
            case Kind::INT: return "int";
            case Kind::FLOAT: return "float";
            case Kind::VOID: return "void";
            case Kind::VECTOR: return "vector";
            default: return "unknown";
        }
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        if (kind == Kind::VECTOR) {
            std::cout << "Type: vector<" << getVectorElementTypeName() << ">" << std::endl;
            if (vectorSizeExpr) {
                printIndent(indent + 1);
                std::cout << "Size:" << std::endl;
                vectorSizeExpr->print(indent + 2);
            }
        } else {
            std::cout << "Type: " << getTypeName() << std::endl;
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        if (kind == Kind::VECTOR) {
            std::unique_ptr<ExprAST> sizeClone;
            if (vectorSizeExpr) {
                sizeClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(vectorSizeExpr->clone().release()));
            }
            return std::make_unique<TypeAST>(vectorElementKind, std::move(sizeClone));
        }
        return std::make_unique<TypeAST>(kind);
    }
};

// 整数字面量
class IntConstExprAST : public ExprAST {
    int value;
    
public:
    IntConstExprAST(int val, int line = -1) : ExprAST(line), value(val) {}
    
    int getValue() const { return value; }
    
    bool isConstant() const override { return true; }
    int getIntValue() const override { return value; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "IntConst: " << value << std::endl;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<IntConstExprAST>(value, getLineNumber());
    }
};

// 浮点数字面量
class FloatConstExprAST : public ExprAST {
    float value;
    
public:
    FloatConstExprAST(float val, int line = -1) : ExprAST(line), value(val) {}
    
    float getValue() const { return value; }
    
    bool isConstant() const override { return true; }
    float getFloatValue() const override { return value; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "FloatConst: " << value << std::endl;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<FloatConstExprAST>(value, getLineNumber());
    }
};

// 左值表达式（变量/数组访问）
class LValExprAST : public ExprAST {
    std::string name;
    std::vector<std::unique_ptr<ExprAST>> indices;  // 数组下标
    
public:
    LValExprAST(const std::string& n, int line = -1) : ExprAST(line), name(n) {}
    
    void addIndex(std::unique_ptr<ExprAST> idx) {
        indices.push_back(std::move(idx));
    }
    
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<ExprAST>>& getIndices() const { return indices; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "LVal: " << name;
        if (!indices.empty()) {
            std::cout << " [" << indices.size() << " dimensions]";
        }
        std::cout << std::endl;
        for (const auto& idx : indices) {
            idx->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<LValExprAST>(name, getLineNumber());
        for (const auto& idx : indices) {
            auto idxClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(idx->clone().release()));
            clone->addIndex(std::move(idxClone));
        }
        return clone;
    }
};

// 二元运算表达式
class BinaryExprAST : public ExprAST {
public:
    enum Operator {
        ADD, SUB, MUL, DIV, MOD,           // 算术运算
        LT, GT, LE, GE,                     // 关系运算
        EQ, NE,                             // 相等性运算
        AND, OR                             // 逻辑运算
    };

private:
    Operator op;
    std::unique_ptr<ExprAST> lhs;
    std::unique_ptr<ExprAST> rhs;

public:
    BinaryExprAST(Operator oper, 
                  std::unique_ptr<ExprAST> left, 
                  std::unique_ptr<ExprAST> right, 
                  int line = -1)
        : ExprAST(line), op(oper), lhs(std::move(left)), rhs(std::move(right)) {}

    Operator getOp() const { return op; }
    ExprAST* getLHS() const { return lhs.get(); }
    ExprAST* getRHS() const { return rhs.get(); }
    
    // 设置左操作数
    void setLHS(std::unique_ptr<ExprAST> newLHS) {
        lhs = std::move(newLHS);
    }
    
    // 设置右操作数
    void setRHS(std::unique_ptr<ExprAST> newRHS) {
        rhs = std::move(newRHS);
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::string opStr;
        switch (op) {
            case ADD: opStr = "+"; break;
            case SUB: opStr = "-"; break;
            case MUL: opStr = "*"; break;
            case DIV: opStr = "/"; break;
            case MOD: opStr = "%"; break;
            case LT: opStr = "<"; break;
            case GT: opStr = ">"; break;
            case LE: opStr = "<="; break;
            case GE: opStr = ">="; break;
            case EQ: opStr = "=="; break;
            case NE: opStr = "!="; break;
            case AND: opStr = "&&"; break;
            case OR: opStr = "||"; break;
        }
        std::cout << "BinaryExpr: " << opStr << std::endl;
        lhs->print(indent + 1);
        rhs->print(indent + 1);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto lhsClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(lhs->clone().release()));
        auto rhsClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(rhs->clone().release()));
        return std::make_unique<BinaryExprAST>(op, std::move(lhsClone), std::move(rhsClone), getLineNumber());
    }
};

// 一元运算表达式
class UnaryExprAST : public ExprAST {
public:
    enum Operator {
        PLUS,
        MINUS,
        NOT
    };

private:
    Operator op;
    std::unique_ptr<ExprAST> operand;

public:
    UnaryExprAST(Operator oper, std::unique_ptr<ExprAST> expr, int line = -1)
        : ExprAST(line), op(oper), operand(std::move(expr)) {}

    Operator getOp() const { return op; }
    ExprAST* getOperand() const { return operand.get(); }
    
    // 设置操作数
    void setOperand(std::unique_ptr<ExprAST> newOperand) {
        operand = std::move(newOperand);
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::string opStr;
        switch (op) {
            case PLUS: opStr = "+"; break;
            case MINUS: opStr = "-"; break;
            case NOT: opStr = "!"; break;
        }
        std::cout << "UnaryExpr: " << opStr << std::endl;
        operand->print(indent + 1);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto operandClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(operand->clone().release()));
        return std::make_unique<UnaryExprAST>(op, std::move(operandClone), getLineNumber());
    }
};

// 函数调用表达式
class CallExprAST : public ExprAST {
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;
    
public:
    CallExprAST(const std::string& func, int line = -1) : ExprAST(line), callee(func) {}
    
    void addArg(std::unique_ptr<ExprAST> arg) {
        args.push_back(std::move(arg));
    }
    
    const std::string& getCallee() const { return callee; }
    const std::vector<std::unique_ptr<ExprAST>>& getArgs() const { return args; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "CallExpr: " << callee << " (" << args.size() << " args)" << std::endl;
        for (const auto& arg : args) {
            arg->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<CallExprAST>(callee, getLineNumber());
        for (const auto& arg : args) {
            auto argClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(arg->clone().release()));
            clone->addArg(std::move(argClone));
        }
        return clone;
    }
};

// 字符串字面量（用于putf等函数）
class StringLiteralExprAST : public ExprAST {
    std::string value;
    
public:
    StringLiteralExprAST(const std::string& val, int line = -1) : ExprAST(line), value(val) {}
    
    const std::string& getValue() const { return value; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "StringLiteral: \"" << value << "\"" << std::endl;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<StringLiteralExprAST>(value, getLineNumber());
    }
};

// ==================== 初始化值节点 ====================

class InitValAST : public ASTNode {
public:
    virtual ~InitValAST() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

// 表达式初始化值
class ExprInitValAST : public InitValAST {
    std::unique_ptr<ExprAST> expr;
    
public:
    ExprInitValAST(std::unique_ptr<ExprAST> e) : expr(std::move(e)) {}
    
    ExprAST* getExpr() const { return expr.get(); }
    
    // 设置表达式
    void setExpr(std::unique_ptr<ExprAST> newExpr) {
        expr = std::move(newExpr);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ExprInitVal:" << std::endl;
        expr->print(indent + 1);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto exprClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(expr->clone().release()));
        return std::make_unique<ExprInitValAST>(std::move(exprClone));
    }
};

// 列表初始化值（数组）
class ListInitValAST : public InitValAST {
    std::vector<std::unique_ptr<InitValAST>> initVals;
    
public:
    void addInitVal(std::unique_ptr<InitValAST> val) {
        initVals.push_back(std::move(val));
    }
    
    const std::vector<std::unique_ptr<InitValAST>>& getInitVals() const {
        return initVals;
    }
    
    // 获取可修改的initVals引用
    std::vector<std::unique_ptr<InitValAST>>& getMutableInitVals() {
        return initVals;
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ListInitVal: {" << initVals.size() << " elements}" << std::endl;
        for (const auto& val : initVals) {
            val->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<ListInitValAST>();
        for (const auto& val : initVals) {
            auto valClone = std::unique_ptr<InitValAST>(static_cast<InitValAST*>(val->clone().release()));
            clone->addInitVal(std::move(valClone));
        }
        return clone;
    }
};

// ==================== 声明节点 ====================

class DeclAST : public ASTNode {
public:
    virtual ~DeclAST() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

// 变量定义
class VarDefAST : public ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ExprAST>> arraySizes;  // 数组维度
    std::unique_ptr<InitValAST> initVal;
    
public:
    VarDefAST(const std::string& n) : name(n) {}
    
    void addArraySize(std::unique_ptr<ExprAST> size) {
        arraySizes.push_back(std::move(size));
    }
    
    void setInitVal(std::unique_ptr<InitValAST> val) {
        initVal = std::move(val);
    }
    
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<ExprAST>>& getArraySizes() const { return arraySizes; }
    InitValAST* getInitVal() const { return initVal.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "VarDef: " << name;
        if (!arraySizes.empty()) {
            std::cout << " [array]";
        }
        std::cout << std::endl;
        
        for (const auto& size : arraySizes) {
            size->print(indent + 1);
        }
        
        if (initVal) {
            printIndent(indent + 1);
            std::cout << "InitVal:" << std::endl;
            initVal->print(indent + 2);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<VarDefAST>(name);
        for (const auto& size : arraySizes) {
            auto sizeClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(size->clone().release()));
            clone->addArraySize(std::move(sizeClone));
        }
        if (initVal) {
            auto initClone = std::unique_ptr<InitValAST>(static_cast<InitValAST*>(initVal->clone().release()));
            clone->setInitVal(std::move(initClone));
        }
        return clone;
    }
};

// 变量声明
class VarDeclAST : public DeclAST {
    std::unique_ptr<TypeAST> type;
    std::vector<std::unique_ptr<VarDefAST>> varDefs;
    
public:
    VarDeclAST(std::unique_ptr<TypeAST> t) : type(std::move(t)) {}
    
    void addVarDef(std::unique_ptr<VarDefAST> varDef) {
        varDefs.push_back(std::move(varDef));
    }
    
    TypeAST* getType() const { return type.get(); }
    const std::vector<std::unique_ptr<VarDefAST>>& getVarDefs() const { return varDefs; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "VarDecl:" << std::endl;
        type->print(indent + 1);
        for (const auto& varDef : varDefs) {
            varDef->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto typeClone = std::unique_ptr<TypeAST>(static_cast<TypeAST*>(type->clone().release()));
        auto clone = std::make_unique<VarDeclAST>(std::move(typeClone));
        for (const auto& varDef : varDefs) {
            auto varDefClone = std::unique_ptr<VarDefAST>(static_cast<VarDefAST*>(varDef->clone().release()));
            clone->addVarDef(std::move(varDefClone));
        }
        return clone;
    }
};

// 常量定义
class ConstDefAST : public ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ExprAST>> arraySizes;
    std::unique_ptr<InitValAST> initVal;
    
public:
    ConstDefAST(const std::string& n) : name(n) {}
    
    void addArraySize(std::unique_ptr<ExprAST> size) {
        arraySizes.push_back(std::move(size));
    }
    
    void setInitVal(std::unique_ptr<InitValAST> val) {
        initVal = std::move(val);
    }
    
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<ExprAST>>& getArraySizes() const { return arraySizes; }
    InitValAST* getInitVal() const { return initVal.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ConstDef: " << name << std::endl;
        for (const auto& size : arraySizes) {
            size->print(indent + 1);
        }
        if (initVal) {
            initVal->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<ConstDefAST>(name);
        for (const auto& size : arraySizes) {
            auto sizeClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(size->clone().release()));
            clone->addArraySize(std::move(sizeClone));
        }
        if (initVal) {
            auto initClone = std::unique_ptr<InitValAST>(static_cast<InitValAST*>(initVal->clone().release()));
            clone->setInitVal(std::move(initClone));
        }
        return clone;
    }
};

// 常量声明
class ConstDeclAST : public DeclAST {
    std::unique_ptr<TypeAST> type;
    std::vector<std::unique_ptr<ConstDefAST>> constDefs;
    
public:
    ConstDeclAST(std::unique_ptr<TypeAST> t) : type(std::move(t)) {}
    
    void addConstDef(std::unique_ptr<ConstDefAST> constDef) {
        constDefs.push_back(std::move(constDef));
    }
    
    TypeAST* getType() const { return type.get(); }
    const std::vector<std::unique_ptr<ConstDefAST>>& getConstDefs() const { return constDefs; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ConstDecl:" << std::endl;
        type->print(indent + 1);
        for (const auto& constDef : constDefs) {
            constDef->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto typeClone = std::unique_ptr<TypeAST>(static_cast<TypeAST*>(type->clone().release()));
        auto clone = std::make_unique<ConstDeclAST>(std::move(typeClone));
        for (const auto& constDef : constDefs) {
            auto constDefClone = std::unique_ptr<ConstDefAST>(static_cast<ConstDefAST*>(constDef->clone().release()));
            clone->addConstDef(std::move(constDefClone));
        }
        return clone;
    }
};

// ==================== 语句节点 ====================

class StmtAST : public ASTNode {
public:
    virtual ~StmtAST() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

// 赋值语句
class AssignStmtAST : public StmtAST {
    std::unique_ptr<LValExprAST> lval;
    std::unique_ptr<ExprAST> expr;
    
public:
    AssignStmtAST(std::unique_ptr<LValExprAST> lv, std::unique_ptr<ExprAST> e)
        : lval(std::move(lv)), expr(std::move(e)) {}
    
    LValExprAST* getLVal() const { return lval.get(); }
    ExprAST* getExpr() const { return expr.get(); }
    
    // 设置表达式
    void setExpr(std::unique_ptr<ExprAST> newExpr) {
        expr = std::move(newExpr);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "AssignStmt:" << std::endl;
        lval->print(indent + 1);
        expr->print(indent + 1);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto lvalClone = std::unique_ptr<LValExprAST>(static_cast<LValExprAST*>(lval->clone().release()));
        auto exprClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(expr->clone().release()));
        return std::make_unique<AssignStmtAST>(std::move(lvalClone), std::move(exprClone));
    }
};

// 表达式语句
class ExprStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> expr;  // 可能为空
    
public:
    ExprStmtAST(std::unique_ptr<ExprAST> e = nullptr) : expr(std::move(e)) {}
    
    ExprAST* getExpr() const { return expr.get(); }
    
    // 设置表达式
    void setExpr(std::unique_ptr<ExprAST> newExpr) {
        expr = std::move(newExpr);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ExprStmt:" << std::endl;
        if (expr) {
            expr->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        std::unique_ptr<ExprAST> exprClone = nullptr;
        if (expr) {
            exprClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(expr->clone().release()));
        }
        return std::make_unique<ExprStmtAST>(std::move(exprClone));
    }
};

// Return 语句
class ReturnStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> returnValue;  // 可能为空（void函数）
    
public:
    ReturnStmtAST(std::unique_ptr<ExprAST> expr = nullptr) 
        : returnValue(std::move(expr)) {}
    
    ExprAST* getReturnValue() const { return returnValue.get(); }
    
    // 设置返回值
    void setReturnValue(std::unique_ptr<ExprAST> newReturnValue) {
        returnValue = std::move(newReturnValue);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ReturnStmt:" << std::endl;
        if (returnValue) {
            returnValue->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        std::unique_ptr<ExprAST> returnClone = nullptr;
        if (returnValue) {
            returnClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(returnValue->clone().release()));
        }
        return std::make_unique<ReturnStmtAST>(std::move(returnClone));
    }
};

// If 语句
class IfStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> thenStmt;
    std::unique_ptr<StmtAST> elseStmt;  // 可能为空
    
public:
    IfStmtAST(std::unique_ptr<ExprAST> cond,
              std::unique_ptr<StmtAST> thenS,
              std::unique_ptr<StmtAST> elseS = nullptr)
        : condition(std::move(cond)), thenStmt(std::move(thenS)), elseStmt(std::move(elseS)) {}
    
    ExprAST* getCondition() const { return condition.get(); }
    StmtAST* getThenStmt() const { return thenStmt.get(); }
    StmtAST* getElseStmt() const { return elseStmt.get(); }
    
    // 设置条件
    void setCondition(std::unique_ptr<ExprAST> newCond) {
        condition = std::move(newCond);
    }
    
    // 设置then语句
    void setThenStmt(std::unique_ptr<StmtAST> newThenStmt) {
        thenStmt = std::move(newThenStmt);
    }
    
    // 设置else语句
    void setElseStmt(std::unique_ptr<StmtAST> newElseStmt) {
        elseStmt = std::move(newElseStmt);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "IfStmt:" << std::endl;
        printIndent(indent + 1);
        std::cout << "Condition:" << std::endl;
        condition->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "Then:" << std::endl;
        thenStmt->print(indent + 2);
        if (elseStmt) {
            printIndent(indent + 1);
            std::cout << "Else:" << std::endl;
            elseStmt->print(indent + 2);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto condClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(condition->clone().release()));
        auto thenClone = std::unique_ptr<StmtAST>(static_cast<StmtAST*>(thenStmt->clone().release()));
        std::unique_ptr<StmtAST> elseClone = nullptr;
        if (elseStmt) {
            elseClone = std::unique_ptr<StmtAST>(static_cast<StmtAST*>(elseStmt->clone().release()));
        }
        return std::make_unique<IfStmtAST>(std::move(condClone), std::move(thenClone), std::move(elseClone));
    }
};

// While 语句
class WhileStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> body;
    
public:
    WhileStmtAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<StmtAST> b)
        : condition(std::move(cond)), body(std::move(b)) {}
    
    ExprAST* getCondition() const { return condition.get(); }
    StmtAST* getBody() const { return body.get(); }
    
    // 设置条件
    void setCondition(std::unique_ptr<ExprAST> newCond) {
        condition = std::move(newCond);
    }
    
    // 设置body
    void setBody(std::unique_ptr<StmtAST> newBody) {
        body = std::move(newBody);
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "WhileStmt:" << std::endl;
        printIndent(indent + 1);
        std::cout << "Condition:" << std::endl;
        condition->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "Body:" << std::endl;
        body->print(indent + 2);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto condClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(condition->clone().release()));
        auto bodyClone = std::unique_ptr<StmtAST>(static_cast<StmtAST*>(body->clone().release()));
        return std::make_unique<WhileStmtAST>(std::move(condClone), std::move(bodyClone));
    }
};

// Break 语句
class BreakStmtAST : public StmtAST {
public:
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BreakStmt" << std::endl;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<BreakStmtAST>();
    }
};

// Continue 语句
class ContinueStmtAST : public StmtAST {
public:
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ContinueStmt" << std::endl;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ContinueStmtAST>();
    }
};

// ==================== 块节点 ====================

class BlockItemAST : public ASTNode {
public:
    virtual ~BlockItemAST() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

// 声明块项
class DeclBlockItemAST : public BlockItemAST {
    std::unique_ptr<DeclAST> decl;
    
public:
    DeclBlockItemAST(std::unique_ptr<DeclAST> d) : decl(std::move(d)) {}
    
    DeclAST* getDecl() const { return decl.get(); }
    
    void print(int indent = 0) const override {
        decl->print(indent);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto declClone = std::unique_ptr<DeclAST>(static_cast<DeclAST*>(decl->clone().release()));
        return std::make_unique<DeclBlockItemAST>(std::move(declClone));
    }
};

// 语句块项
class StmtBlockItemAST : public BlockItemAST {
    std::unique_ptr<StmtAST> stmt;
    
public:
    StmtBlockItemAST(std::unique_ptr<StmtAST> s) : stmt(std::move(s)) {}
    
    StmtAST* getStmt() const { return stmt.get(); }
    
    void print(int indent = 0) const override {
        stmt->print(indent);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto stmtClone = std::unique_ptr<StmtAST>(static_cast<StmtAST*>(stmt->clone().release()));
        return std::make_unique<StmtBlockItemAST>(std::move(stmtClone));
    }
};

// 语句块
class BlockAST : public StmtAST {
    std::vector<std::unique_ptr<BlockItemAST>> items;
    
public:
    BlockAST() = default;
    
    void addItem(std::unique_ptr<BlockItemAST> item) {
        items.push_back(std::move(item));
    }
    
    const std::vector<std::unique_ptr<BlockItemAST>>& getItems() const {
        return items;
    }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "Block: (" << items.size() << " items)" << std::endl;
        for (const auto& item : items) {
            item->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<BlockAST>();
        for (const auto& item : items) {
            auto itemClone = std::unique_ptr<BlockItemAST>(static_cast<BlockItemAST*>(item->clone().release()));
            clone->addItem(std::move(itemClone));
        }
        return clone;
    }
};

// ==================== 函数参数节点 ====================

class FuncFParamAST : public ASTNode {
    std::unique_ptr<TypeAST> type;
    std::string name;
    bool isArray;
    std::vector<std::unique_ptr<ExprAST>> arraySizes;  // 第一维为空，后续维度有大小
    
public:
    FuncFParamAST(std::unique_ptr<TypeAST> t, const std::string& n, bool arr = false)
        : type(std::move(t)), name(n), isArray(arr) {}
    
    void addArraySize(std::unique_ptr<ExprAST> size) {
        arraySizes.push_back(std::move(size));
    }
    
    TypeAST* getType() const { return type.get(); }
    const std::string& getName() const { return name; }
    bool getIsArray() const { return isArray; }
    const std::vector<std::unique_ptr<ExprAST>>& getArraySizes() const { return arraySizes; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "FuncFParam: " << name;
        if (isArray) {
            std::cout << " [array]";
        }
        std::cout << std::endl;
        type->print(indent + 1);
        for (const auto& size : arraySizes) {
            size->print(indent + 1);
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto typeClone = std::unique_ptr<TypeAST>(static_cast<TypeAST*>(type->clone().release()));
        auto clone = std::make_unique<FuncFParamAST>(std::move(typeClone), name, isArray);
        for (const auto& size : arraySizes) {
            auto sizeClone = std::unique_ptr<ExprAST>(static_cast<ExprAST*>(size->clone().release()));
            clone->addArraySize(std::move(sizeClone));
        }
        return clone;
    }
};

// ==================== 函数节点 ====================

class FunctionAST : public ASTNode {
    std::unique_ptr<TypeAST> returnType;
    std::string name;
    std::vector<std::unique_ptr<FuncFParamAST>> params;
    std::unique_ptr<BlockAST> body;
    
public:
    FunctionAST(std::unique_ptr<TypeAST> retType, 
                const std::string& funcName,
                std::unique_ptr<BlockAST> funcBody)
        : returnType(std::move(retType)),
          name(funcName),
          body(std::move(funcBody)) {}
    
    void addParam(std::unique_ptr<FuncFParamAST> param) {
        params.push_back(std::move(param));
    }
    
    const std::string& getName() const { return name; }
    TypeAST* getReturnType() const { return returnType.get(); }
    const std::vector<std::unique_ptr<FuncFParamAST>>& getParams() const { return params; }
    BlockAST* getBody() const { return body.get(); }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "Function: " << name << " (" << params.size() << " params)" << std::endl;
        
        printIndent(indent + 1);
        std::cout << "ReturnType:" << std::endl;
        returnType->print(indent + 2);
        
        if (!params.empty()) {
            printIndent(indent + 1);
            std::cout << "Params:" << std::endl;
            for (const auto& param : params) {
                param->print(indent + 2);
            }
        }
        
        printIndent(indent + 1);
        std::cout << "Body:" << std::endl;
        body->print(indent + 2);
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto retTypeClone = std::unique_ptr<TypeAST>(static_cast<TypeAST*>(returnType->clone().release()));
        auto bodyClone = std::unique_ptr<BlockAST>(static_cast<BlockAST*>(body->clone().release()));
        auto clone = std::make_unique<FunctionAST>(std::move(retTypeClone), name, std::move(bodyClone));
        for (const auto& param : params) {
            auto paramClone = std::unique_ptr<FuncFParamAST>(static_cast<FuncFParamAST*>(param->clone().release()));
            clone->addParam(std::move(paramClone));
        }
        return clone;
    }
};

// ==================== 编译单元节点（根节点）====================

class CompUnitAST : public ASTNode {
    std::vector<std::unique_ptr<DeclAST>> decls;
    std::vector<std::unique_ptr<FunctionAST>> functions;
    
public:
    CompUnitAST() = default;
    
    void addDecl(std::unique_ptr<DeclAST> decl) {
        decls.push_back(std::move(decl));
    }
    
    void addFunction(std::unique_ptr<FunctionAST> func) {
        functions.push_back(std::move(func));
    }
    
    const std::vector<std::unique_ptr<DeclAST>>& getDecls() const { return decls; }
    const std::vector<std::unique_ptr<FunctionAST>>& getFunctions() const { return functions; }
    
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "=== CompUnit AST ===" << std::endl;
        
        if (!decls.empty()) {
            printIndent(indent);
            std::cout << "Global Declarations:" << std::endl;
            for (const auto& decl : decls) {
                decl->print(indent + 1);
            }
        }
        
        if (!functions.empty()) {
            printIndent(indent);
            std::cout << "Functions:" << std::endl;
            for (const auto& func : functions) {
                func->print(indent + 1);
            }
        }
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        auto clone = std::make_unique<CompUnitAST>();
        for (const auto& decl : decls) {
            auto declClone = std::unique_ptr<DeclAST>(static_cast<DeclAST*>(decl->clone().release()));
            clone->addDecl(std::move(declClone));
        }
        for (const auto& func : functions) {
            auto funcClone = std::unique_ptr<FunctionAST>(static_cast<FunctionAST*>(func->clone().release()));
            clone->addFunction(std::move(funcClone));
        }
        return clone;
    }
};

#endif // AST_H
