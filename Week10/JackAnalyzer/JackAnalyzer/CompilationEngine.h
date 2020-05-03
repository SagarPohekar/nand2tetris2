#ifndef __COMPILATIONENGINE_H__
#define __COMPILATIONENGINE_H__

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <tuple>
#include <map>
#include <queue>
#include "JackTokenizer.h"
#include "XMLWriter.h"

class CompilationEngine
{
public:
  using pair_t = std::pair<std::string, JackTokenizer::TokenType>;

public:
  explicit CompilationEngine(const std::filesystem::path& sourceFilePath);
  ~CompilationEngine() noexcept;

public:
  /**
  * Compiles complete class
  */
  void compileClass();

  /**
  * Compiles static or field declarations
  */
  void compileClassVarDec();

  /**
  * Compiles complete method, function or constructor
  */
  void compileSubroutine();

  /**
  * Compiles possibly empty parameter list
  * not including ()
  */
  void compileParameterList();

  /**
  * Compiles var declaration
  */
  void compileVarDec();

  /**
  * Compiles sequence of statements , not
  * including   enclosing {}
  */
  void compileStatements();

  /**
  * Compiles 'do' statements
  */
  void compileDo();

  /**
  * Compiles 'let' statements
  */
  void compileLet();

  /**
  * Compiles 'while' statements
  */
  void compileWhile();

  /**
  * Compiles 'return' statements
  */
  void compileReturn();

  /**
  * Compiles 'if' statements possibly with trailing else clause
  */
  void compileIf();

 
  /**
  * Compiles an expression
  */
  void compileExpression();

  /**
  * Compiles a term. This routine is faced With a slight difficulty
  * when trying to decide between some of the alternative parsing
  * rules. Specifically, if the current token is an identifier,
  * the routine must distinguish between a variable, an array entry,
  * and a subroutine call.
  * A single look-ahead token, which may be one Of ' ' , or to
  * distinguish between the three possibilities. Any other token is not
  * part of this term and should not be advanced over.
  */
  void compileTerm();

  /**
  * Compiles a (possibly empty) comma-separated list or expressions.
  */
  void compileExpressionList();

  
  std::queue<pair_t>& getNextToken();

  [[nodiscard]] const std::filesystem::path getSourceFilePath() const noexcept { return m_source_file_path; }
private:
  bool writeToken(bool res, const char* msg = "");
  bool compileKeyword(const std::string& keyword);
  bool compileSymbol(const std::string& symbol);
  bool compileIdentifier(const std::string& keyword);
  bool compileType(const std::string& keyword);

private:
  std::filesystem::path m_source_file_path;
  std::filesystem::path m_output_file_path;
  std::ofstream m_output_file;
  JackTokenizer m_jack_tokenizer;
  XMLWriter m_xml_writer;
  std::queue<pair_t> m_token_q;
  using Method = void (CompilationEngine::*)();
  std::map<JackTokenizer::KeywordSubtype, Method> m_methods;
  void InitalizeMethodMap();
};

#endif // !__COMPILATIONENGINE_H__
