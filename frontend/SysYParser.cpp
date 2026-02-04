
// Generated from SysY.g4 by ANTLR 4.13.2


#include "SysYListener.h"
#include "SysYVisitor.h"

#include "SysYParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SysYParserStaticData final {
  SysYParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SysYParserStaticData(const SysYParserStaticData&) = delete;
  SysYParserStaticData(SysYParserStaticData&&) = delete;
  SysYParserStaticData& operator=(const SysYParserStaticData&) = delete;
  SysYParserStaticData& operator=(SysYParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sysyParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<SysYParserStaticData> sysyParserStaticData = nullptr;

void sysyParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sysyParserStaticData != nullptr) {
    return;
  }
#else
  assert(sysyParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SysYParserStaticData>(
    std::vector<std::string>{
      "compUnit", "decl", "constDecl", "bType", "vectorType", "constDef", 
      "constInitVal", "varDecl", "varDef", "initVal", "funcDef", "funcType", 
      "funcFParams", "funcFParam", "block", "blockItem", "stmt", "exp", 
      "cond", "lVal", "primaryExp", "number", "unaryExp", "unaryOp", "funcRParams", 
      "mulExp", "addExp", "relExp", "eqExp", "lAndExp", "lOrExp", "constExp"
    },
    std::vector<std::string>{
      "", "'const'", "'int'", "'float'", "'void'", "'vector'", "'if'", "'else'", 
      "'while'", "'break'", "'continue'", "'return'", "'+'", "'-'", "'*'", 
      "'/'", "'%'", "'='", "'=='", "'!='", "'<'", "'>'", "'<='", "'>='", 
      "'!'", "'&&'", "'||'", "','", "';'", "'('", "')'", "'['", "']'", "'{'", 
      "'}'"
    },
    std::vector<std::string>{
      "", "CONST", "INT", "FLOAT", "VOID", "VECTOR", "IF", "ELSE", "WHILE", 
      "BREAK", "CONTINUE", "RETURN", "PLUS", "MINUS", "MUL", "DIV", "MOD", 
      "ASSIGN", "EQ", "NE", "LT", "GT", "LE", "GE", "NOT", "AND", "OR", 
      "COMMA", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", 
      "RBRACE", "IDENT", "IntConst", "FloatConst", "StringLiteral", "LINE_COMMENT", 
      "BLOCK_COMMENT", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,41,379,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,1,0,1,0,4,0,67,8,0,11,0,12,0,68,1,1,
  	1,1,3,1,73,8,1,1,2,1,2,1,2,1,2,1,2,5,2,80,8,2,10,2,12,2,83,9,2,1,2,1,
  	2,1,3,1,3,1,3,3,3,90,8,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,
  	1,5,5,5,104,8,5,10,5,12,5,107,9,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,5,6,
  	117,8,6,10,6,12,6,120,9,6,3,6,122,8,6,1,6,3,6,125,8,6,1,7,1,7,1,7,1,7,
  	5,7,131,8,7,10,7,12,7,134,9,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,5,8,143,8,8,
  	10,8,12,8,146,9,8,1,8,1,8,1,8,1,8,1,8,5,8,153,8,8,10,8,12,8,156,9,8,1,
  	8,1,8,3,8,160,8,8,1,9,1,9,1,9,1,9,1,9,5,9,167,8,9,10,9,12,9,170,9,9,3,
  	9,172,8,9,1,9,3,9,175,8,9,1,10,1,10,1,10,1,10,3,10,181,8,10,1,10,1,10,
  	1,10,1,11,1,11,1,11,1,11,3,11,190,8,11,1,12,1,12,1,12,5,12,195,8,12,10,
  	12,12,12,198,9,12,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,5,13,208,8,
  	13,10,13,12,13,211,9,13,3,13,213,8,13,1,14,1,14,5,14,217,8,14,10,14,12,
  	14,220,9,14,1,14,1,14,1,15,1,15,3,15,226,8,15,1,16,1,16,1,16,1,16,1,16,
  	1,16,3,16,234,8,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,3,16,
  	245,8,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,
  	3,16,259,8,16,1,16,3,16,262,8,16,1,17,1,17,1,18,1,18,1,19,1,19,1,19,1,
  	19,1,19,5,19,273,8,19,10,19,12,19,276,9,19,1,20,1,20,1,20,1,20,1,20,1,
  	20,1,20,3,20,285,8,20,1,21,1,21,1,22,1,22,1,22,1,22,3,22,293,8,22,1,22,
  	1,22,1,22,1,22,3,22,299,8,22,1,23,1,23,1,24,1,24,1,24,5,24,306,8,24,10,
  	24,12,24,309,9,24,1,25,1,25,1,25,1,25,1,25,1,25,5,25,317,8,25,10,25,12,
  	25,320,9,25,1,26,1,26,1,26,1,26,1,26,1,26,5,26,328,8,26,10,26,12,26,331,
  	9,26,1,27,1,27,1,27,1,27,1,27,1,27,5,27,339,8,27,10,27,12,27,342,9,27,
  	1,28,1,28,1,28,1,28,1,28,1,28,5,28,350,8,28,10,28,12,28,353,9,28,1,29,
  	1,29,1,29,1,29,1,29,1,29,5,29,361,8,29,10,29,12,29,364,9,29,1,30,1,30,
  	1,30,1,30,1,30,1,30,5,30,372,8,30,10,30,12,30,375,9,30,1,31,1,31,1,31,
  	0,6,50,52,54,56,58,60,32,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,
  	34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,0,7,1,0,2,3,1,0,36,37,2,
  	0,12,13,24,24,1,0,14,16,1,0,12,13,1,0,20,23,1,0,18,19,396,0,66,1,0,0,
  	0,2,72,1,0,0,0,4,74,1,0,0,0,6,89,1,0,0,0,8,91,1,0,0,0,10,98,1,0,0,0,12,
  	124,1,0,0,0,14,126,1,0,0,0,16,159,1,0,0,0,18,174,1,0,0,0,20,176,1,0,0,
  	0,22,189,1,0,0,0,24,191,1,0,0,0,26,199,1,0,0,0,28,214,1,0,0,0,30,225,
  	1,0,0,0,32,261,1,0,0,0,34,263,1,0,0,0,36,265,1,0,0,0,38,267,1,0,0,0,40,
  	284,1,0,0,0,42,286,1,0,0,0,44,298,1,0,0,0,46,300,1,0,0,0,48,302,1,0,0,
  	0,50,310,1,0,0,0,52,321,1,0,0,0,54,332,1,0,0,0,56,343,1,0,0,0,58,354,
  	1,0,0,0,60,365,1,0,0,0,62,376,1,0,0,0,64,67,3,2,1,0,65,67,3,20,10,0,66,
  	64,1,0,0,0,66,65,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,
  	1,1,0,0,0,70,73,3,4,2,0,71,73,3,14,7,0,72,70,1,0,0,0,72,71,1,0,0,0,73,
  	3,1,0,0,0,74,75,5,1,0,0,75,76,3,6,3,0,76,81,3,10,5,0,77,78,5,27,0,0,78,
  	80,3,10,5,0,79,77,1,0,0,0,80,83,1,0,0,0,81,79,1,0,0,0,81,82,1,0,0,0,82,
  	84,1,0,0,0,83,81,1,0,0,0,84,85,5,28,0,0,85,5,1,0,0,0,86,90,5,2,0,0,87,
  	90,5,3,0,0,88,90,3,8,4,0,89,86,1,0,0,0,89,87,1,0,0,0,89,88,1,0,0,0,90,
  	7,1,0,0,0,91,92,5,5,0,0,92,93,5,20,0,0,93,94,7,0,0,0,94,95,5,27,0,0,95,
  	96,3,62,31,0,96,97,5,21,0,0,97,9,1,0,0,0,98,105,5,35,0,0,99,100,5,31,
  	0,0,100,101,3,62,31,0,101,102,5,32,0,0,102,104,1,0,0,0,103,99,1,0,0,0,
  	104,107,1,0,0,0,105,103,1,0,0,0,105,106,1,0,0,0,106,108,1,0,0,0,107,105,
  	1,0,0,0,108,109,5,17,0,0,109,110,3,12,6,0,110,11,1,0,0,0,111,125,3,62,
  	31,0,112,121,5,33,0,0,113,118,3,12,6,0,114,115,5,27,0,0,115,117,3,12,
  	6,0,116,114,1,0,0,0,117,120,1,0,0,0,118,116,1,0,0,0,118,119,1,0,0,0,119,
  	122,1,0,0,0,120,118,1,0,0,0,121,113,1,0,0,0,121,122,1,0,0,0,122,123,1,
  	0,0,0,123,125,5,34,0,0,124,111,1,0,0,0,124,112,1,0,0,0,125,13,1,0,0,0,
  	126,127,3,6,3,0,127,132,3,16,8,0,128,129,5,27,0,0,129,131,3,16,8,0,130,
  	128,1,0,0,0,131,134,1,0,0,0,132,130,1,0,0,0,132,133,1,0,0,0,133,135,1,
  	0,0,0,134,132,1,0,0,0,135,136,5,28,0,0,136,15,1,0,0,0,137,144,5,35,0,
  	0,138,139,5,31,0,0,139,140,3,62,31,0,140,141,5,32,0,0,141,143,1,0,0,0,
  	142,138,1,0,0,0,143,146,1,0,0,0,144,142,1,0,0,0,144,145,1,0,0,0,145,160,
  	1,0,0,0,146,144,1,0,0,0,147,154,5,35,0,0,148,149,5,31,0,0,149,150,3,62,
  	31,0,150,151,5,32,0,0,151,153,1,0,0,0,152,148,1,0,0,0,153,156,1,0,0,0,
  	154,152,1,0,0,0,154,155,1,0,0,0,155,157,1,0,0,0,156,154,1,0,0,0,157,158,
  	5,17,0,0,158,160,3,18,9,0,159,137,1,0,0,0,159,147,1,0,0,0,160,17,1,0,
  	0,0,161,175,3,34,17,0,162,171,5,33,0,0,163,168,3,18,9,0,164,165,5,27,
  	0,0,165,167,3,18,9,0,166,164,1,0,0,0,167,170,1,0,0,0,168,166,1,0,0,0,
  	168,169,1,0,0,0,169,172,1,0,0,0,170,168,1,0,0,0,171,163,1,0,0,0,171,172,
  	1,0,0,0,172,173,1,0,0,0,173,175,5,34,0,0,174,161,1,0,0,0,174,162,1,0,
  	0,0,175,19,1,0,0,0,176,177,3,22,11,0,177,178,5,35,0,0,178,180,5,29,0,
  	0,179,181,3,24,12,0,180,179,1,0,0,0,180,181,1,0,0,0,181,182,1,0,0,0,182,
  	183,5,30,0,0,183,184,3,28,14,0,184,21,1,0,0,0,185,190,5,4,0,0,186,190,
  	5,2,0,0,187,190,5,3,0,0,188,190,3,8,4,0,189,185,1,0,0,0,189,186,1,0,0,
  	0,189,187,1,0,0,0,189,188,1,0,0,0,190,23,1,0,0,0,191,196,3,26,13,0,192,
  	193,5,27,0,0,193,195,3,26,13,0,194,192,1,0,0,0,195,198,1,0,0,0,196,194,
  	1,0,0,0,196,197,1,0,0,0,197,25,1,0,0,0,198,196,1,0,0,0,199,200,3,6,3,
  	0,200,212,5,35,0,0,201,202,5,31,0,0,202,209,5,32,0,0,203,204,5,31,0,0,
  	204,205,3,34,17,0,205,206,5,32,0,0,206,208,1,0,0,0,207,203,1,0,0,0,208,
  	211,1,0,0,0,209,207,1,0,0,0,209,210,1,0,0,0,210,213,1,0,0,0,211,209,1,
  	0,0,0,212,201,1,0,0,0,212,213,1,0,0,0,213,27,1,0,0,0,214,218,5,33,0,0,
  	215,217,3,30,15,0,216,215,1,0,0,0,217,220,1,0,0,0,218,216,1,0,0,0,218,
  	219,1,0,0,0,219,221,1,0,0,0,220,218,1,0,0,0,221,222,5,34,0,0,222,29,1,
  	0,0,0,223,226,3,2,1,0,224,226,3,32,16,0,225,223,1,0,0,0,225,224,1,0,0,
  	0,226,31,1,0,0,0,227,228,3,38,19,0,228,229,5,17,0,0,229,230,3,34,17,0,
  	230,231,5,28,0,0,231,262,1,0,0,0,232,234,3,34,17,0,233,232,1,0,0,0,233,
  	234,1,0,0,0,234,235,1,0,0,0,235,262,5,28,0,0,236,262,3,28,14,0,237,238,
  	5,6,0,0,238,239,5,29,0,0,239,240,3,36,18,0,240,241,5,30,0,0,241,244,3,
  	32,16,0,242,243,5,7,0,0,243,245,3,32,16,0,244,242,1,0,0,0,244,245,1,0,
  	0,0,245,262,1,0,0,0,246,247,5,8,0,0,247,248,5,29,0,0,248,249,3,36,18,
  	0,249,250,5,30,0,0,250,251,3,32,16,0,251,262,1,0,0,0,252,253,5,9,0,0,
  	253,262,5,28,0,0,254,255,5,10,0,0,255,262,5,28,0,0,256,258,5,11,0,0,257,
  	259,3,34,17,0,258,257,1,0,0,0,258,259,1,0,0,0,259,260,1,0,0,0,260,262,
  	5,28,0,0,261,227,1,0,0,0,261,233,1,0,0,0,261,236,1,0,0,0,261,237,1,0,
  	0,0,261,246,1,0,0,0,261,252,1,0,0,0,261,254,1,0,0,0,261,256,1,0,0,0,262,
  	33,1,0,0,0,263,264,3,52,26,0,264,35,1,0,0,0,265,266,3,60,30,0,266,37,
  	1,0,0,0,267,274,5,35,0,0,268,269,5,31,0,0,269,270,3,34,17,0,270,271,5,
  	32,0,0,271,273,1,0,0,0,272,268,1,0,0,0,273,276,1,0,0,0,274,272,1,0,0,
  	0,274,275,1,0,0,0,275,39,1,0,0,0,276,274,1,0,0,0,277,278,5,29,0,0,278,
  	279,3,34,17,0,279,280,5,30,0,0,280,285,1,0,0,0,281,285,3,38,19,0,282,
  	285,3,42,21,0,283,285,5,38,0,0,284,277,1,0,0,0,284,281,1,0,0,0,284,282,
  	1,0,0,0,284,283,1,0,0,0,285,41,1,0,0,0,286,287,7,1,0,0,287,43,1,0,0,0,
  	288,299,3,40,20,0,289,290,5,35,0,0,290,292,5,29,0,0,291,293,3,48,24,0,
  	292,291,1,0,0,0,292,293,1,0,0,0,293,294,1,0,0,0,294,299,5,30,0,0,295,
  	296,3,46,23,0,296,297,3,44,22,0,297,299,1,0,0,0,298,288,1,0,0,0,298,289,
  	1,0,0,0,298,295,1,0,0,0,299,45,1,0,0,0,300,301,7,2,0,0,301,47,1,0,0,0,
  	302,307,3,34,17,0,303,304,5,27,0,0,304,306,3,34,17,0,305,303,1,0,0,0,
  	306,309,1,0,0,0,307,305,1,0,0,0,307,308,1,0,0,0,308,49,1,0,0,0,309,307,
  	1,0,0,0,310,311,6,25,-1,0,311,312,3,44,22,0,312,318,1,0,0,0,313,314,10,
  	1,0,0,314,315,7,3,0,0,315,317,3,44,22,0,316,313,1,0,0,0,317,320,1,0,0,
  	0,318,316,1,0,0,0,318,319,1,0,0,0,319,51,1,0,0,0,320,318,1,0,0,0,321,
  	322,6,26,-1,0,322,323,3,50,25,0,323,329,1,0,0,0,324,325,10,1,0,0,325,
  	326,7,4,0,0,326,328,3,50,25,0,327,324,1,0,0,0,328,331,1,0,0,0,329,327,
  	1,0,0,0,329,330,1,0,0,0,330,53,1,0,0,0,331,329,1,0,0,0,332,333,6,27,-1,
  	0,333,334,3,52,26,0,334,340,1,0,0,0,335,336,10,1,0,0,336,337,7,5,0,0,
  	337,339,3,52,26,0,338,335,1,0,0,0,339,342,1,0,0,0,340,338,1,0,0,0,340,
  	341,1,0,0,0,341,55,1,0,0,0,342,340,1,0,0,0,343,344,6,28,-1,0,344,345,
  	3,54,27,0,345,351,1,0,0,0,346,347,10,1,0,0,347,348,7,6,0,0,348,350,3,
  	54,27,0,349,346,1,0,0,0,350,353,1,0,0,0,351,349,1,0,0,0,351,352,1,0,0,
  	0,352,57,1,0,0,0,353,351,1,0,0,0,354,355,6,29,-1,0,355,356,3,56,28,0,
  	356,362,1,0,0,0,357,358,10,1,0,0,358,359,5,25,0,0,359,361,3,56,28,0,360,
  	357,1,0,0,0,361,364,1,0,0,0,362,360,1,0,0,0,362,363,1,0,0,0,363,59,1,
  	0,0,0,364,362,1,0,0,0,365,366,6,30,-1,0,366,367,3,58,29,0,367,373,1,0,
  	0,0,368,369,10,1,0,0,369,370,5,26,0,0,370,372,3,58,29,0,371,368,1,0,0,
  	0,372,375,1,0,0,0,373,371,1,0,0,0,373,374,1,0,0,0,374,61,1,0,0,0,375,
  	373,1,0,0,0,376,377,3,52,26,0,377,63,1,0,0,0,38,66,68,72,81,89,105,118,
  	121,124,132,144,154,159,168,171,174,180,189,196,209,212,218,225,233,244,
  	258,261,274,284,292,298,307,318,329,340,351,362,373
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sysyParserStaticData = std::move(staticData);
}

}

SysYParser::SysYParser(TokenStream *input) : SysYParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SysYParser::SysYParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SysYParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *sysyParserStaticData->atn, sysyParserStaticData->decisionToDFA, sysyParserStaticData->sharedContextCache, options);
}

