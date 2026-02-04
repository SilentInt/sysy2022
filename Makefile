# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include/antlr4-runtime -I. -Ifrontend -g

# LLVM 配置
LLVM_CONFIG = llvm-config-17
LLVM_CXXFLAGS = $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_CXXFLAGS := $(filter-out -fno-exceptions,$(LLVM_CXXFLAGS))
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --system-libs --libs core)

# 链接器设置
LDFLAGS = -L/usr/local/lib
LDLIBS = -lantlr4-runtime

# ANTLR 生成的源文件
ANTLR_SOURCES = frontend/SysYLexer.cpp \
                frontend/SysYParser.cpp \
                frontend/SysYBaseVisitor.cpp \
                frontend/SysYVisitor.cpp

# 检查文件是否存在并过滤
ANTLR_SOURCES := $(wildcard $(ANTLR_SOURCES))
ANTLR_OBJECTS = $(ANTLR_SOURCES:.cpp=.o)

# Codegen 源文件 - 使用 wildcard 自动查找
CODEGEN_SOURCES = $(wildcard codegen/ir_generator.cpp)
CODEGEN_OBJECTS = $(CODEGEN_SOURCES:.cpp=.o)

# Backend 源文件 - 使用 wildcard 自动查找
BACKEND_SOURCES = $(wildcard codegen/riscv_backend.cpp)
BACKEND_OBJECTS = $(BACKEND_SOURCES:.cpp=.o)

# 主程序源文件
MAIN_SOURCE = main.cpp
MAIN_OBJECT = main.o

# 前后端依赖头文件
ANTLR_HEADERS = frontend/SysYLexer.h frontend/SysYParser.h
AST_HEADERS = ast/ast.h ast/ast_builder.h
BACKEND_HEADERS = codegen/riscv_backend.h
CODEGEN_HEADERS = codegen/ir_generator.h

# 所有头文件
HEADERS = $(ANTLR_HEADERS) $(AST_HEADERS) $(CODEGEN_HEADERS)  $(BACKEND_HEADERS) 

# 所有对象文件
OBJECTS = $(MAIN_OBJECT) $(ANTLR_OBJECTS) $(CODEGEN_OBJECTS) $(BACKEND_OBJECTS)

# 目标可执行文件
TARGET = compiler

# AST测试文件
TEST_AST_SOURCE = test/test_ast_generator.cpp
TEST_AST_OBJECT = $(TEST_AST_SOURCE:.cpp=.o) 
TEST_AST_TARGET = test_ast
TEST_FILE = test/examples/test_complex.sy


# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LLVM_LDFLAGS) $(LDLIBS)
	@echo "Build successful!"

# 编译主程序
$(MAIN_OBJECT): $(MAIN_SOURCE) $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

# 编译 ANTLR 生成的文件
frontend/%.o: frontend/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译 Codegen 文件
codegen/ir_generator.o: codegen/ir_generator.cpp ast/ast.h	codegen/ir_generator.h
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

# 编译 Backend 文件
codegen/riscv_backend.o: codegen/riscv_backend.cpp codegen/riscv_backend.h
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@


#=========================== AST 测试 =====================
.PHONY: test-ast
test-ast: $(TEST_AST_TARGET)
	@echo "Running AST test..."
	./$< $(TEST_FILE)

$(TEST_AST_TARGET): $(TEST_AST_OBJECT) $(ANTLR_OBJECTS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS)  -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "Build successful!"

$(TEST_AST_OBJECT): $(TEST_AST_SOURCE) $(AST_HEADERS) $(ANTLR_HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS)   -c $< -o $@

#==========================================================

#=========================== IR 测试 ======================
# IR测试文件
TEST_IR_SOURCE = test/test_ir.cpp
TEST_IR_OBJECT = $(TEST_IR_SOURCE:.cpp=.o)
TEST_IR_TARGET = test_ir

.PHONY: test-ir
test-ir: $(TEST_IR_TARGET)
	@echo "Running IR test..."
	./$< $(TEST_FILE)

$(TEST_IR_TARGET): $(TEST_IR_OBJECT) $(ANTLR_OBJECTS) $(CODEGEN_OBJECTS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LLVM_LDFLAGS) $(LDLIBS)
	@echo "Build successful!"

$(TEST_IR_OBJECT): $(TEST_IR_SOURCE) $(AST_HEADERS) $(ANTLR_HEADERS) $(CODEGEN_HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

#==========================================================




# 深度清理(包括 ANTLR 生成的文件)
.PHONY: distclean
distclean: clean
	@echo "Deep cleaning..."
	rm -f frontend/SysYLexer.* frontend/SysYParser.* 
	rm -f frontend/SysYVisitor.* frontend/SysYBaseVisitor.*
	@echo "Deep clean complete!"



#=========================== 编译测试文件 ======================
# 查找所有测试文件
SY_FILES = $(wildcard test/examples_final/*.sy)
# 对应的汇编文件名（保存在test_res目录中）
ASM_FILES = $(patsubst test/examples_final/%.sy,test_res/%.s,$(SY_FILES))

# 创建test_res目录
test_res:
	@mkdir -p test_res

# 编译所有.sy文件为汇编代码
.PHONY: compile-tests
compile-tests: $(TARGET) test_res
	@rm -f errorlog.txt
	@echo "Compiling all test files with optimization level O$(OPT_LEVEL)..."
	@$(MAKE) --no-print-directory $(ASM_FILES) OPT_LEVEL=$(OPT_LEVEL)
	@echo "All test files compiled to assembly!"

# 单个.sy文件编译规则
test_res/%.s: test/examples_final/%.sy $(TARGET)
	@echo "Compiling $< to assembly with O$(OPT_LEVEL)..."
	@mkdir -p test_res
	@-./$(TARGET) $< -o $@ -O$(OPT_LEVEL) 2>&1 >/dev/null | sed "s|^|[$(notdir $<)] |" >> errorlog.txt
	@if [ $$? -eq 0 ]; then \
		echo "Generated $@"; \
		chmod -R 777 test_res/; \
	else \
		echo "Failed to compile $<, error logged to errorlog.txt"; \
	fi

# 优化级别默认值
OPT_LEVEL ?= 0

#==========================================================

# 显示编译器版本
.PHONY: version
version:
	@echo "=== Compiler Versions ==="
	@$(CXX) --version
	@echo ""
	@echo "=== LLVM Version ==="
	@$(LLVM_CONFIG) --version

# 修改clean目标，添加test_res目录清理
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	rm -f $(TEST_AST_TARGET) $(TEST_AST_OBJECT)
	rm -f $(TEST_IR_TARGET) $(TEST_IR_OBJECT)
	rm -f *.o frontend/*.o codegen/*.o
	rm -f *.ast *.ll *.s
	rm -rf test_res
	rm -f errorlog.txt
	@echo "Clean complete!"

