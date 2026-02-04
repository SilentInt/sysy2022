// ast_builder.h
#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include "ast.h"
#include "frontend/SysYBaseVisitor.h"
#include "frontend/SysYParser.h"
#include <stdexcept>
#include <memory>
#include <any>

class ASTBuilder : public SysYBaseVisitor {
public:
    // ==================== 编译单元 ====================
    
    std::any visitCompUnit(SysYParser::CompUnitContext *ctx) override {
        auto compUnit = std::make_unique<CompUnitAST>();
        
        for (auto child : ctx->children) {
            if (auto declCtx = dynamic_cast<SysYParser::DeclContext*>(child)) {
                auto declAny = visit(declCtx);
                DeclAST* declPtr = std::any_cast<DeclAST*>(declAny);
                compUnit->addDecl(std::unique_ptr<DeclAST>(declPtr));
            } else if (auto funcDefCtx = dynamic_cast<SysYParser::FuncDefContext*>(child)) {
                auto funcAny = visit(funcDefCtx);
                FunctionAST* funcPtr = std::any_cast<FunctionAST*>(funcAny);
                compUnit->addFunction(std::unique_ptr<FunctionAST>(funcPtr));
            }
        }
        
        return compUnit.release();
    }

    // ==================== 声明 ====================
    
    std::any visitDecl(SysYParser::DeclContext *ctx) override {
        if (ctx->constDecl()) {
            return visit(ctx->constDecl());
        } else if (ctx->varDecl()) {
            return visit(ctx->varDecl());
        }
        throw std::runtime_error("Unknown declaration type");
    }

    // 常量声明
    std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override {
        auto typeAny = visit(ctx->bType());
        TypeAST* typePtr = std::any_cast<TypeAST*>(typeAny);
        auto constDecl = std::make_unique<ConstDeclAST>(std::unique_ptr<TypeAST>(typePtr));
        
        for (auto constDefCtx : ctx->constDef()) {
            auto constDefAny = visit(constDefCtx);
            ConstDefAST* constDefPtr = std::any_cast<ConstDefAST*>(constDefAny);
            constDecl->addConstDef(std::unique_ptr<ConstDefAST>(constDefPtr));
        }
        
        return static_cast<DeclAST*>(constDecl.release());
    }

    // 常量定义
    std::any visitConstDef(SysYParser::ConstDefContext *ctx) override {
        std::string name = ctx->IDENT()->getText();
        auto constDef = std::make_unique<ConstDefAST>(name);
        
        // 处理数组维度
        for (auto constExpCtx : ctx->constExp()) {
            auto expAny = visit(constExpCtx);
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            constDef->addArraySize(std::unique_ptr<ExprAST>(expPtr));
        }
        
        // 处理初始化值
        auto initValAny = visit(ctx->constInitVal());
        InitValAST* initValPtr = std::any_cast<InitValAST*>(initValAny);
        constDef->setInitVal(std::unique_ptr<InitValAST>(initValPtr));
        
        return constDef.release();
    }

    // 常量初始化值
    std::any visitConstInitVal(SysYParser::ConstInitValContext *ctx) override {
        if (ctx->constExp()) {
            // 单个表达式
            auto expAny = visit(ctx->constExp());
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            auto initVal = std::make_unique<ExprInitValAST>(std::unique_ptr<ExprAST>(expPtr));
            return static_cast<InitValAST*>(initVal.release());
        } else {
            // 列表初始化
            auto listInitVal = std::make_unique<ListInitValAST>();
            for (auto initValCtx : ctx->constInitVal()) {
                auto initValAny = visit(initValCtx);
                InitValAST* initValPtr = std::any_cast<InitValAST*>(initValAny);
                listInitVal->addInitVal(std::unique_ptr<InitValAST>(initValPtr));
            }
            return static_cast<InitValAST*>(listInitVal.release());
        }
    }