SysYParser::~SysYParser() {
  delete _interpreter;
}

const atn::ATN& SysYParser::getATN() const {
  return *sysyParserStaticData->atn;
}

std::string SysYParser::getGrammarFileName() const {
  return "SysY.g4";
}

const std::vector<std::string>& SysYParser::getRuleNames() const {
  return sysyParserStaticData->ruleNames;
}

const dfa::Vocabulary& SysYParser::getVocabulary() const {
  return sysyParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SysYParser::getSerializedATN() const {
  return sysyParserStaticData->serializedATN;
}


//----------------- CompUnitContext ------------------------------------------------------------------

SysYParser::CompUnitContext::CompUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::DeclContext *> SysYParser::CompUnitContext::decl() {
  return getRuleContexts<SysYParser::DeclContext>();
}

SysYParser::DeclContext* SysYParser::CompUnitContext::decl(size_t i) {
  return getRuleContext<SysYParser::DeclContext>(i);
}

std::vector<SysYParser::FuncDefContext *> SysYParser::CompUnitContext::funcDef() {
  return getRuleContexts<SysYParser::FuncDefContext>();
}

SysYParser::FuncDefContext* SysYParser::CompUnitContext::funcDef(size_t i) {
  return getRuleContext<SysYParser::FuncDefContext>(i);
}


size_t SysYParser::CompUnitContext::getRuleIndex() const {
  return SysYParser::RuleCompUnit;
}

void SysYParser::CompUnitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompUnit(this);
}

