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
  enum class Keyword : int {
    CLASS, CONSTRUCTOR, FUNCTION, METHOD, FIELD, STATIC, VAR,
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
  std::tuple<std::string, TokenType> getCurrentToken();

private:
  bool is_keyword(const std::string& token) const noexcept;
  bool is_symbol(char s) const noexcept;
  void compute_token_type(const std::string& token);
#if JK_DEBUG
  std::string writeXMLTag(std::string_view tag, std::string_view val, bool opening = true, bool closing = true);
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

