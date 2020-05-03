#pragma once
#ifndef __JACKTOKENIZER_H__
#define __JACKTOKENIZER_H__
#define JK_DEBUG 1

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <tuple>

namespace Tk
{
  constexpr const char* CLASS = "class";
  constexpr const char* CONSTRUCTOR = "constructor";
  constexpr const char* FUNCTION = "function";
  constexpr const char* METHOD = "method";
  constexpr const char* FIELD = "field";
  constexpr const char* STATIC = "static";
  constexpr const char* VAR = "var";
  constexpr const char* INT = "int";
  constexpr const char* CHAR = "char";
  constexpr const char* BOOLEAN = "boolean";
  constexpr const char* VOID = "void";
  constexpr const char* TRUE = "true";
  constexpr const char* FALSE = "false";
  constexpr const char* kNULL = "null";
  constexpr const char* THIS = "this";
  constexpr const char* LET = "let";
  constexpr const char* DO = "do";
  constexpr const char* IF = "if";
  constexpr const char* ELSE = "else";
  constexpr const char* WHILE = "while";
  constexpr const char* RETURN = "return";

  template<bool Or, typename T, typename... Args>
  bool CompareOR(const T& first, Args...args)
  {
    std::vector<T> container{ args... };
    if constexpr (Or) {
      return std::any_of(container.begin(), container.end(), [&](auto& c) { return c == first; });
    } else {
      return std::all_of(container.begin(), container.end(), [&](auto& c) { return c == first; });
    }
    
  }

  template<typename T>
  bool Compare(const T& first, const T& second)
  {
    return first == second;
  }

  template<typename First, typename ... T>
  bool is_in(First&& first, T&& ... t)
  {
    return ((first == t) || ...);
  }

}
class JackTokenizer
{
public:
  enum class TokenType : int {
    Invalid = -1,
    Keyword,
    Symbol,
    IntegerConstant,
    StringConstant,
    Identifier
  };
  enum class KeywordSubtype : int {
    Invalid = -1, CLASS, CONSTRUCTOR, FUNCTION, METHOD, FIELD, STATIC, VAR,
    INT, CHAR, BOOLEAN, VOID, TRUE, FALSE, kNULL, THIS, LET,
    DO, IF, ELSE, WHILE, RETURN
  };

public:
  explicit JackTokenizer(const std::filesystem::path& sourceFilePath);
  ~JackTokenizer() noexcept;

public:
  [[nodiscard]] inline const bool hasMoreTokens() const noexcept { return m_source_file.good(); }
  [[nodiscard]] inline const TokenType tokenType() const noexcept { return m_token_type; }
  void advance();
  void advance2();
  std::pair<std::string, TokenType> getCurrentToken();
  KeywordSubtype getKeywordSubtype();

private:
  bool is_keyword(const std::string& token) const noexcept;
  bool is_symbol(char s) const noexcept;
  void compute_token_type(const std::string& token);
#if JK_DEBUG
  std::string createXMLTag(std::string_view tag, std::string_view val, bool opening = true, bool closing = true);
  void writeCurrentTokenToXML();
#endif
private:
  std::ifstream m_source_file;
  bool m_is_open;
  const std::filesystem::path& m_source_file_path;
  std::string m_curr_token;
  TokenType m_token_type;
#if JK_DEBUG
  std::ofstream m_output_file;
#endif
private:
  static const std::unordered_set<std::string> ms_keywords;
  static const std::unordered_set<char> ms_symbols;
};

std::ostream& operator<<(std::ostream& os, JackTokenizer::TokenType);

#endif

