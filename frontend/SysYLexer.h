
// Generated from SysY.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  SysYLexer : public antlr4::Lexer {
public:
  enum {
    CONST = 1, INT = 2, FLOAT = 3, VOID = 4, VECTOR = 5, IF = 6, ELSE = 7, 
    WHILE = 8, BREAK = 9, CONTINUE = 10, RETURN = 11, PLUS = 12, MINUS = 13, 
    MUL = 14, DIV = 15, MOD = 16, ASSIGN = 17, EQ = 18, NE = 19, LT = 20, 
    GT = 21, LE = 22, GE = 23, NOT = 24, AND = 25, OR = 26, COMMA = 27, 
    SEMICOLON = 28, LPAREN = 29, RPAREN = 30, LBRACK = 31, RBRACK = 32, 
    LBRACE = 33, RBRACE = 34, IDENT = 35, IntConst = 36, FloatConst = 37, 
    StringLiteral = 38, LINE_COMMENT = 39, BLOCK_COMMENT = 40, WS = 41
  };

  explicit SysYLexer(antlr4::CharStream *input);

  ~SysYLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

