#ifndef __COMPILATIONENGINE_H__
#define __COMPILATIONENGINE_H__

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <tuple>
#include "JackTokenizer.h"

class CompilationEngine
{
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
  void compileClassVarDesc();

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
  void compileVarDesc();

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


private:

private:
  std::filesystem::path m_source_file_path;
  std::filesystem::path m_output_file_path;
  std::ofstream m_output_file;
  JackTokenizer m_jack_tokenizer;
};

#endif // !__COMPILATIONENGINE_H__