void SysYParser::CompUnitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompUnit(this);
}


std::any SysYParser::CompUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitCompUnit(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::CompUnitContext* SysYParser::compUnit() {
  CompUnitContext *_localctx = _tracker.createInstance<CompUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, SysYParser::RuleCompUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(66); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(66);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(64);
        decl();
        break;
      }

      case 2: {
        setState(65);
        funcDef();
        break;
      }

      default:
        break;
      }
      setState(68); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 62) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

SysYParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::ConstDeclContext* SysYParser::DeclContext::constDecl() {
  return getRuleContext<SysYParser::ConstDeclContext>(0);
}

SysYParser::VarDeclContext* SysYParser::DeclContext::varDecl() {
  return getRuleContext<SysYParser::VarDeclContext>(0);
}


size_t SysYParser::DeclContext::getRuleIndex() const {
  return SysYParser::RuleDecl;
}

void SysYParser::DeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDecl(this);
}

void SysYParser::DeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDecl(this);
}


std::any SysYParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::DeclContext* SysYParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 2, SysYParser::RuleDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(72);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::CONST: {
        enterOuterAlt(_localctx, 1);
        setState(70);
        constDecl();
        break;
      }

      case SysYParser::INT:
      case SysYParser::FLOAT:
      case SysYParser::VECTOR: {
        enterOuterAlt(_localctx, 2);
        setState(71);
        varDecl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDeclContext ------------------------------------------------------------------

SysYParser::ConstDeclContext::ConstDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::ConstDeclContext::CONST() {
  return getToken(SysYParser::CONST, 0);
}

SysYParser::BTypeContext* SysYParser::ConstDeclContext::bType() {
  return getRuleContext<SysYParser::BTypeContext>(0);
}

std::vector<SysYParser::ConstDefContext *> SysYParser::ConstDeclContext::constDef() {
  return getRuleContexts<SysYParser::ConstDefContext>();
}

SysYParser::ConstDefContext* SysYParser::ConstDeclContext::constDef(size_t i) {
  return getRuleContext<SysYParser::ConstDefContext>(i);
}

tree::TerminalNode* SysYParser::ConstDeclContext::SEMICOLON() {
  return getToken(SysYParser::SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> SysYParser::ConstDeclContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::ConstDeclContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::ConstDeclContext::getRuleIndex() const {
  return SysYParser::RuleConstDecl;
}

void SysYParser::ConstDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstDecl(this);
}

void SysYParser::ConstDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstDecl(this);
}


std::any SysYParser::ConstDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstDeclContext* SysYParser::constDecl() {
  ConstDeclContext *_localctx = _tracker.createInstance<ConstDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, SysYParser::RuleConstDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    match(SysYParser::CONST);
    setState(75);
    bType();
    setState(76);
    constDef();
    setState(81);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::COMMA) {
      setState(77);
      match(SysYParser::COMMA);
      setState(78);
      constDef();
      setState(83);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(84);
    match(SysYParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BTypeContext ------------------------------------------------------------------

SysYParser::BTypeContext::BTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::BTypeContext::INT() {
  return getToken(SysYParser::INT, 0);
}

tree::TerminalNode* SysYParser::BTypeContext::FLOAT() {
  return getToken(SysYParser::FLOAT, 0);
}

SysYParser::VectorTypeContext* SysYParser::BTypeContext::vectorType() {
  return getRuleContext<SysYParser::VectorTypeContext>(0);
}


size_t SysYParser::BTypeContext::getRuleIndex() const {
  return SysYParser::RuleBType;
}

void SysYParser::BTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBType(this);
}

void SysYParser::BTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBType(this);
}


std::any SysYParser::BTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBType(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::BTypeContext* SysYParser::bType() {
  BTypeContext *_localctx = _tracker.createInstance<BTypeContext>(_ctx, getState());
  enterRule(_localctx, 6, SysYParser::RuleBType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(89);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::INT: {
        enterOuterAlt(_localctx, 1);
        setState(86);
        match(SysYParser::INT);
        break;
      }

      case SysYParser::FLOAT: {
        enterOuterAlt(_localctx, 2);
        setState(87);
        match(SysYParser::FLOAT);
        break;
      }

      case SysYParser::VECTOR: {
        enterOuterAlt(_localctx, 3);
        setState(88);
        vectorType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VectorTypeContext ------------------------------------------------------------------

SysYParser::VectorTypeContext::VectorTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::VectorTypeContext::VECTOR() {
  return getToken(SysYParser::VECTOR, 0);
}

tree::TerminalNode* SysYParser::VectorTypeContext::LT() {
  return getToken(SysYParser::LT, 0);
}

tree::TerminalNode* SysYParser::VectorTypeContext::COMMA() {
  return getToken(SysYParser::COMMA, 0);
}

SysYParser::ConstExpContext* SysYParser::VectorTypeContext::constExp() {
  return getRuleContext<SysYParser::ConstExpContext>(0);
}

tree::TerminalNode* SysYParser::VectorTypeContext::GT() {
  return getToken(SysYParser::GT, 0);
}

tree::TerminalNode* SysYParser::VectorTypeContext::INT() {
  return getToken(SysYParser::INT, 0);
}

tree::TerminalNode* SysYParser::VectorTypeContext::FLOAT() {
  return getToken(SysYParser::FLOAT, 0);
}


size_t SysYParser::VectorTypeContext::getRuleIndex() const {
  return SysYParser::RuleVectorType;
}

void SysYParser::VectorTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVectorType(this);
}

void SysYParser::VectorTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVectorType(this);
}


std::any SysYParser::VectorTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitVectorType(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::VectorTypeContext* SysYParser::vectorType() {
  VectorTypeContext *_localctx = _tracker.createInstance<VectorTypeContext>(_ctx, getState());
  enterRule(_localctx, 8, SysYParser::RuleVectorType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(91);
    match(SysYParser::VECTOR);
    setState(92);
    match(SysYParser::LT);
    setState(93);
    _la = _input->LA(1);
    if (!(_la == SysYParser::INT

    || _la == SysYParser::FLOAT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(94);
    match(SysYParser::COMMA);
    setState(95);
    constExp();
    setState(96);
    match(SysYParser::GT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDefContext ------------------------------------------------------------------

SysYParser::ConstDefContext::ConstDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::ConstDefContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

tree::TerminalNode* SysYParser::ConstDefContext::ASSIGN() {
  return getToken(SysYParser::ASSIGN, 0);
}

SysYParser::ConstInitValContext* SysYParser::ConstDefContext::constInitVal() {
  return getRuleContext<SysYParser::ConstInitValContext>(0);
}

std::vector<tree::TerminalNode *> SysYParser::ConstDefContext::LBRACK() {
  return getTokens(SysYParser::LBRACK);
}

tree::TerminalNode* SysYParser::ConstDefContext::LBRACK(size_t i) {
  return getToken(SysYParser::LBRACK, i);
}

std::vector<SysYParser::ConstExpContext *> SysYParser::ConstDefContext::constExp() {
  return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext* SysYParser::ConstDefContext::constExp(size_t i) {
  return getRuleContext<SysYParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::ConstDefContext::RBRACK() {
  return getTokens(SysYParser::RBRACK);
}

tree::TerminalNode* SysYParser::ConstDefContext::RBRACK(size_t i) {
  return getToken(SysYParser::RBRACK, i);
}


size_t SysYParser::ConstDefContext::getRuleIndex() const {
  return SysYParser::RuleConstDef;
}

void SysYParser::ConstDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstDef(this);
}

void SysYParser::ConstDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstDef(this);
}


std::any SysYParser::ConstDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstDefContext* SysYParser::constDef() {
  ConstDefContext *_localctx = _tracker.createInstance<ConstDefContext>(_ctx, getState());
  enterRule(_localctx, 10, SysYParser::RuleConstDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(SysYParser::IDENT);
    setState(105);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::LBRACK) {
      setState(99);
      match(SysYParser::LBRACK);
      setState(100);
      constExp();
      setState(101);
      match(SysYParser::RBRACK);
      setState(107);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(108);
    match(SysYParser::ASSIGN);
    setState(109);
    constInitVal();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstInitValContext ------------------------------------------------------------------

SysYParser::ConstInitValContext::ConstInitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::ConstExpContext* SysYParser::ConstInitValContext::constExp() {
  return getRuleContext<SysYParser::ConstExpContext>(0);
}

tree::TerminalNode* SysYParser::ConstInitValContext::LBRACE() {
  return getToken(SysYParser::LBRACE, 0);
}

tree::TerminalNode* SysYParser::ConstInitValContext::RBRACE() {
  return getToken(SysYParser::RBRACE, 0);
}

std::vector<SysYParser::ConstInitValContext *> SysYParser::ConstInitValContext::constInitVal() {
  return getRuleContexts<SysYParser::ConstInitValContext>();
}

SysYParser::ConstInitValContext* SysYParser::ConstInitValContext::constInitVal(size_t i) {
  return getRuleContext<SysYParser::ConstInitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::ConstInitValContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::ConstInitValContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::ConstInitValContext::getRuleIndex() const {
  return SysYParser::RuleConstInitVal;
}

void SysYParser::ConstInitValContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstInitVal(this);
}

void SysYParser::ConstInitValContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstInitVal(this);
}


std::any SysYParser::ConstInitValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstInitVal(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstInitValContext* SysYParser::constInitVal() {
  ConstInitValContext *_localctx = _tracker.createInstance<ConstInitValContext>(_ctx, getState());
  enterRule(_localctx, 12, SysYParser::RuleConstInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(124);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::PLUS:
      case SysYParser::MINUS:
      case SysYParser::NOT:
      case SysYParser::LPAREN:
      case SysYParser::IDENT:
      case SysYParser::IntConst:
      case SysYParser::FloatConst:
      case SysYParser::StringLiteral: {
        enterOuterAlt(_localctx, 1);
        setState(111);
        constExp();
        break;
      }

      case SysYParser::LBRACE: {
        enterOuterAlt(_localctx, 2);
        setState(112);
        match(SysYParser::LBRACE);
        setState(121);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 524539670528) != 0)) {
          setState(113);
          constInitVal();
          setState(118);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParser::COMMA) {
            setState(114);
            match(SysYParser::COMMA);
            setState(115);
            constInitVal();
            setState(120);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(123);
        match(SysYParser::RBRACE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

SysYParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::BTypeContext* SysYParser::VarDeclContext::bType() {
  return getRuleContext<SysYParser::BTypeContext>(0);
}

std::vector<SysYParser::VarDefContext *> SysYParser::VarDeclContext::varDef() {
  return getRuleContexts<SysYParser::VarDefContext>();
}

SysYParser::VarDefContext* SysYParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<SysYParser::VarDefContext>(i);
}

tree::TerminalNode* SysYParser::VarDeclContext::SEMICOLON() {
  return getToken(SysYParser::SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> SysYParser::VarDeclContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::VarDeclContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::VarDeclContext::getRuleIndex() const {
  return SysYParser::RuleVarDecl;
}

void SysYParser::VarDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDecl(this);
}

void SysYParser::VarDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDecl(this);
}


std::any SysYParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::VarDeclContext* SysYParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 14, SysYParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    bType();
    setState(127);
    varDef();
    setState(132);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::COMMA) {
      setState(128);
      match(SysYParser::COMMA);
      setState(129);
      varDef();
      setState(134);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(135);
    match(SysYParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

SysYParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::VarDefContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

std::vector<tree::TerminalNode *> SysYParser::VarDefContext::LBRACK() {
  return getTokens(SysYParser::LBRACK);
}

tree::TerminalNode* SysYParser::VarDefContext::LBRACK(size_t i) {
  return getToken(SysYParser::LBRACK, i);
}

std::vector<SysYParser::ConstExpContext *> SysYParser::VarDefContext::constExp() {
  return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext* SysYParser::VarDefContext::constExp(size_t i) {
  return getRuleContext<SysYParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::VarDefContext::RBRACK() {
  return getTokens(SysYParser::RBRACK);
}

tree::TerminalNode* SysYParser::VarDefContext::RBRACK(size_t i) {
  return getToken(SysYParser::RBRACK, i);
}

tree::TerminalNode* SysYParser::VarDefContext::ASSIGN() {
  return getToken(SysYParser::ASSIGN, 0);
}

SysYParser::InitValContext* SysYParser::VarDefContext::initVal() {
  return getRuleContext<SysYParser::InitValContext>(0);
}


size_t SysYParser::VarDefContext::getRuleIndex() const {
  return SysYParser::RuleVarDef;
}

void SysYParser::VarDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDef(this);
}

void SysYParser::VarDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDef(this);
}


std::any SysYParser::VarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitVarDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::VarDefContext* SysYParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 16, SysYParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(159);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(137);
      match(SysYParser::IDENT);
      setState(144);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::LBRACK) {
        setState(138);
        match(SysYParser::LBRACK);
        setState(139);
        constExp();
        setState(140);
        match(SysYParser::RBRACK);
        setState(146);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(147);
      match(SysYParser::IDENT);
      setState(154);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::LBRACK) {
        setState(148);
        match(SysYParser::LBRACK);
        setState(149);
        constExp();
        setState(150);
        match(SysYParser::RBRACK);
        setState(156);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(157);
      match(SysYParser::ASSIGN);
      setState(158);
      initVal();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitValContext ------------------------------------------------------------------

SysYParser::InitValContext::InitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::ExpContext* SysYParser::InitValContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

tree::TerminalNode* SysYParser::InitValContext::LBRACE() {
  return getToken(SysYParser::LBRACE, 0);
}

tree::TerminalNode* SysYParser::InitValContext::RBRACE() {
  return getToken(SysYParser::RBRACE, 0);
}

std::vector<SysYParser::InitValContext *> SysYParser::InitValContext::initVal() {
  return getRuleContexts<SysYParser::InitValContext>();
}

SysYParser::InitValContext* SysYParser::InitValContext::initVal(size_t i) {
  return getRuleContext<SysYParser::InitValContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::InitValContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::InitValContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::InitValContext::getRuleIndex() const {
  return SysYParser::RuleInitVal;
}

void SysYParser::InitValContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitVal(this);
}

void SysYParser::InitValContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitVal(this);
}


std::any SysYParser::InitValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitInitVal(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::InitValContext* SysYParser::initVal() {
  InitValContext *_localctx = _tracker.createInstance<InitValContext>(_ctx, getState());
  enterRule(_localctx, 18, SysYParser::RuleInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(174);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::PLUS:
      case SysYParser::MINUS:
      case SysYParser::NOT:
      case SysYParser::LPAREN:
      case SysYParser::IDENT:
      case SysYParser::IntConst:
      case SysYParser::FloatConst:
      case SysYParser::StringLiteral: {
        enterOuterAlt(_localctx, 1);
        setState(161);
        exp();
        break;
      }

      case SysYParser::LBRACE: {
        enterOuterAlt(_localctx, 2);
        setState(162);
        match(SysYParser::LBRACE);
        setState(171);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 524539670528) != 0)) {
          setState(163);
          initVal();
          setState(168);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParser::COMMA) {
            setState(164);
            match(SysYParser::COMMA);
            setState(165);
            initVal();
            setState(170);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(173);
        match(SysYParser::RBRACE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

SysYParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::FuncTypeContext* SysYParser::FuncDefContext::funcType() {
  return getRuleContext<SysYParser::FuncTypeContext>(0);
}

tree::TerminalNode* SysYParser::FuncDefContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

tree::TerminalNode* SysYParser::FuncDefContext::LPAREN() {
  return getToken(SysYParser::LPAREN, 0);
}

tree::TerminalNode* SysYParser::FuncDefContext::RPAREN() {
  return getToken(SysYParser::RPAREN, 0);
}

SysYParser::BlockContext* SysYParser::FuncDefContext::block() {
  return getRuleContext<SysYParser::BlockContext>(0);
}

SysYParser::FuncFParamsContext* SysYParser::FuncDefContext::funcFParams() {
  return getRuleContext<SysYParser::FuncFParamsContext>(0);
}


size_t SysYParser::FuncDefContext::getRuleIndex() const {
  return SysYParser::RuleFuncDef;
}

void SysYParser::FuncDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncDef(this);
}

void SysYParser::FuncDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncDef(this);
}


std::any SysYParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncDefContext* SysYParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 20, SysYParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    funcType();
    setState(177);
    match(SysYParser::IDENT);
    setState(178);
    match(SysYParser::LPAREN);
    setState(180);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 44) != 0)) {
      setState(179);
      funcFParams();
    }
    setState(182);
    match(SysYParser::RPAREN);
    setState(183);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncTypeContext ------------------------------------------------------------------

SysYParser::FuncTypeContext::FuncTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::FuncTypeContext::VOID() {
  return getToken(SysYParser::VOID, 0);
}

tree::TerminalNode* SysYParser::FuncTypeContext::INT() {
  return getToken(SysYParser::INT, 0);
}

tree::TerminalNode* SysYParser::FuncTypeContext::FLOAT() {
  return getToken(SysYParser::FLOAT, 0);
}

SysYParser::VectorTypeContext* SysYParser::FuncTypeContext::vectorType() {
  return getRuleContext<SysYParser::VectorTypeContext>(0);
}


size_t SysYParser::FuncTypeContext::getRuleIndex() const {
  return SysYParser::RuleFuncType;
}

void SysYParser::FuncTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncType(this);
}

void SysYParser::FuncTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncType(this);
}


std::any SysYParser::FuncTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncType(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncTypeContext* SysYParser::funcType() {
  FuncTypeContext *_localctx = _tracker.createInstance<FuncTypeContext>(_ctx, getState());
  enterRule(_localctx, 22, SysYParser::RuleFuncType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(189);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::VOID: {
        enterOuterAlt(_localctx, 1);
        setState(185);
        match(SysYParser::VOID);
        break;
      }

      case SysYParser::INT: {
        enterOuterAlt(_localctx, 2);
        setState(186);
        match(SysYParser::INT);
        break;
      }

      case SysYParser::FLOAT: {
        enterOuterAlt(_localctx, 3);
        setState(187);
        match(SysYParser::FLOAT);
        break;
      }

      case SysYParser::VECTOR: {
        enterOuterAlt(_localctx, 4);
        setState(188);
        vectorType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamsContext ------------------------------------------------------------------

SysYParser::FuncFParamsContext::FuncFParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::FuncFParamContext *> SysYParser::FuncFParamsContext::funcFParam() {
  return getRuleContexts<SysYParser::FuncFParamContext>();
}

SysYParser::FuncFParamContext* SysYParser::FuncFParamsContext::funcFParam(size_t i) {
  return getRuleContext<SysYParser::FuncFParamContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::FuncFParamsContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::FuncFParamsContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::FuncFParamsContext::getRuleIndex() const {
  return SysYParser::RuleFuncFParams;
}

void SysYParser::FuncFParamsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncFParams(this);
}

void SysYParser::FuncFParamsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncFParams(this);
}


std::any SysYParser::FuncFParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncFParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncFParamsContext* SysYParser::funcFParams() {
  FuncFParamsContext *_localctx = _tracker.createInstance<FuncFParamsContext>(_ctx, getState());
  enterRule(_localctx, 24, SysYParser::RuleFuncFParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    funcFParam();
    setState(196);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::COMMA) {
      setState(192);
      match(SysYParser::COMMA);
      setState(193);
      funcFParam();
      setState(198);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamContext ------------------------------------------------------------------

SysYParser::FuncFParamContext::FuncFParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::BTypeContext* SysYParser::FuncFParamContext::bType() {
  return getRuleContext<SysYParser::BTypeContext>(0);
}

tree::TerminalNode* SysYParser::FuncFParamContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

std::vector<tree::TerminalNode *> SysYParser::FuncFParamContext::LBRACK() {
  return getTokens(SysYParser::LBRACK);
}

tree::TerminalNode* SysYParser::FuncFParamContext::LBRACK(size_t i) {
  return getToken(SysYParser::LBRACK, i);
}

std::vector<tree::TerminalNode *> SysYParser::FuncFParamContext::RBRACK() {
  return getTokens(SysYParser::RBRACK);
}

tree::TerminalNode* SysYParser::FuncFParamContext::RBRACK(size_t i) {
  return getToken(SysYParser::RBRACK, i);
}

std::vector<SysYParser::ExpContext *> SysYParser::FuncFParamContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::FuncFParamContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}


size_t SysYParser::FuncFParamContext::getRuleIndex() const {
  return SysYParser::RuleFuncFParam;
}

void SysYParser::FuncFParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncFParam(this);
}

void SysYParser::FuncFParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncFParam(this);
}


std::any SysYParser::FuncFParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncFParam(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncFParamContext* SysYParser::funcFParam() {
  FuncFParamContext *_localctx = _tracker.createInstance<FuncFParamContext>(_ctx, getState());
  enterRule(_localctx, 26, SysYParser::RuleFuncFParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(199);
    bType();
    setState(200);
    match(SysYParser::IDENT);
    setState(212);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SysYParser::LBRACK) {
      setState(201);
      match(SysYParser::LBRACK);
      setState(202);
      match(SysYParser::RBRACK);
      setState(209);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::LBRACK) {
        setState(203);
        match(SysYParser::LBRACK);
        setState(204);
        exp();
        setState(205);
        match(SysYParser::RBRACK);
        setState(211);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

SysYParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::BlockContext::LBRACE() {
  return getToken(SysYParser::LBRACE, 0);
}

tree::TerminalNode* SysYParser::BlockContext::RBRACE() {
  return getToken(SysYParser::RBRACE, 0);
}

std::vector<SysYParser::BlockItemContext *> SysYParser::BlockContext::blockItem() {
  return getRuleContexts<SysYParser::BlockItemContext>();
}

SysYParser::BlockItemContext* SysYParser::BlockContext::blockItem(size_t i) {
  return getRuleContext<SysYParser::BlockItemContext>(i);
}


size_t SysYParser::BlockContext::getRuleIndex() const {
  return SysYParser::RuleBlock;
}

void SysYParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void SysYParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any SysYParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::BlockContext* SysYParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 28, SysYParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
    match(SysYParser::LBRACE);
    setState(218);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 524808109934) != 0)) {
      setState(215);
      blockItem();
      setState(220);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(221);
    match(SysYParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

SysYParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::DeclContext* SysYParser::BlockItemContext::decl() {
  return getRuleContext<SysYParser::DeclContext>(0);
}

SysYParser::StmtContext* SysYParser::BlockItemContext::stmt() {
  return getRuleContext<SysYParser::StmtContext>(0);
}


size_t SysYParser::BlockItemContext::getRuleIndex() const {
  return SysYParser::RuleBlockItem;
}

void SysYParser::BlockItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockItem(this);
}

void SysYParser::BlockItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockItem(this);
}


std::any SysYParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::BlockItemContext* SysYParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 30, SysYParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(225);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::CONST:
      case SysYParser::INT:
      case SysYParser::FLOAT:
      case SysYParser::VECTOR: {
        enterOuterAlt(_localctx, 1);
        setState(223);
        decl();
        break;
      }

      case SysYParser::IF:
      case SysYParser::WHILE:
      case SysYParser::BREAK:
      case SysYParser::CONTINUE:
      case SysYParser::RETURN:
      case SysYParser::PLUS:
      case SysYParser::MINUS:
      case SysYParser::NOT:
      case SysYParser::SEMICOLON:
      case SysYParser::LPAREN:
      case SysYParser::LBRACE:
      case SysYParser::IDENT:
      case SysYParser::IntConst:
      case SysYParser::FloatConst:
      case SysYParser::StringLiteral: {
        enterOuterAlt(_localctx, 2);
        setState(224);
        stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SysYParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::LValContext* SysYParser::StmtContext::lVal() {
  return getRuleContext<SysYParser::LValContext>(0);
}

tree::TerminalNode* SysYParser::StmtContext::ASSIGN() {
  return getToken(SysYParser::ASSIGN, 0);
}

SysYParser::ExpContext* SysYParser::StmtContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

tree::TerminalNode* SysYParser::StmtContext::SEMICOLON() {
  return getToken(SysYParser::SEMICOLON, 0);
}

SysYParser::BlockContext* SysYParser::StmtContext::block() {
  return getRuleContext<SysYParser::BlockContext>(0);
}

tree::TerminalNode* SysYParser::StmtContext::IF() {
  return getToken(SysYParser::IF, 0);
}

tree::TerminalNode* SysYParser::StmtContext::LPAREN() {
  return getToken(SysYParser::LPAREN, 0);
}

SysYParser::CondContext* SysYParser::StmtContext::cond() {
  return getRuleContext<SysYParser::CondContext>(0);
}

tree::TerminalNode* SysYParser::StmtContext::RPAREN() {
  return getToken(SysYParser::RPAREN, 0);
}

std::vector<SysYParser::StmtContext *> SysYParser::StmtContext::stmt() {
  return getRuleContexts<SysYParser::StmtContext>();
}

SysYParser::StmtContext* SysYParser::StmtContext::stmt(size_t i) {
  return getRuleContext<SysYParser::StmtContext>(i);
}

tree::TerminalNode* SysYParser::StmtContext::ELSE() {
  return getToken(SysYParser::ELSE, 0);
}

tree::TerminalNode* SysYParser::StmtContext::WHILE() {
  return getToken(SysYParser::WHILE, 0);
}

tree::TerminalNode* SysYParser::StmtContext::BREAK() {
  return getToken(SysYParser::BREAK, 0);
}

tree::TerminalNode* SysYParser::StmtContext::CONTINUE() {
  return getToken(SysYParser::CONTINUE, 0);
}

tree::TerminalNode* SysYParser::StmtContext::RETURN() {
  return getToken(SysYParser::RETURN, 0);
}


size_t SysYParser::StmtContext::getRuleIndex() const {
  return SysYParser::RuleStmt;
}

void SysYParser::StmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStmt(this);
}

void SysYParser::StmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStmt(this);
}


std::any SysYParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::StmtContext* SysYParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 32, SysYParser::RuleStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(261);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(227);
      lVal();
      setState(228);
      match(SysYParser::ASSIGN);
      setState(229);
      exp();
      setState(230);
      match(SysYParser::SEMICOLON);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(233);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 515949735936) != 0)) {
        setState(232);
        exp();
      }
      setState(235);
      match(SysYParser::SEMICOLON);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(236);
      block();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(237);
      match(SysYParser::IF);
      setState(238);
      match(SysYParser::LPAREN);
      setState(239);
      cond();
      setState(240);
      match(SysYParser::RPAREN);
      setState(241);
      stmt();
      setState(244);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
      case 1: {
        setState(242);
        match(SysYParser::ELSE);
        setState(243);
        stmt();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(246);
      match(SysYParser::WHILE);
      setState(247);
      match(SysYParser::LPAREN);
      setState(248);
      cond();
      setState(249);
      match(SysYParser::RPAREN);
      setState(250);
      stmt();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(252);
      match(SysYParser::BREAK);
      setState(253);
      match(SysYParser::SEMICOLON);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(254);
      match(SysYParser::CONTINUE);
      setState(255);
      match(SysYParser::SEMICOLON);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(256);
      match(SysYParser::RETURN);
      setState(258);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 515949735936) != 0)) {
        setState(257);
        exp();
      }
      setState(260);
      match(SysYParser::SEMICOLON);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpContext ------------------------------------------------------------------

SysYParser::ExpContext::ExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::AddExpContext* SysYParser::ExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}


size_t SysYParser::ExpContext::getRuleIndex() const {
  return SysYParser::RuleExp;
}

void SysYParser::ExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp(this);
}

void SysYParser::ExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp(this);
}


std::any SysYParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ExpContext* SysYParser::exp() {
  ExpContext *_localctx = _tracker.createInstance<ExpContext>(_ctx, getState());
  enterRule(_localctx, 34, SysYParser::RuleExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(263);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondContext ------------------------------------------------------------------

SysYParser::CondContext::CondContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::LOrExpContext* SysYParser::CondContext::lOrExp() {
  return getRuleContext<SysYParser::LOrExpContext>(0);
}


size_t SysYParser::CondContext::getRuleIndex() const {
  return SysYParser::RuleCond;
}

void SysYParser::CondContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCond(this);
}

void SysYParser::CondContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCond(this);
}


std::any SysYParser::CondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitCond(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::CondContext* SysYParser::cond() {
  CondContext *_localctx = _tracker.createInstance<CondContext>(_ctx, getState());
  enterRule(_localctx, 36, SysYParser::RuleCond);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(265);
    lOrExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

SysYParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::LValContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

std::vector<tree::TerminalNode *> SysYParser::LValContext::LBRACK() {
  return getTokens(SysYParser::LBRACK);
}

tree::TerminalNode* SysYParser::LValContext::LBRACK(size_t i) {
  return getToken(SysYParser::LBRACK, i);
}

std::vector<SysYParser::ExpContext *> SysYParser::LValContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::LValContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::LValContext::RBRACK() {
  return getTokens(SysYParser::RBRACK);
}

tree::TerminalNode* SysYParser::LValContext::RBRACK(size_t i) {
  return getToken(SysYParser::RBRACK, i);
}


size_t SysYParser::LValContext::getRuleIndex() const {
  return SysYParser::RuleLVal;
}

void SysYParser::LValContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLVal(this);
}

void SysYParser::LValContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLVal(this);
}


std::any SysYParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::LValContext* SysYParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 38, SysYParser::RuleLVal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(267);
    match(SysYParser::IDENT);
    setState(274);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(268);
        match(SysYParser::LBRACK);
        setState(269);
        exp();
        setState(270);
        match(SysYParser::RBRACK); 
      }
      setState(276);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

SysYParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::PrimaryExpContext::LPAREN() {
  return getToken(SysYParser::LPAREN, 0);
}

SysYParser::ExpContext* SysYParser::PrimaryExpContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

tree::TerminalNode* SysYParser::PrimaryExpContext::RPAREN() {
  return getToken(SysYParser::RPAREN, 0);
}

SysYParser::LValContext* SysYParser::PrimaryExpContext::lVal() {
  return getRuleContext<SysYParser::LValContext>(0);
}

SysYParser::NumberContext* SysYParser::PrimaryExpContext::number() {
  return getRuleContext<SysYParser::NumberContext>(0);
}

tree::TerminalNode* SysYParser::PrimaryExpContext::StringLiteral() {
  return getToken(SysYParser::StringLiteral, 0);
}


size_t SysYParser::PrimaryExpContext::getRuleIndex() const {
  return SysYParser::RulePrimaryExp;
}

void SysYParser::PrimaryExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExp(this);
}

void SysYParser::PrimaryExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExp(this);
}


std::any SysYParser::PrimaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::PrimaryExpContext* SysYParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 40, SysYParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(284);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::LPAREN: {
        enterOuterAlt(_localctx, 1);
        setState(277);
        match(SysYParser::LPAREN);
        setState(278);
        exp();
        setState(279);
        match(SysYParser::RPAREN);
        break;
      }

      case SysYParser::IDENT: {
        enterOuterAlt(_localctx, 2);
        setState(281);
        lVal();
        break;
      }

      case SysYParser::IntConst:
      case SysYParser::FloatConst: {
        enterOuterAlt(_localctx, 3);
        setState(282);
        number();
        break;
      }

      case SysYParser::StringLiteral: {
        enterOuterAlt(_localctx, 4);
        setState(283);
        match(SysYParser::StringLiteral);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

SysYParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::NumberContext::IntConst() {
  return getToken(SysYParser::IntConst, 0);
}

tree::TerminalNode* SysYParser::NumberContext::FloatConst() {
  return getToken(SysYParser::FloatConst, 0);
}


size_t SysYParser::NumberContext::getRuleIndex() const {
  return SysYParser::RuleNumber;
}

void SysYParser::NumberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumber(this);
}