    // 变量声明
    std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override {
        auto typeAny = visit(ctx->bType());
        TypeAST* typePtr = std::any_cast<TypeAST*>(typeAny);
        auto varDecl = std::make_unique<VarDeclAST>(std::unique_ptr<TypeAST>(typePtr));
        
        for (auto varDefCtx : ctx->varDef()) {
            auto varDefAny = visit(varDefCtx);
            VarDefAST* varDefPtr = std::any_cast<VarDefAST*>(varDefAny);
            varDecl->addVarDef(std::unique_ptr<VarDefAST>(varDefPtr));
        }
        
        return static_cast<DeclAST*>(varDecl.release());
    }

    // 变量定义
    std::any visitVarDef(SysYParser::VarDefContext *ctx) override {
        std::string name = ctx->IDENT()->getText();
        auto varDef = std::make_unique<VarDefAST>(name);
        
        // 处理数组维度
        for (auto constExpCtx : ctx->constExp()) {
            auto expAny = visit(constExpCtx);
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            varDef->addArraySize(std::unique_ptr<ExprAST>(expPtr));
        }
        
        // 处理初始化值（如果有）
        if (ctx->initVal()) {
            auto initValAny = visit(ctx->initVal());
            InitValAST* initValPtr = std::any_cast<InitValAST*>(initValAny);
            varDef->setInitVal(std::unique_ptr<InitValAST>(initValPtr));
        }
        
        return varDef.release();
    }

    // 变量初始化值
    std::any visitInitVal(SysYParser::InitValContext *ctx) override {
        if (ctx->exp()) {
            // 单个表达式
            auto expAny = visit(ctx->exp());
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            auto initVal = std::make_unique<ExprInitValAST>(std::unique_ptr<ExprAST>(expPtr));
            return static_cast<InitValAST*>(initVal.release());
        } else {
            // 列表初始化
            auto listInitVal = std::make_unique<ListInitValAST>();
            for (auto initValCtx : ctx->initVal()) {
                auto initValAny = visit(initValCtx);
                InitValAST* initValPtr = std::any_cast<InitValAST*>(initValAny);
                listInitVal->addInitVal(std::unique_ptr<InitValAST>(initValPtr));
            }
            return static_cast<InitValAST*>(listInitVal.release());
        }
    }

    // ==================== 类型 ====================
    
    std::any visitBType(SysYParser::BTypeContext *ctx) override {
        std::unique_ptr<TypeAST> type;
        if (ctx->INT()) {
            type = std::make_unique<TypeAST>(TypeAST::Kind::INT);
        } else if (ctx->FLOAT()) {
            type = std::make_unique<TypeAST>(TypeAST::Kind::FLOAT);
        } else if (ctx->vectorType()) {
            auto typeAny = visit(ctx->vectorType());
            TypeAST* typePtr = std::any_cast<TypeAST*>(typeAny);
            type = std::unique_ptr<TypeAST>(typePtr);
        } else {
            throw std::runtime_error("Unknown basic type");
        }
        return type.release();
    }

    std::any visitVectorType(SysYParser::VectorTypeContext *ctx) override {
        TypeAST::Kind elemKind;
        if (ctx->INT()) {
            elemKind = TypeAST::Kind::INT;
        } else if (ctx->FLOAT()) {
            elemKind = TypeAST::Kind::FLOAT;
        } else {
            throw std::runtime_error("Unknown vector element type");
        }
        auto sizeAny = visit(ctx->constExp());
        ExprAST* sizeExpr = std::any_cast<ExprAST*>(sizeAny);
        auto type = std::make_unique<TypeAST>(elemKind, std::unique_ptr<ExprAST>(sizeExpr));
        return type.release();
    }

    // ==================== 函数 ====================
    
    std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override {
        // 获取返回类型
        auto retTypeAny = visit(ctx->funcType());
        TypeAST* retTypePtr = std::any_cast<TypeAST*>(retTypeAny);
        
        // 获取函数名
        std::string funcName = ctx->IDENT()->getText();
        
        // 获取函数体
        auto bodyAny = visit(ctx->block());
        BlockAST* bodyPtr = std::any_cast<BlockAST*>(bodyAny);
        
        // 创建函数
        auto func = std::make_unique<FunctionAST>(
            std::unique_ptr<TypeAST>(retTypePtr),
            funcName,
            std::unique_ptr<BlockAST>(bodyPtr)
        );
        
        // 添加参数
        if (ctx->funcFParams()) {
            auto paramsAny = visit(ctx->funcFParams());
            auto params = std::any_cast<std::vector<FuncFParamAST*>>(paramsAny);
            for (auto param : params) {
                func->addParam(std::unique_ptr<FuncFParamAST>(param));
            }
        }
        
        return func.release();
    }

