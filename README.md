# SysY Compiler - RISC-V 64 Backend

这是一个完整的 SysY 语言编译器实现，可以将 SysY 源代码编译为 RISC-V 64 汇编代码。编译器采用多阶段设计，包含词法分析、语法分析、AST 构建、AST 优化、LLVM IR 生成和最终的 RISC-V 汇编代码生成。

## 当前项目结构（updated：2026/02/05）

```
├── ast/                     # 抽象语法树相关实现
│   ├── ast.h               # AST 节点定义
│   ├── ast_builder.h       # AST 构建器（基于 ANTLR Visitor）
│   ├── ast_optimizer.h     # AST 优化器
│   ├── constant_folding.h  # 常量折叠优化
│   └── loop_optimization.h # 循环优化（预留）
├── codegen/                # 代码生成相关实现
│   ├── ir_generator.cpp/h  # LLVM IR 生成器
│   └── riscv_backend.cpp/h # RISC-V 后端
├── frontend/               # ANTLR 生成的前端代码（由 antlr_generate.sh 生成）
│   ├── SysYLexer.cpp/h
│   ├── SysYParser.cpp/h
│   ├── SysYBaseVisitor.cpp/h
│   ├── SysYVisitor.cpp/h
│   ├── SysYBaseListener.cpp/h
│   ├── SysYListener.cpp/h
│   ├── SysY.interp
│   └── SysY.tokens
├── test/                   # 测试用例
│   └── vector/             # 向量相关测试
├── antlr_generate.sh       # 生成前端代码脚本
├── main.cpp                # 主程序入口
├── Makefile                # 构建配置
└── SysY.g4                 # ANTLR4 语法定义文件
```

## 目前实现的功能特性

- **完整的 SysY 语言支持（含扩展）**
  
  - 变量和常量声明与定义
  - 整数和浮点数类型支持
  - 数组支持（一维和多维）
  - 向量类型支持（定长向量）
  - 完整的表达式支持：
    - 算术运算（+、-、*、/、%）
      - 向量算术运算（+、-、*、/，同类型同长度逐元素运算）
      - 向量-标量广播运算（+、-、*、/、%，支持标量自动广播）
    - 关系运算（<、>、<=、>=）
    - 相等性运算（==、!=）
    - 逻辑运算（&&、||、!）
    - 一元运算（+、-、!）
    - 优先级和结合性处理
  - 完整的语句支持：
    - 赋值语句
    - 表达式语句
    - 条件语句（if-else）
    - 循环语句（while）
    - 跳转语句（break、continue）
    - 返回语句（return）
  - 函数定义和调用
    - 支持不同的返回类型（int、float、void）
    - 支持参数传递
    - 支持数组参数
  - 初始化值支持
    - 表达式初始化
    - 列表初始化（数组）

- **AST 优化**
  
  - 常量折叠优化
  - 循环优化（预留接口，未启用）
  - 多轮优化支持（最多 8 轮）

- **基于 ANTLR4 的词法和语法分析**
  
  - 完整的 SysY 语法规则定义在 SysY.g4 中
  - 自动生成的词法分析器和语法分析器

- **完整的 AST 结构设计**
  
  - 分层设计的 AST 节点
  - 支持所有 SysY 语言构造
  - 方便遍历和生成代码

- **LLVM IR 中间代码生成**
  
  - 完整的表达式生成
  - 语句生成（if、while、return、break、continue 等）
  - 函数生成
  - 声明生成
  - 数组支持
  - 初始化值生成
  - 内建函数：`vsum`（向量元素求和）

- **RISC-V 64 目标汇编代码生成**
  
  - 基于 LLVM 的 RISC-V 后端
  - 生成高效的 RISC-V 64 汇编代码
  - 支持多级 CodeGen 优化（O0-O3）

- **调试输出支持**
  
  - `--dump-ast`：输出抽象语法树到文件
  - `--dump-ir`：输出 LLVM IR 到文件
  - `-v, --verbose`：详细输出编译过程



## 编译要求

- C++17 或更高版本
- LLVM 17（通过 `llvm-config-17`）
- ANTLR4（命令行工具 `antlr4` + C++ 运行时库）
- Make 构建系统