void SysYParser::NumberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumber(this);
}


std::any SysYParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::NumberContext* SysYParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 42, SysYParser::RuleNumber);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    _la = _input->LA(1);
    if (!(_la == SysYParser::IntConst

    || _la == SysYParser::FloatConst)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

SysYParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::PrimaryExpContext* SysYParser::UnaryExpContext::primaryExp() {
  return getRuleContext<SysYParser::PrimaryExpContext>(0);
}

tree::TerminalNode* SysYParser::UnaryExpContext::IDENT() {
  return getToken(SysYParser::IDENT, 0);
}

tree::TerminalNode* SysYParser::UnaryExpContext::LPAREN() {
  return getToken(SysYParser::LPAREN, 0);
}

tree::TerminalNode* SysYParser::UnaryExpContext::RPAREN() {
  return getToken(SysYParser::RPAREN, 0);
}

SysYParser::FuncRParamsContext* SysYParser::UnaryExpContext::funcRParams() {
  return getRuleContext<SysYParser::FuncRParamsContext>(0);
}

SysYParser::UnaryOpContext* SysYParser::UnaryExpContext::unaryOp() {
  return getRuleContext<SysYParser::UnaryOpContext>(0);
}

SysYParser::UnaryExpContext* SysYParser::UnaryExpContext::unaryExp() {
  return getRuleContext<SysYParser::UnaryExpContext>(0);
}


size_t SysYParser::UnaryExpContext::getRuleIndex() const {
  return SysYParser::RuleUnaryExp;
}

void SysYParser::UnaryExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryExp(this);
}