    std::any visitFuncType(SysYParser::FuncTypeContext *ctx) override {
        std::unique_ptr<TypeAST> type;
        if (ctx->VOID()) {
            type = std::make_unique<TypeAST>(TypeAST::Kind::VOID);
        } else if (ctx->INT()) {
            type = std::make_unique<TypeAST>(TypeAST::Kind::INT);
        } else if (ctx->FLOAT()) {
            type = std::make_unique<TypeAST>(TypeAST::Kind::FLOAT);
        } else if (ctx->vectorType()) {
            auto typeAny = visit(ctx->vectorType());
            TypeAST* typePtr = std::any_cast<TypeAST*>(typeAny);
            type = std::unique_ptr<TypeAST>(typePtr);
        } else {
            throw std::runtime_error("Unknown function type");
        }
        return type.release();
    }

    std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override {
        std::vector<FuncFParamAST*> params;
        for (auto paramCtx : ctx->funcFParam()) {
            auto paramAny = visit(paramCtx);
            FuncFParamAST* paramPtr = std::any_cast<FuncFParamAST*>(paramAny);
            params.push_back(paramPtr);
        }
        return params;
    }

    std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override {
        auto typeAny = visit(ctx->bType());
        TypeAST* typePtr = std::any_cast<TypeAST*>(typeAny);
        std::string name = ctx->IDENT()->getText();
        
        // 检查是否是数组参数
        auto brackets = ctx->LBRACK();
        bool isArray = !brackets.empty();
        
        // 只创建一次对象
        auto param = std::make_unique<FuncFParamAST>(
            std::unique_ptr<TypeAST>(typePtr),
            name,
            isArray
        );
        
        // 如果是数组参数，添加维度信息
        if (isArray) {
            // 第一维是空的（arr[]），从第二维开始有表达式
            // 注意：ctx->exp() 返回的是除第一维外的所有维度
            for (size_t i = 0; i < ctx->exp().size(); i++) {
                auto expAny = visit(ctx->exp(i));
                ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
                param->addArraySize(std::unique_ptr<ExprAST>(expPtr));
            }
        }
        
        return param.release();
    }

    // ==================== 语句块 ====================
    
    std::any visitBlock(SysYParser::BlockContext *ctx) override {
        auto block = std::make_unique<BlockAST>();
        
        for (auto itemCtx : ctx->blockItem()) {
            auto itemAny = visit(itemCtx);
            BlockItemAST* itemPtr = std::any_cast<BlockItemAST*>(itemAny);
            block->addItem(std::unique_ptr<BlockItemAST>(itemPtr));
        }
        
        return block.release();
    }

    std::any visitBlockItem(SysYParser::BlockItemContext *ctx) override {
        if (ctx->decl()) {
            auto declAny = visit(ctx->decl());
            DeclAST* declPtr = std::any_cast<DeclAST*>(declAny);
            auto item = std::make_unique<DeclBlockItemAST>(std::unique_ptr<DeclAST>(declPtr));
            return static_cast<BlockItemAST*>(item.release());
        } else if (ctx->stmt()) {
            auto stmtAny = visit(ctx->stmt());
            StmtAST* stmtPtr = std::any_cast<StmtAST*>(stmtAny);
            auto item = std::make_unique<StmtBlockItemAST>(std::unique_ptr<StmtAST>(stmtPtr));
            return static_cast<BlockItemAST*>(item.release());
        }
        throw std::runtime_error("Unknown block item type");
    }

    // ==================== 语句 ====================
    
