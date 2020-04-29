#include "JackTokenizer.h"
#include "DebugUtils.hpp"
#include <algorithm>

using namespace std::string_literals;

//-------------------------------------------------------------------------
// ms_keywords : initialization
//-------------------------------------------------------------------------
const std::unordered_set<std::string> JackTokenizer::ms_keywords{
  "class", "constructor", "function", "method", "field", "static", "var",
  "int", "char", "boolean", "void" ,"true", "false", "null", "this", "let",
  "do", "if", "else", "while", "return"
};

//-------------------------------------------------------------------------
// ms_symbols : initialization
//-------------------------------------------------------------------------
const std::unordered_set<char> JackTokenizer::ms_symbols{
  '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/',
  '&', '|', '<', '>', '=', '~'
};

//-------------------------------------------------------------------------
// JackTokenizer :: Constructor
//-------------------------------------------------------------------------
JackTokenizer::JackTokenizer(const std::filesystem::path& sourceFilePath)
  : m_source_file{ sourceFilePath },
    m_is_open{ m_source_file.is_open() },
    m_source_file_path{ sourceFilePath },
    m_curr_token{ "" },
    m_token_type{ JackTokenizer::TokenType::Invalid }
{
}

//-------------------------------------------------------------------------
// JackTokenizer :: Destructor
//-------------------------------------------------------------------------
JackTokenizer::~JackTokenizer() noexcept
{
  if (m_is_open) {
    m_source_file.close();
  }
}

//-------------------------------------------------------------------------
// JackTokenizer :: advance
//-------------------------------------------------------------------------
void JackTokenizer::advance()
{
  if (!m_is_open) {
    Debug::Error("Unable to open file"s + m_source_file_path.string());
    return;
  }

  auto seekUpToChar = [=](std::ifstream& is, char ch) {
    char seekChar{ 0 };
    do {
      seekChar = is.get();
    } while (is.good() && seekChar != ch);
  };

  auto seekUpToMultiLineEnd = [=](std::ifstream& is, char prevChar, char nextChar) {

    char prev{ prevChar };
    char next{ nextChar };

    while (is.good() && !('*' == prev && '/' == next)) {
      prev = is.get();
      next = is.peek();
    }
    is.get(); // match '/'
  };

  std::string token{ "" };
  char ch{ static_cast<char>(m_source_file.get()) };
  do {
    
    while (m_source_file.good()) {
      if ('/' == ch && m_source_file.peek() == '/') { // inline comment
        seekUpToChar(m_source_file, '\n');
      }
      else if ('/' == ch && m_source_file.peek() == '*') { // multiline comment
        seekUpToMultiLineEnd(m_source_file, ch, '/');
      }
      else if (std::isblank(ch) || '\n' == ch) {
        ch = m_source_file.peek();
      }
      else {
        break;
      }
      ch = m_source_file.get();
    }
    token += ch;
    ch = m_source_file.get();
    
  } while (m_source_file.good() && !m_source_file.eof() && !is_symbol(ch) && !is_symbol(token[0]) && !is_keyword(token) && !std::isblank(ch));
  m_source_file.unget();
  compute_token_type(token);
}

//-------------------------------------------------------------------------
// JackTokenizer :: getCurrentToken
//-------------------------------------------------------------------------
std::tuple<std::string, JackTokenizer::TokenType> JackTokenizer::getCurrentToken()
{
  return { m_curr_token, m_token_type };
}

//-------------------------------------------------------------------------
// JackTokenizer :: is_keyword
//-------------------------------------------------------------------------
bool JackTokenizer::is_keyword(const std::string& token) const noexcept
{
  return ms_keywords.count(token);
}

//-------------------------------------------------------------------------
// JackTokenizer :: is_symbol
//-------------------------------------------------------------------------
bool JackTokenizer::is_symbol(char s) const noexcept
{
  return ms_symbols.count(s) > 0;;
}

//-------------------------------------------------------------------------
// JackTokenizer :: computeTokenType
//-------------------------------------------------------------------------
void JackTokenizer::compute_token_type(const std::string& token)
{
  m_curr_token = token;
  if (token.empty()) {
    m_token_type = JackTokenizer::TokenType::Invalid;
  }
  else if (is_keyword(token)) {
    m_token_type = JackTokenizer::TokenType::Keyword;
  }
  else if (1 == token.size() && is_symbol(token[0])) {
    m_token_type = JackTokenizer::TokenType::Symbol;
  }
  else if (std::all_of(token.cbegin(), token.cend(), [](char c) { return std::isdigit(c);})) {
    int num = std::stoi(token);
    if (0 <= num && num < 32767) {
      m_token_type = JackTokenizer::TokenType::IntegerConstant;
    }
  }
  else if ('\"' == token[0] && '\"' == token[token.size() - 1ull]) {
    m_token_type = JackTokenizer::TokenType::StringConstant;
  }
  else if (m_source_file.good()) {
    m_token_type = JackTokenizer::TokenType::Identifier;
  }
  else {
    m_curr_token = "";
    m_token_type = JackTokenizer::TokenType::Invalid;
  }
  
}

std::ostream& operator<<(std::ostream& os, JackTokenizer::TokenType tk)
{
  switch (tk)
  {
  case JackTokenizer::TokenType::Invalid:
    os << "Invalid";
    break;
  case JackTokenizer::TokenType::Keyword:
    os << "Keyword";
    break;
  case JackTokenizer::TokenType::Symbol:
    os << "Symbol";
    break;
  case JackTokenizer::TokenType::IntegerConstant:
    os << "IntegerConstant";
    break;
  case JackTokenizer::TokenType::StringConstant:
    os << "StringConstant";
    break;
  case JackTokenizer::TokenType::Identifier:
    os << "Identifier";
    break;
  default:
    break;
  }
  return os;
}