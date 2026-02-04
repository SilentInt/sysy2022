
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "antlr4-runtime.h"
#include "frontend/SysYLexer.h"
#include "frontend/SysYParser.h"
#include "ast/ast_builder.h"
#include "codegen/ir_generator.h"
#include <llvm/Support/raw_ostream.h>

using namespace antlr4;
using namespace std;

// 命令行参数结构
struct CompilerOptions {
    string inputFile;
    string outputFile;
    bool dumpAST = false;      // 输出抽象语法树
    bool dumpIR = false;       // 输出LLVM IR
    bool verbose = false;       // 详细输出
    bool help = false;          // 显示帮助

    // 输出文件名
    string astFile;
    string irFile;
};

void printUsage(const char* progName) {
    cout << "SysY Compiler - RISC-V 64 Code Generator (IR Test)" << endl;
    cout << "Usage: " << progName << " <input.sy> [options]" << endl;
    cout << "Options:" << endl;
    cout << "  -o <file>        Specify output LLVM IR file (default: output.ll)" << endl;
    cout << "  --dump-ast       Output abstract syntax tree to <input>.ast" << endl;
    cout << "  --dump-ir        Output LLVM IR to <input>.ll" << endl;
    cout << "  -v, --verbose    Enable verbose output" << endl;
    cout << "  -h, --help       Display this help message" << endl;
    cout << "\nExamples:" << endl;
    cout << "  " << progName << " test.sy                    # Generate test.ll" << endl;
    cout << "  " << progName << " test.sy -o out.ll          # Generate out.ll" << endl;
    cout << "  " << progName << " test.sy --dump-ast --dump-ir  # Debug mode" << endl;
    cout << endl;
}

bool parseArguments(int argc, char* argv[], CompilerOptions& options) {
    if (argc < 2) {
        return false;
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            options.help = true;
            return true;
        }
        else if (arg == "-o") {
            if (i + 1 < argc) {
                options.outputFile = argv[++i];
            } else {
                cerr << "Error: -o requires an argument" << endl;
                return false;
            }
        }
        else if (arg == "--dump-ast") {
            options.dumpAST = true;
        }
        else if (arg == "--dump-ir") {
            options.dumpIR = true;
        }
        else if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        }
        else if (arg[0] != '-') {
            if (options.inputFile.empty()) {
                options.inputFile = arg;
            } else {
                cerr << "Error: Multiple input files specified" << endl;
                return false;
            }
        }
        else {
            cerr << "Error: Unknown option: " << arg << endl;
            return false;
        }
    }

    if (options.inputFile.empty() && !options.help) {
        cerr << "Error: No input file specified" << endl;
        return false;
    }

    return true;
}

void setupOutputFiles(CompilerOptions& options) {
    // 获取输入文件的基础名（不含扩展名）
    string baseName = options.inputFile;
    size_t lastDot = baseName.find_last_of('.');
    if (lastDot != string::npos) {
        baseName = baseName.substr(0, lastDot);
    }

    // 设置默认输出文件名
    if (options.outputFile.empty()) {
        options.irFile = baseName + ".ll";
    } else {
        options.irFile = options.outputFile;
    }

    if (options.dumpAST) {
        options.astFile = baseName + ".ast";
    }
}

void printHeader(const CompilerOptions& options) {
    cout << "========================================" << endl;
    cout << "  SysY Compiler - LLVM IR Test" << endl;
    cout << "========================================" << endl;
    cout << "[+]Input:  " << options.inputFile << endl;
    cout << "[+]Output: " << options.irFile << endl;
    if (options.dumpAST) {
        cout << "[+]AST:    " << options.astFile << endl;
    }
    cout << "========================================" << endl;
    cout << endl;
}