    std::any visitStmt(SysYParser::StmtContext *ctx) override {
        // 赋值语句
        if (ctx->ASSIGN()) {
            auto lvalAny = visit(ctx->lVal());
            LValExprAST* lvalPtr = std::any_cast<LValExprAST*>(lvalAny);
            auto expAny = visit(ctx->exp());
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            
            auto assignStmt = std::make_unique<AssignStmtAST>(
                std::unique_ptr<LValExprAST>(lvalPtr),
                std::unique_ptr<ExprAST>(expPtr)
            );
            return static_cast<StmtAST*>(assignStmt.release());
        }
        
        // Break 语句
        if (ctx->BREAK()) {
            auto breakStmt = std::make_unique<BreakStmtAST>();
            return static_cast<StmtAST*>(breakStmt.release());
        }
        
        // Continue 语句
        if (ctx->CONTINUE()) {
            auto continueStmt = std::make_unique<ContinueStmtAST>();
            return static_cast<StmtAST*>(continueStmt.release());
        }
        
        // Return 语句
        if (ctx->RETURN()) {
            std::unique_ptr<ExprAST> returnValue = nullptr;
            if (ctx->exp()) {
                auto expAny = visit(ctx->exp());
                ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
                returnValue = std::unique_ptr<ExprAST>(expPtr);
            }
            auto returnStmt = std::make_unique<ReturnStmtAST>(std::move(returnValue));
            return static_cast<StmtAST*>(returnStmt.release());
        }
        
        // 表达式语句
        if (ctx->SEMICOLON()) {
            std::unique_ptr<ExprAST> expr = nullptr;
            if (ctx->exp()) {
                auto expAny = visit(ctx->exp());
                ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
                expr = std::unique_ptr<ExprAST>(expPtr);
            }
            auto exprStmt = std::make_unique<ExprStmtAST>(std::move(expr));
            return static_cast<StmtAST*>(exprStmt.release());
        }
        
        // 语句块
        if (ctx->block()) {
            auto blockAny = visit(ctx->block());
            BlockAST* blockPtr = std::any_cast<BlockAST*>(blockAny);
            return static_cast<StmtAST*>(blockPtr);
        }
        
        // If 语句
        if (ctx->IF()) {
            auto condAny = visit(ctx->cond());
            ExprAST* condPtr = std::any_cast<ExprAST*>(condAny);
            
            auto thenStmtAny = visit(ctx->stmt(0));
            StmtAST* thenPtr = std::any_cast<StmtAST*>(thenStmtAny);
            
            std::unique_ptr<StmtAST> elseStmt = nullptr;
            if (ctx->ELSE()) {
                auto elseStmtAny = visit(ctx->stmt(1));
                StmtAST* elsePtr = std::any_cast<StmtAST*>(elseStmtAny);
                elseStmt = std::unique_ptr<StmtAST>(elsePtr);
            }
            
            auto ifStmt = std::make_unique<IfStmtAST>(
                std::unique_ptr<ExprAST>(condPtr),
                std::unique_ptr<StmtAST>(thenPtr),
                std::move(elseStmt)
            );
            return static_cast<StmtAST*>(ifStmt.release());
        }
        
        // While 语句
        if (ctx->WHILE()) {
            auto condAny = visit(ctx->cond());
            ExprAST* condPtr = std::any_cast<ExprAST*>(condAny);
            
            auto bodyAny = visit(ctx->stmt(0));
            StmtAST* bodyPtr = std::any_cast<StmtAST*>(bodyAny);
            
            auto whileStmt = std::make_unique<WhileStmtAST>(
                std::unique_ptr<ExprAST>(condPtr),
                std::unique_ptr<StmtAST>(bodyPtr)
            );
            return static_cast<StmtAST*>(whileStmt.release());
        }
        
        throw std::runtime_error("Unknown statement type");
    }

    // ==================== 表达式 ====================
    
    std::any visitExp(SysYParser::ExpContext *ctx) override {
        return visit(ctx->addExp());
    }

    std::any visitCond(SysYParser::CondContext *ctx) override {
        return visit(ctx->lOrExp());
    }

    std::any visitLVal(SysYParser::LValContext *ctx) override {
        std::string name = ctx->IDENT()->getText();
        int line = ctx->getStart()->getLine();
        auto lval = std::make_unique<LValExprAST>(name, line);
        
        for (auto expCtx : ctx->exp()) {
            auto expAny = visit(expCtx);
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            lval->addIndex(std::unique_ptr<ExprAST>(expPtr));
        }
        
        return lval.release();
    }