void SysYParser::UnaryExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryExp(this);
}


std::any SysYParser::UnaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitUnaryExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::UnaryExpContext* SysYParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 44, SysYParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(298);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      primaryExp();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(289);
      match(SysYParser::IDENT);
      setState(290);
      match(SysYParser::LPAREN);
      setState(292);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 515949735936) != 0)) {
        setState(291);
        funcRParams();
      }
      setState(294);
      match(SysYParser::RPAREN);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(295);
      unaryOp();
      setState(296);
      unaryExp();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

SysYParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::UnaryOpContext::PLUS() {
  return getToken(SysYParser::PLUS, 0);
}

tree::TerminalNode* SysYParser::UnaryOpContext::MINUS() {
  return getToken(SysYParser::MINUS, 0);
}

tree::TerminalNode* SysYParser::UnaryOpContext::NOT() {
  return getToken(SysYParser::NOT, 0);
}


size_t SysYParser::UnaryOpContext::getRuleIndex() const {
  return SysYParser::RuleUnaryOp;
}

void SysYParser::UnaryOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryOp(this);
}

void SysYParser::UnaryOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryOp(this);
}


std::any SysYParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::UnaryOpContext* SysYParser::unaryOp() {
  UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
  enterRule(_localctx, 46, SysYParser::RuleUnaryOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(300);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16789504) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncRParamsContext ------------------------------------------------------------------

SysYParser::FuncRParamsContext::FuncRParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::ExpContext *> SysYParser::FuncRParamsContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::FuncRParamsContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}

