grammar SysY;

// --- Parser Rules --- 

// 编译单元 (开始符号)
compUnit: (decl | funcDef)+;

// 声明
decl: constDecl | varDecl;

// 常量声明
constDecl: CONST bType constDef (COMMA constDef)* SEMICOLON;

// 基本类型
bType: INT | FLOAT | vectorType;

// 向量类型
vectorType: VECTOR LT (INT | FLOAT) COMMA constExp GT;

// 常数定义
constDef: IDENT (LBRACK constExp RBRACK)* ASSIGN constInitVal;

// 常量初值
constInitVal
    : constExp
    | LBRACE (constInitVal (COMMA constInitVal)*)? RBRACE
    ;

// 变量声明
varDecl: bType varDef (COMMA varDef)* SEMICOLON;

// 变量定义
varDef
    : IDENT (LBRACK constExp RBRACK)*
    | IDENT (LBRACK constExp RBRACK)* ASSIGN initVal
    ;

// 变量初值
initVal
    : exp
    | LBRACE (initVal (COMMA initVal)*)? RBRACE
    ;

// 函数定义
funcDef: funcType IDENT LPAREN (funcFParams)? RPAREN block;

// 函数类型
funcType: VOID | INT | FLOAT | vectorType;

// 函数形参表
funcFParams: funcFParam (COMMA funcFParam)*;

// 函数形参
funcFParam: bType IDENT (LBRACK RBRACK (LBRACK exp RBRACK)*)?;

// 语句块
block: LBRACE (blockItem)* RBRACE;

// 语句块项
blockItem: decl | stmt;

// 语句
stmt
    : lVal ASSIGN exp SEMICOLON
    | (exp)? SEMICOLON
    | block
    | IF LPAREN cond RPAREN stmt (ELSE stmt)?
    | WHILE LPAREN cond RPAREN stmt
    | BREAK SEMICOLON
    | CONTINUE SEMICOLON
    | RETURN (exp)? SEMICOLON
    ;

// 表达式
exp: addExp;

// 条件表达式
cond: lOrExp;

// 左值表达式
lVal: IDENT (LBRACK exp RBRACK)*;

// 基本表达式
primaryExp
    : LPAREN exp RPAREN
    | lVal
    | number
    | StringLiteral  // 添加字符串字面量支持
    ;

// 数值
number: IntConst | FloatConst;

// 一元表达式
unaryExp
    : primaryExp
    | IDENT LPAREN (funcRParams)? RPAREN
    | unaryOp unaryExp
    ;

// 单目运算符
unaryOp: PLUS | MINUS | NOT;

// 函数实参表
funcRParams: exp (COMMA exp)*;

// 乘除模表达式
mulExp
    : unaryExp
    | mulExp (MUL | DIV | MOD) unaryExp
    ;

// 加减表达式
addExp
    : mulExp
    | addExp (PLUS | MINUS) mulExp
    ;

// 关系表达式
relExp
    : addExp
    | relExp (LT | GT | LE | GE) addExp
    ;

// 相等性表达式
eqExp
    : relExp
    | eqExp (EQ | NE) relExp
    ;

// 逻辑与表达式
lAndExp
    : eqExp
    | lAndExp AND eqExp
    ;

// 逻辑或表达式
lOrExp
    : lAndExp
    | lOrExp OR lAndExp
    ;

// 常量表达式
constExp: addExp;

// --- Lexer Rules --- 

// 关键字
CONST: 'const';
INT: 'int';
FLOAT: 'float';
VOID: 'void';
VECTOR: 'vector';
IF: 'if';
ELSE: 'else';
WHILE: 'while';
BREAK: 'break';
CONTINUE: 'continue';
RETURN: 'return';

// 运算符
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
MOD: '%';
ASSIGN: '=';
EQ: '==';
NE: '!=';
LT: '<';
GT: '>';
LE: '<=';
GE: '>=';
NOT: '!';
AND: '&&';
OR: '||';

// 分隔符
COMMA: ',';
SEMICOLON: ';';
LPAREN: '(';
RPAREN: ')';
LBRACK: '[';
RBRACK: ']';
LBRACE: '{';
RBRACE: '}';

// 标识符 (Ident)
IDENT: [a-zA-Z_] [a-zA-Z_0-9]*;

// 整型常量 (IntConst)
IntConst
    : DecimalConst
    | OctalConst
    | HexadecimalConst
    ;

fragment DecimalConst: [1-9] [0-9]*;
fragment OctalConst: '0' [0-7]*;
fragment HexadecimalConst: ('0x' | '0X') [0-9a-fA-F]+;

// 浮点型常量 (FloatConst)
FloatConst
    : HexadecimalFloatConst
    | Fractionalconst Exponentpart?
    | Digitsequence Exponentpart
    ;

fragment HexadecimalFloatConst
    : ('0x' | '0X') HexDigitSequence? '.' HexDigitSequence HexExponentpart?
    | ('0x' | '0X') HexDigitSequence '.' HexExponentpart?
    | ('0x' | '0X') HexDigitSequence HexExponentpart
    ;

fragment HexDigitSequence: [0-9a-fA-F]+;

fragment HexExponentpart
    : [pP] [+-]? Digitsequence
    ;

// 字符串字面量
StringLiteral: '"' (ESC | ~["\\\r\n])* '"' | '\'' (ESC | ~['\\\r\n])* '\'';

fragment ESC: '\\' (["\\/bfnrt] | UNICODE);
fragment UNICODE: 'u' HEX HEX HEX HEX;
fragment HEX: [0-9a-fA-F];

fragment Fractionalconst
    : Digitsequence? '.' Digitsequence
    | Digitsequence '.'
    ;

fragment Exponentpart
    : [eE] [+-]? Digitsequence
    ;

fragment Digitsequence: [0-9]+;

// 注释
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;

// 空白字符
WS: [ \t\r\n]+ -> skip;