    std::any visitPrimaryExp(SysYParser::PrimaryExpContext *ctx) override {
        if (ctx->LPAREN()) {
            return visit(ctx->exp());
        }
        
        if (ctx->lVal()) {
            auto lvalAny = visit(ctx->lVal());
            LValExprAST* lvalPtr = std::any_cast<LValExprAST*>(lvalAny);
            return static_cast<ExprAST*>(lvalPtr);
        }
        
        if (ctx->number()) {
            return visit(ctx->number());
        }
        
        if (ctx->StringLiteral()) {
            return visitStringLiteral(ctx->StringLiteral());
        }
        
        throw std::runtime_error("Unknown primary expression type");
    }

    std::any visitNumber(SysYParser::NumberContext *ctx) override {
        int line = ctx->getStart()->getLine();
        if (ctx->IntConst()) {
            std::string numStr = ctx->IntConst()->getText();
            int value;
            
            // 处理不同进制
            if (numStr.size() >= 2 && (numStr[1] == 'x' || numStr[1] == 'X')) {
                // 十六进制
                value = std::stoi(numStr, nullptr, 16);
            } else if (numStr[0] == '0' && numStr.size() > 1) {
                // 八进制
                value = std::stoi(numStr, nullptr, 8);
            } else {
                // 十进制
                value = std::stoi(numStr);
            }
            
            auto intExpr = std::make_unique<IntConstExprAST>(value, line);
            return static_cast<ExprAST*>(intExpr.release());
        }
        
        if (ctx->FloatConst()) {
            std::string numStr = ctx->FloatConst()->getText();
            float value = std::stof(numStr);
            auto floatExpr = std::make_unique<FloatConstExprAST>(value, line);
            return static_cast<ExprAST*>(floatExpr.release());
        }
        
        throw std::runtime_error("Unknown number type");
    }

    std::any visitUnaryExp(SysYParser::UnaryExpContext *ctx) override {
        // 函数调用
        if (ctx->IDENT()) {
            std::string funcName = ctx->IDENT()->getText();
            // 获取行号
            int line = ctx->getStart()->getLine();
            auto callExpr = std::make_unique<CallExprAST>(funcName, line);
            
            if (ctx->funcRParams()) {
                auto argsAny = visit(ctx->funcRParams());
                auto args = std::any_cast<std::vector<ExprAST*>>(argsAny);
                for (auto arg : args) {
                    callExpr->addArg(std::unique_ptr<ExprAST>(arg));
                }
            }
            
            return static_cast<ExprAST*>(callExpr.release());
        }
        
        // 一元运算
        if (ctx->unaryOp()) {
            auto opAny = visit(ctx->unaryOp());
            UnaryExprAST::Operator op = std::any_cast<UnaryExprAST::Operator>(opAny);
            
            auto operandAny = visit(ctx->unaryExp());
            ExprAST* operandPtr = std::any_cast<ExprAST*>(operandAny);
            
            int line = ctx->getStart()->getLine();
            auto unaryExpr = std::make_unique<UnaryExprAST>(
                op,
                std::unique_ptr<ExprAST>(operandPtr),
                line
            );
            return static_cast<ExprAST*>(unaryExpr.release());
        }
        
        // primaryExp
        return visit(ctx->primaryExp());
    }

    std::any visitUnaryOp(SysYParser::UnaryOpContext *ctx) override {
        if (ctx->PLUS()) {
            return UnaryExprAST::Operator::PLUS;
        } else if (ctx->MINUS()) {
            return UnaryExprAST::Operator::MINUS;
        } else if (ctx->NOT()) {
            return UnaryExprAST::Operator::NOT;
        }
        throw std::runtime_error("Unknown unary operator");
    }

    std::any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override {
        std::vector<ExprAST*> args;
        for (auto expCtx : ctx->exp()) {
            auto expAny = visit(expCtx);
            ExprAST* expPtr = std::any_cast<ExprAST*>(expAny);
            args.push_back(expPtr);
        }
        return args;
    }