int main(int argc, char *argv[]) {
    CompilerOptions options;

    // 解析命令行参数
    if (!parseArguments(argc, argv, options)) {
        printUsage(argv[0]);
        return 1;
    }

    if (options.help) {
        printUsage(argv[0]);
        return 0;
    }

    // 设置输出文件名
    setupOutputFiles(options);

    // 打印编译信息
    if (options.verbose) {
        printHeader(options);
    }

    // ========================================
    // Step 1: 词法分析和语法分析
    // ========================================
    if (options.verbose) {
        cout << "[1/3] Lexical and Syntax Analysis..." << endl;
    }

    ifstream stream(options.inputFile);
    if (!stream.is_open()) {
        cerr << "[-]Error: Cannot open input file: " << options.inputFile << endl;
        return 1;
    }

    ANTLRInputStream input(stream);
    SysYLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SysYParser parser(&tokens);

    // 检查语法错误
    if (parser.getNumberOfSyntaxErrors() > 0) {
        cerr << "[-]Error: Parsing failed with " << parser.getNumberOfSyntaxErrors()
             << " syntax error(s)" << endl;
        return 1;
    }

    if (options.verbose) {
        cout << "[+]Parsing completed successfully" << endl << endl;
    }

    // ========================================
    // Step 2: 构建抽象语法树 (AST)
    // ========================================
    if (options.verbose) {
        cout << "[2/3] Building Abstract Syntax Tree..." << endl;
    }

    tree::ParseTree *parseTree = parser.compUnit();
    ASTBuilder astBuilder;
    auto astResult = astBuilder.visit(parseTree);

    CompUnitAST* astPtr = std::any_cast<CompUnitAST*>(astResult);
    std::unique_ptr<CompUnitAST> ast(astPtr);

    if (!ast) {
        cerr << "[-]Error: Failed to build AST" << endl;
        return 1;
    }

    if (options.verbose) {
        cout << "[+]AST built successfully" << endl << endl;
    }

    // 输出 AST（如果需要）
    if (options.dumpAST) {
        if (options.verbose) {
            cout << "[+]Writing AST to " << options.astFile << "..." << endl;
        }

        ofstream astOut(options.astFile);
        if (!astOut.is_open()) {
            cerr << "[-]Warning: Cannot open AST output file: " << options.astFile << endl;
        } else {
            // 重定向 cout 到文件
            streambuf* coutBuf = cout.rdbuf();
            cout.rdbuf(astOut.rdbuf());

            ast->print();

            // 恢复 cout
            cout.rdbuf(coutBuf);
            astOut.close();

            if (options.verbose) {
                cout << "[+]AST written to " << options.astFile << endl << endl;
            }
        }
    }

    // ========================================
    // Step 3: 生成 LLVM IR
    // ========================================
    if (options.verbose) {
        cout << "[3/3] Generating LLVM Intermediate Representation..." << endl;
    }

    IRGenerator irGen;
    auto module = irGen.generate(ast.get());

    if (!module) {
        cerr << "[-]Error: Failed to generate LLVM IR" << endl;
        return 1;
    }

    if (options.verbose) {
        cout << "[+]LLVM IR generated successfully" << endl << endl;
    }

    // 输出 IR（总是输出）
    if (options.verbose) {
        cout << "[+]Writing IR to " << options.irFile << "..." << endl;
    }

    std::error_code ec;
    llvm::raw_fd_ostream irOut(options.irFile, ec);

    if (ec) {
        cerr << "[-]Error: Cannot open IR output file: " << ec.message() << endl;
        return 1;
    } else {
        module->print(irOut, nullptr);
        irOut.close();

        if (options.verbose) {
            cout << "[+]LLVM IR written to " << options.irFile << endl << endl;
        }
    }

    // ========================================
    // 完成
    // ========================================
    if (options.verbose) {
        cout << "========================================" << endl;
        cout << "  IR Generation Successful!" << endl;
        cout << "========================================" << endl;
        cout << endl;
        cout << "Generated files:" << endl;
        if (options.dumpAST) {
            cout << "  - AST:      " << options.astFile << endl;
        }
        cout << "  - LLVM IR:  " << options.irFile << endl;
    } else {
        // 简洁模式：只输出成功信息
        cout << "Compiled " << options.inputFile << " -> " << options.irFile << endl;
    }

    return 0;
}
