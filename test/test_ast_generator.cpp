#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include "frontend/SysYLexer.h"
#include "frontend/SysYParser.h"
#include "ast/ast_builder.h"

using namespace antlr4;
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // 保存原始cout
    streambuf* original_cout = cout.rdbuf();
    ofstream output_file("ast_test_res.txt");
    // 重定向cout到文件
    cout.rdbuf(output_file.rdbuf());

    // 读取输入文件
    ifstream stream;
    stream.open(argv[1]);
    if (!stream.is_open()) {
        cerr << "Error: Cannot open file " << argv[1] << endl;
        return 1;
    }

    cout << "==================================" << endl;
    cout << "   AST Builder Test" << endl;
    cout << "==================================" << endl;
    cout << "Input file: " << argv[1] << endl << endl;

    try {
        // 创建输入流
        ANTLRInputStream input(stream);

        // 词法分析
        SysYLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        // 语法分析
        SysYParser parser(&tokens);

        // 检查语法错误
        if (parser.getNumberOfSyntaxErrors() > 0) {
            cerr << "✗ Parsing failed with " << parser.getNumberOfSyntaxErrors()
                 << " syntax error(s)" << endl;
            return 1;
        }

        cout << "✓ Parsing successful" << endl << endl;

        // 获取解析树根节点
        tree::ParseTree *tree = parser.compUnit();

        // 构建 AST
        cout << "Building AST..." << endl;
        ASTBuilder builder;
        auto astResult = builder.visit(tree);

        // 从 std::any 中提取原始指针，然后包装成 unique_ptr
        CompUnitAST* astPtr = std::any_cast<CompUnitAST*>(astResult);
        std::unique_ptr<CompUnitAST> ast(astPtr);

        cout << "✓ AST built successfully" << endl << endl;

        // 打印 AST
        cout << "==================================" << endl;
        cout << "   AST Structure" << endl;
        cout << "==================================" << endl;
        ast->print();

        cout << endl << "==================================" << endl;
        cout << "   Test Passed!" << endl;
        cout << "==================================" << endl;

        // 验证 AST 内容
        cout << endl << "Verification:" << endl;
        cout << "  Global declarations: " << ast->getDecls().size() << endl;
        cout << "  Functions: " << ast->getFunctions().size() << endl;

        // 查找main函数
        FunctionAST* mainFunc = nullptr;
        for (const auto& func : ast->getFunctions()) {
            if (func->getName() == "main") {
                mainFunc = func.get();
                break;
            }
        }

        if (mainFunc) {
            cout << "  Main function found:" << endl;
            cout << "    Return type: " << mainFunc->getReturnType()->getTypeName() << endl;
            cout << "    Parameters: " << mainFunc->getParams().size() << endl;

            // 获取函数体中的语句数量
            if (mainFunc->getBody()) {
                cout << "    Block items: " << mainFunc->getBody()->getItems().size() << endl;

                // 查找return语句
                for (const auto& item : mainFunc->getBody()->getItems()) {
                    if (auto stmtItem = dynamic_cast<StmtBlockItemAST*>(item.get())) {
                        if (auto retStmt = dynamic_cast<ReturnStmtAST*>(stmtItem->getStmt())) {
                            cout << "    Return statement found" << endl;
                            if (retStmt->getReturnValue()) {
                                if (auto intExpr = dynamic_cast<IntConstExprAST*>(retStmt->getReturnValue())) {
                                    cout << "    Return value: " << intExpr->getValue() << endl;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            cout << "  Main function not found" << endl;
        }

    } catch (const exception& e) {
        cerr << "✗ Error: " << e.what() << endl;
        // 恢复原始cout
        cout.rdbuf(original_cout);
        return 1;
    }

    cout.rdbuf(original_cout);
    return 0;
}