    std::any visitMulExp(SysYParser::MulExpContext *ctx) override {
        if (ctx->MUL() || ctx->DIV() || ctx->MOD()) {
            auto lhsAny = visit(ctx->mulExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->unaryExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            BinaryExprAST::Operator op;
            if (ctx->MUL()) {
                op = BinaryExprAST::Operator::MUL;
            } else if (ctx->DIV()) {
                op = BinaryExprAST::Operator::DIV;
            } else {
                op = BinaryExprAST::Operator::MOD;
            }
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                op,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->unaryExp());
    }

    std::any visitAddExp(SysYParser::AddExpContext *ctx) override {
        if (ctx->PLUS() || ctx->MINUS()) {
            auto lhsAny = visit(ctx->addExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->mulExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            BinaryExprAST::Operator op = ctx->PLUS() ? 
                BinaryExprAST::Operator::ADD : BinaryExprAST::Operator::SUB;
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                op,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->mulExp());
    }

    std::any visitRelExp(SysYParser::RelExpContext *ctx) override {
        if (ctx->LT() || ctx->GT() || ctx->LE() || ctx->GE()) {
            auto lhsAny = visit(ctx->relExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->addExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            BinaryExprAST::Operator op;
            if (ctx->LT()) op = BinaryExprAST::Operator::LT;
            else if (ctx->GT()) op = BinaryExprAST::Operator::GT;
            else if (ctx->LE()) op = BinaryExprAST::Operator::LE;
            else op = BinaryExprAST::Operator::GE;
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                op,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->addExp());
    }

    std::any visitEqExp(SysYParser::EqExpContext *ctx) override {
        if (ctx->EQ() || ctx->NE()) {
            auto lhsAny = visit(ctx->eqExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->relExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            BinaryExprAST::Operator op = ctx->EQ() ? 
                BinaryExprAST::Operator::EQ : BinaryExprAST::Operator::NE;
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                op,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->relExp());
    }

    std::any visitLAndExp(SysYParser::LAndExpContext *ctx) override {
        if (ctx->AND()) {
            auto lhsAny = visit(ctx->lAndExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->eqExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                BinaryExprAST::Operator::AND,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->eqExp());
    }

    std::any visitLOrExp(SysYParser::LOrExpContext *ctx) override {
        if (ctx->OR()) {
            auto lhsAny = visit(ctx->lOrExp());
            ExprAST* lhsPtr = std::any_cast<ExprAST*>(lhsAny);
            
            auto rhsAny = visit(ctx->lAndExp());
            ExprAST* rhsPtr = std::any_cast<ExprAST*>(rhsAny);
            
            int line = ctx->getStart()->getLine();
            auto binaryExpr = std::make_unique<BinaryExprAST>(
                BinaryExprAST::Operator::OR,
                std::unique_ptr<ExprAST>(lhsPtr),
                std::unique_ptr<ExprAST>(rhsPtr),
                line
            );
            return static_cast<ExprAST*>(binaryExpr.release());
        }
        
        return visit(ctx->lAndExp());
    }

    std::any visitConstExp(SysYParser::ConstExpContext *ctx) override {
        return visit(ctx->addExp());
    }
    
    // 处理字符串字面量
    std::any visitStringLiteral(antlr4::tree::TerminalNode* ctx) {
        // 获取原始字符串（去掉引号）
        std::string rawStr = ctx->getText();
        std::string value = rawStr.substr(1, rawStr.size() - 2);
        
        // 处理转义序列（简化版）
        size_t pos = 0;
        while ((pos = value.find('\\', pos)) != std::string::npos) {
            if (pos + 1 < value.size()) {
                char escapeChar = value[pos + 1];
                std::string replacement;
                
                switch (escapeChar) {
                    case '"': replacement = "\""; break;
                    case '\\': replacement = "\\"; break;
                    case 'n': replacement = "\n"; break;
                    case 't': replacement = "\t"; break;
                    // 其他转义序列...
                    default: replacement = escapeChar; break;
                }
                
                value.replace(pos, 2, replacement);
                pos += replacement.size();
            } else {
                break;
            }
        }
        
        int line = ctx->getSymbol()->getLine();
        auto strExpr = std::make_unique<StringLiteralExprAST>(value, line);
        return static_cast<ExprAST*>(strExpr.release());
    }
};

#endif // AST_BUILDER_H