std::vector<tree::TerminalNode *> SysYParser::FuncRParamsContext::COMMA() {
  return getTokens(SysYParser::COMMA);
}

tree::TerminalNode* SysYParser::FuncRParamsContext::COMMA(size_t i) {
  return getToken(SysYParser::COMMA, i);
}


size_t SysYParser::FuncRParamsContext::getRuleIndex() const {
  return SysYParser::RuleFuncRParams;
}

void SysYParser::FuncRParamsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncRParams(this);
}

void SysYParser::FuncRParamsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncRParams(this);
}


std::any SysYParser::FuncRParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncRParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncRParamsContext* SysYParser::funcRParams() {
  FuncRParamsContext *_localctx = _tracker.createInstance<FuncRParamsContext>(_ctx, getState());
  enterRule(_localctx, 48, SysYParser::RuleFuncRParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(302);
    exp();
    setState(307);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::COMMA) {
      setState(303);
      match(SysYParser::COMMA);
      setState(304);
      exp();
      setState(309);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

SysYParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::UnaryExpContext* SysYParser::MulExpContext::unaryExp() {
  return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::MulExpContext* SysYParser::MulExpContext::mulExp() {
  return getRuleContext<SysYParser::MulExpContext>(0);
}

tree::TerminalNode* SysYParser::MulExpContext::MUL() {
  return getToken(SysYParser::MUL, 0);
}

tree::TerminalNode* SysYParser::MulExpContext::DIV() {
  return getToken(SysYParser::DIV, 0);
}

tree::TerminalNode* SysYParser::MulExpContext::MOD() {
  return getToken(SysYParser::MOD, 0);
}


size_t SysYParser::MulExpContext::getRuleIndex() const {
  return SysYParser::RuleMulExp;
}

void SysYParser::MulExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulExp(this);
}

void SysYParser::MulExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulExp(this);
}


std::any SysYParser::MulExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitMulExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::MulExpContext* SysYParser::mulExp() {
   return mulExp(0);
}

SysYParser::MulExpContext* SysYParser::mulExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, parentState);
  SysYParser::MulExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 50;
  enterRecursionRule(_localctx, 50, SysYParser::RuleMulExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(311);
    unaryExp();
    _ctx->stop = _input->LT(-1);
    setState(318);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<MulExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleMulExp);
        setState(313);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(314);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 114688) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(315);
        unaryExp(); 
      }
      setState(320);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

SysYParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::MulExpContext* SysYParser::AddExpContext::mulExp() {
  return getRuleContext<SysYParser::MulExpContext>(0);
}

SysYParser::AddExpContext* SysYParser::AddExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}

tree::TerminalNode* SysYParser::AddExpContext::PLUS() {
  return getToken(SysYParser::PLUS, 0);
}

tree::TerminalNode* SysYParser::AddExpContext::MINUS() {
  return getToken(SysYParser::MINUS, 0);
}


