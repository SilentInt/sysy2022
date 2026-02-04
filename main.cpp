#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "antlr4-runtime.h"
#include "frontend/SysYLexer.h"
#include "frontend/SysYParser.h"
#include "ast/ast_builder.h"
#include "ast/ast_optimizer.h"
#include "codegen/ir_generator.h"
#include "codegen/riscv_backend.h"
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
    int optLevel = 0;           // 优化级别：0-3，对应O0-O3
    
    // 输出文件名
    string astFile;
    string irFile;
    string asmFile;
};

void printUsage(const char* progName) {
    cout << "SysY Compiler - RISC-V 64 Code Generator\n" << endl;
    cout << "Usage: " << progName << " <input.sy> [options]\n" << endl;
    cout << "Options:" << endl;
    cout << "  -o <file>        Specify output assembly file (default: output.s)" << endl;
    cout << "  --dump-ast       Output abstract syntax tree to <input>.ast" << endl;
    cout << "  --dump-ir        Output LLVM IR to <input>.ll" << endl;
    cout << "  -O <level>       Optimization level (0-3, default: O0)" << endl;
    cout << "  -v, --verbose    Enable verbose output" << endl;
    cout << "  -h, --help       Display this help message" << endl;
    cout << "\nExamples:" << endl;
    cout << "  " << progName << " test.sy                    # Generate test.s" << endl;
    cout << "  " << progName << " test.sy -o out.s          # Generate out.s" << endl;
    cout << "  " << progName << " test.sy -O2               # Generate optimized code with O2" << endl;
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
        else if (arg == "-O") {
            if (i + 1 < argc) {
                string optStr = argv[++i];
                try {
                    options.optLevel = stoi(optStr);
                    if (options.optLevel < 0 || options.optLevel > 3) {
                        cerr << "Error: Optimization level must be between 0 and 3" << endl;
                        return false;
                    }
                } catch (const invalid_argument&) {
                    cerr << "Error: Invalid optimization level: " << optStr << endl;
                    return false;
                }
            } else {
                cerr << "Error: -O requires an argument" << endl;
                return false;
            }
        }
        else if (arg.substr(0, 2) == "-O") {
            // 处理 -O1, -O2, -O3 这种合并形式
            string optStr = arg.substr(2);
            try {
                options.optLevel = stoi(optStr);
                if (options.optLevel < 0 || options.optLevel > 3) {
                    cerr << "Error: Optimization level must be between 0 and 3" << endl;
                    return false;
                }
            } catch (const invalid_argument&) {
                cerr << "Error: Invalid optimization level: " << optStr << endl;
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
        options.asmFile = baseName + ".s";
    } else {
        options.asmFile = options.outputFile;
    }
    
    if (options.dumpAST) {
        options.astFile = baseName + ".ast";
    }
    
    if (options.dumpIR) {
        options.irFile = baseName + ".ll";
    }
}

void printHeader(const CompilerOptions& options) {
    cout << "========================================" << endl;
    cout << "  SysY Compiler - RISC-V 64 Backend" << endl;
    cout << "========================================" << endl;
    cout << "[+]Input:  " << options.inputFile << endl;
    cout << "[+]Output: " << options.asmFile << endl;
    cout << "[+]Opt:    O" << options.optLevel << endl;
    if (options.dumpAST) {
        cout << "[+]AST:    " << options.astFile << endl;
    }
    if (options.dumpIR) {
        cout << "[+]IR:     " << options.irFile << endl;
    }
    cout << "========================================" << endl;
    cout << endl;
}

int main(int argc, char *argv[]) {
    try {
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
            cout << "[1/4] Lexical and Syntax Analysis..." << endl;
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
            cout << "[2/4] Building Abstract Syntax Tree..." << endl;
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
        
        // ========================================
        // Step 2.5: AST优化
        // ========================================
        if (options.verbose) {
            cout << "[2.5/4] Optimizing Abstract Syntax Tree..." << endl;
        }
        
        ASTOptimizer optimizer(options.verbose);
        optimizer.optimize(ast.get());
        
        if (options.verbose) {
            cout << "[+]AST optimized successfully" << endl << endl;
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
            cout << "[3/4] Generating LLVM Intermediate Representation..." << endl;
        }
        
        IRGenerator irGen;
        
        // 声明运行时库函数
        irGen.declareLibraryFunctions();
        
        auto module = irGen.generate(ast.get());
        
        if (!module) {
            cerr << "[-]Error: Failed to generate LLVM IR" << endl;
            return 1;
        }
        
        if (options.verbose) {
            cout << "[+]LLVM IR generated successfully" << endl << endl;
        }
        
        // 输出 IR（如果需要）
        if (options.dumpIR) {
            if (options.verbose) {
                cout << "[+]Writing IR to " << options.irFile << "..." << endl;
            }
            
            std::error_code ec;
            llvm::raw_fd_ostream irOut(options.irFile, ec);
            
            if (ec) {
                cerr << "[-]Warning: Cannot open IR output file: " << ec.message() << endl;
            } else {
                module->print(irOut, nullptr);
                irOut.close();
                
                if (options.verbose) {
                    cout << "[+]LLVM IR written to " << options.irFile << endl << endl;
                }
            }
        }
        
        // ========================================
        // Step 4: 生成 RISC-V 64 汇编代码
        // ========================================
        if (options.verbose) {
            cout << "[4/4] Generating RISC-V 64 Assembly..." << endl;
        }
        
        // 初始化 RISC-V 目标
        if (!RISCVBackend::initializeTarget()) {
            cerr << "[-]Error: Failed to initialize RISC-V target" << endl;
            return 1;
        }
        
        RISCVBackend backend(options.optLevel);
        
        if (!backend.generateAssembly(module.get(), options.asmFile)) {
            cerr << "[-]Error: Failed to generate RISC-V assembly" << endl;
            return 1;
        }
        
        if (options.verbose) {
            cout << "[+]RISC-V assembly written to " << options.asmFile << endl << endl;
        }
        
        // ========================================
        // 完成
        // ========================================
        if (options.verbose) {
            cout << "========================================" << endl;
            cout << "  Compilation Successful!" << endl;
            cout << "========================================" << endl;
            cout << endl;
            cout << "Generated files:" << endl;
            if (options.dumpAST) {
                cout << "  - AST:      " << options.astFile << endl;
            }
            if (options.dumpIR) {
                cout << "  - LLVM IR:  " << options.irFile << endl;
            }
            cout << "  - Assembly: " << options.asmFile << endl;
        } else {
            // 简洁模式：只输出成功信息
            cout << "Compiled " << options.inputFile << " -> " << options.asmFile << endl;
        }
        
            return 0;
    } catch (const std::exception& e) {
        cerr << "[-]Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "[-]Error: Unknown exception occurred" << endl;
        return 1;
    }
}
