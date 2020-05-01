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
  void compileVarStatements();

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
  * Compiles complete class
  */
  void compileTerm();

  /**
  * Compiles complete class
  */
  void compileExpressionList();

  std::queue<pair_t>& getNextToken();

  [[nodiscard]] const std::filesystem::path getSourceFilePath() const noexcept { return m_source_file_path; }
private:

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