size_t SysYParser::AddExpContext::getRuleIndex() const {
  return SysYParser::RuleAddExp;
}

void SysYParser::AddExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddExp(this);
}

void SysYParser::AddExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddExp(this);
}


std::any SysYParser::AddExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitAddExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::AddExpContext* SysYParser::addExp() {
   return addExp(0);
}

SysYParser::AddExpContext* SysYParser::addExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, parentState);
  SysYParser::AddExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 52;
  enterRecursionRule(_localctx, 52, SysYParser::RuleAddExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(322);
    mulExp(0);
    _ctx->stop = _input->LT(-1);
    setState(329);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<AddExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleAddExp);
        setState(324);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(325);
        _la = _input->LA(1);
        if (!(_la == SysYParser::PLUS

        || _la == SysYParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(326);
        mulExp(0); 
      }
      setState(331);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

SysYParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::AddExpContext* SysYParser::RelExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}

SysYParser::RelExpContext* SysYParser::RelExpContext::relExp() {
  return getRuleContext<SysYParser::RelExpContext>(0);
}

tree::TerminalNode* SysYParser::RelExpContext::LT() {
  return getToken(SysYParser::LT, 0);
}

tree::TerminalNode* SysYParser::RelExpContext::GT() {
  return getToken(SysYParser::GT, 0);
}

tree::TerminalNode* SysYParser::RelExpContext::LE() {
  return getToken(SysYParser::LE, 0);
}

tree::TerminalNode* SysYParser::RelExpContext::GE() {
  return getToken(SysYParser::GE, 0);
}


size_t SysYParser::RelExpContext::getRuleIndex() const {
  return SysYParser::RuleRelExp;
}

void SysYParser::RelExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelExp(this);
}

void SysYParser::RelExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelExp(this);
}


std::any SysYParser::RelExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitRelExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::RelExpContext* SysYParser::relExp() {
   return relExp(0);
}

SysYParser::RelExpContext* SysYParser::relExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, parentState);
  SysYParser::RelExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 54;
  enterRecursionRule(_localctx, 54, SysYParser::RuleRelExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(333);
    addExp(0);
    _ctx->stop = _input->LT(-1);
    setState(340);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<RelExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleRelExp);
        setState(335);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(336);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 15728640) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(337);
        addExp(0); 
      }
      setState(342);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

SysYParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::RelExpContext* SysYParser::EqExpContext::relExp() {
  return getRuleContext<SysYParser::RelExpContext>(0);
}

SysYParser::EqExpContext* SysYParser::EqExpContext::eqExp() {
  return getRuleContext<SysYParser::EqExpContext>(0);
}

tree::TerminalNode* SysYParser::EqExpContext::EQ() {
  return getToken(SysYParser::EQ, 0);
}

tree::TerminalNode* SysYParser::EqExpContext::NE() {
  return getToken(SysYParser::NE, 0);
}


size_t SysYParser::EqExpContext::getRuleIndex() const {
  return SysYParser::RuleEqExp;
}

void SysYParser::EqExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqExp(this);
}

void SysYParser::EqExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqExp(this);
}


std::any SysYParser::EqExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitEqExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::EqExpContext* SysYParser::eqExp() {
   return eqExp(0);
}

SysYParser::EqExpContext* SysYParser::eqExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, parentState);
  SysYParser::EqExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 56;
  enterRecursionRule(_localctx, 56, SysYParser::RuleEqExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(344);
    relExp(0);
    _ctx->stop = _input->LT(-1);
    setState(351);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<EqExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleEqExp);
        setState(346);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(347);
        _la = _input->LA(1);
        if (!(_la == SysYParser::EQ

        || _la == SysYParser::NE)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(348);
        relExp(0); 
      }
      setState(353);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LAndExpContext ------------------------------------------------------------------

SysYParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::EqExpContext* SysYParser::LAndExpContext::eqExp() {
  return getRuleContext<SysYParser::EqExpContext>(0);
}

SysYParser::LAndExpContext* SysYParser::LAndExpContext::lAndExp() {
  return getRuleContext<SysYParser::LAndExpContext>(0);
}

tree::TerminalNode* SysYParser::LAndExpContext::AND() {
  return getToken(SysYParser::AND, 0);
}


size_t SysYParser::LAndExpContext::getRuleIndex() const {
  return SysYParser::RuleLAndExp;
}

void SysYParser::LAndExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLAndExp(this);
}

void SysYParser::LAndExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLAndExp(this);
}


std::any SysYParser::LAndExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLAndExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::LAndExpContext* SysYParser::lAndExp() {
   return lAndExp(0);
}

SysYParser::LAndExpContext* SysYParser::lAndExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::LAndExpContext *_localctx = _tracker.createInstance<LAndExpContext>(_ctx, parentState);
  SysYParser::LAndExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 58;
  enterRecursionRule(_localctx, 58, SysYParser::RuleLAndExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(355);
    eqExp(0);
    _ctx->stop = _input->LT(-1);
    setState(362);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<LAndExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleLAndExp);
        setState(357);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(358);
        match(SysYParser::AND);
        setState(359);
        eqExp(0); 
      }
      setState(364);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LOrExpContext ------------------------------------------------------------------

SysYParser::LOrExpContext::LOrExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::LAndExpContext* SysYParser::LOrExpContext::lAndExp() {
  return getRuleContext<SysYParser::LAndExpContext>(0);
}

SysYParser::LOrExpContext* SysYParser::LOrExpContext::lOrExp() {
  return getRuleContext<SysYParser::LOrExpContext>(0);
}

tree::TerminalNode* SysYParser::LOrExpContext::OR() {
  return getToken(SysYParser::OR, 0);
}


size_t SysYParser::LOrExpContext::getRuleIndex() const {
  return SysYParser::RuleLOrExp;
}

void SysYParser::LOrExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLOrExp(this);
}

void SysYParser::LOrExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLOrExp(this);
}


std::any SysYParser::LOrExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLOrExp(this);
  else
    return visitor->visitChildren(this);
}


SysYParser::LOrExpContext* SysYParser::lOrExp() {
   return lOrExp(0);
}

SysYParser::LOrExpContext* SysYParser::lOrExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::LOrExpContext *_localctx = _tracker.createInstance<LOrExpContext>(_ctx, parentState);
  SysYParser::LOrExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 60;
  enterRecursionRule(_localctx, 60, SysYParser::RuleLOrExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(366);
    lAndExp(0);
    _ctx->stop = _input->LT(-1);
    setState(373);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<LOrExpContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleLOrExp);
        setState(368);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(369);
        match(SysYParser::OR);
        setState(370);
        lAndExp(0); 
      }
      setState(375);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ConstExpContext ------------------------------------------------------------------

SysYParser::ConstExpContext::ConstExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::AddExpContext* SysYParser::ConstExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}


size_t SysYParser::ConstExpContext::getRuleIndex() const {
  return SysYParser::RuleConstExp;
}

void SysYParser::ConstExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstExp(this);
}

void SysYParser::ConstExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstExp(this);
}


std::any SysYParser::ConstExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstExpContext* SysYParser::constExp() {
  ConstExpContext *_localctx = _tracker.createInstance<ConstExpContext>(_ctx, getState());
  enterRule(_localctx, 62, SysYParser::RuleConstExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(376);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool SysYParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 25: return mulExpSempred(antlrcpp::downCast<MulExpContext *>(context), predicateIndex);
    case 26: return addExpSempred(antlrcpp::downCast<AddExpContext *>(context), predicateIndex);
    case 27: return relExpSempred(antlrcpp::downCast<RelExpContext *>(context), predicateIndex);
    case 28: return eqExpSempred(antlrcpp::downCast<EqExpContext *>(context), predicateIndex);
    case 29: return lAndExpSempred(antlrcpp::downCast<LAndExpContext *>(context), predicateIndex);
    case 30: return lOrExpSempred(antlrcpp::downCast<LOrExpContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SysYParser::mulExpSempred(MulExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::addExpSempred(AddExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::relExpSempred(RelExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::eqExpSempred(EqExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 3: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::lAndExpSempred(LAndExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::lOrExpSempred(LOrExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void SysYParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sysyParserInitialize();
#else
  ::antlr4::internal::call_once(sysyParserOnceFlag, sysyParserInitialize);
#endif
}