## 环境安装
- 下载antlr C++ target runtime并根据readme编译安装
https://www.antlr.org/download.html
- 使用apt等包管理器安装antlr4工具库

## 构建方法

```bash
# 生成 ANTLR 前端代码（首次或语法更新后）
./antlr_generate.sh

# 编译编译器
make

# 清理构建产物
make clean

# 深度清理（包括 ANTLR 生成的文件）
make distclean

# 查看编译器版本
make version
```
  

## 使用方法

### 基本用法

```bash
# 编译源文件
./compiler test.sy

# 指定输出文件
./compiler test.sy -o output.s

# 调试模式
./compiler test.sy --dump-ast --dump-ir -v
```

## 向量类型（扩展语法）

向量是定长、同元素类型的值类型，支持逐元素算术运算、标量广播运算，以及对向量内所有元素求和的运算。

- 语法：`vector<元素类型, 长度>`
- 元素类型：`int` 或 `float`
- 长度：常量表达式（编译期可求值）

示例：

- `vector<int, 4> a;`
- `vector<float, 8> b = {1.0, 2.0, 3.0, 4.0};`
- `vector<float, 4> c = a * b;`  // 逐元素乘法
- `vector<int, 4> d = a + a;`    // 逐元素加法
- `vector<int, 4> e = d + 2;`    // 标量广播
- `int s = vsum(d);`             // 向量元素求和（float 向量返回 float）

约束与说明：

- 向量之间的 `+`、`-`、`*`、`/` 仅支持同长度、同元素类型
- 向量与标量支持 `+`、`-`、`*`、`/`、`%`（浮点向量不支持 `%`）
- 向量不能出现在条件表达式中
- 向量支持一维下标访问与赋值：`v[i]` / `v[i] = x`
- 向量不能再叠加数组维度（不支持 `vector<...>[N]`）

### 命令行选项

- `-o <file>`: 指定输出汇编文件（默认：<input>.s）
- `-O <level>`: 优化级别（0-3，默认：O0）
    - 也支持 `-O1` / `-O2` / `-O3` 形式
  - O0: 无优化
  - O1: 基础优化
  - O2: 中级优化
  - O3: 高级优化
- `--dump-ast`：输出抽象语法树到 \<input>.ast 文件
- `--dump-ir`：输出 LLVM IR 到 \<input>.ll 文件
- `-v, --verbose`：启用详细输出
- `-h, --help`：显示帮助信息

## 编译流程

1. **词法分析和语法分析**
   
   - 使用 ANTLR4 进行词法分析
   - 构建语法分析树

2. **AST 构建**
   
   - 将语法树转换为抽象语法树
   - 支持类型检查和初步的语义分析

3. **AST 优化**
   
   - 常量折叠优化
   - 循环优化（预留）
   - 多轮迭代优化

4. **LLVM IR 生成**
   
   - 进行语义分析和添加语义约束
   - 将 AST 转换为 LLVM IR

5. **RISC-V 代码生成**
   
   - 将 LLVM IR 转换为 RISC-V 64 汇编
   - 应用指定的优化级别

## 测试用例

当前仓库包含向量相关测试，位于 `test/vector/`，每个用例包含 `.sy` 源文件与参考 `.s` 汇编输出。

## 示例代码

```SysY2022
// 简单的 main 函数
int main(){
    return 42;
}

// 带变量和表达式的程序
int main() {
    int a = 10;
    int b = 20;
    int c = a + b * 2;
    return c;
}

// 带数组的程序
int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    int sum = 0;
    int i = 0;
    while (i < 5) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

// 使用运行时库函数
int main() {
    int n = getint();
    putint(n);
    return 0;
}

// 带循环优化的程序
int main() {
    int sum = 0;
    int i = 0;
    while (i < 100) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}
```

## 注意事项

1. 确保已正确安装所有依赖
2. 输入文件必须以 `.sy` 为扩展名
3. 默认输出文件名为 `<input>.s`
4. 调试模式会生成额外的中间文件
5. 支持的 SysY 语言特性已完整实现
6. 优化级别说明：
   - O0：无优化，编译速度快，适合调试
   - O1-O3：逐步提高优化级别，生成更高效的代码


