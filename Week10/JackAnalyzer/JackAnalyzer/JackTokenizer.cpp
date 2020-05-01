#include "JackTokenizer.h"
#include "DebugUtils.hpp"
#include <algorithm>
#include <sstream>

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
#if JK_DEBUG
  auto outputFilePath = sourceFilePath;
  outputFilePath = outputFilePath.replace_extension(".xmltst");
  m_output_file.open(outputFilePath);
  if (m_output_file.is_open()) {
    m_output_file << writeXMLTag("tokens","",true, false);
  }
#endif 
}

//-------------------------------------------------------------------------
// JackTokenizer :: Destructor
//-------------------------------------------------------------------------
JackTokenizer::~JackTokenizer() noexcept
{
  if (m_is_open) {
    m_source_file.close();
  }
#if JK_DEBUG
  if (m_output_file.is_open()) {
    m_output_file << writeXMLTag("tokens", "", false, true);
    m_output_file.close();
  }
#endif 
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
    if ('\"' == ch) {
      token += ch;
      ch = m_source_file.peek();
      while ('\"' != ch) {
        token += m_source_file.get();
        ch = m_source_file.peek();
      }
      token += m_source_file.get();
      m_source_file.get();
      break;
    }
    else {
      token += ch;
    }
    ch = m_source_file.get();
    
  } while (m_source_file.good() && !m_source_file.eof() && !is_symbol(ch) && !is_symbol(token[0]) && !is_keyword(token) && !std::isblank(ch));
  m_source_file.unget();
  compute_token_type(token);

#if JK_DEBUG
  writeCurrentTokenToXML();
#endif
}

//-------------------------------------------------------------------------
// JackTokenizer :: advance2
//-------------------------------------------------------------------------
#include <iterator>
void JackTokenizer::advance2()
{
  if (!m_is_open) {
    Debug::Error("Unable to open file"s + m_source_file_path.string());
    return;
  }

  auto skipUpToChar = [=](std::ifstream& is, char ch) {
    while (ch != is.get()) {  }
  };

  auto addCharsToTokenUntilChar = [=](std::ifstream& is, auto& token, char ch) {
    token += ch;
    do {
      token += is.get();
    } while (ch != is.peek());
    token += ch;
    is.get();
    is.get();
  };

  auto skipUpToMultiCommentEnd = [=](std::ifstream& is, char prevChar, char nextChar) {

    char prev{ prevChar };
    char next{ nextChar };

    while (is.good() && !('*' == prev && '/' == next)) {
      prev = is.get();
      next = is.peek();
    }
    is.get(); // match '/'
  };

  std::string token{ "" };
 
  char curCh{ static_cast<char>(m_source_file.get()) };
  do
  {
    while (m_source_file.good()) {
      const char nxtCh = m_source_file.peek();

      if ('/' == curCh && '/' == nxtCh) { // skip inline comments
        skipUpToChar(m_source_file, '\n');
      }
      else if ('/' == curCh && '*' == nxtCh) { // skip multiline comments
        skipUpToMultiCommentEnd(m_source_file, '/', '*');
      }
      else if ('\n' == curCh || std::isblank(curCh)) {
        //curCh = m_source_file.get();
      } // skip newline
      else {
        break;
      }
      curCh = m_source_file.get();
    }

    if ('\"' == curCh) { // process string constant
      addCharsToTokenUntilChar(m_source_file, token, '\"');
      break;
    }
    else if (is_keyword(token)) { // keyword
      if (!is_symbol(curCh)) {
        token += curCh;
        curCh = m_source_file.get();
        continue;
      }
      break;
    }
    else if (is_symbol(token[0])) { // symbol
      break;
    }
    else {
      token += curCh;
    }
    curCh = m_source_file.get();
  } while (m_source_file.good() && !is_symbol(curCh) && !std::isblank(curCh));
  m_source_file.unget();
  
  compute_token_type(token);

#if JK_DEBUG
  writeCurrentTokenToXML();
#endif

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
    m_curr_token = m_curr_token.substr(1, m_curr_token.size() - 2ull);
  }
  else if (m_source_file.good()) {
    m_token_type = JackTokenizer::TokenType::Identifier;
  }
  else {
    m_curr_token = "";
    m_token_type = JackTokenizer::TokenType::Invalid;
  }
  
}

#if JK_DEBUG
//-------------------------------------------------------------------------
// JackTokenizer :: writeCurrentTokenToXML
//-------------------------------------------------------------------------
void JackTokenizer::writeCurrentTokenToXML()
{
  if (m_output_file.is_open()) {
    switch (m_token_type) {
    case JackTokenizer::TokenType::Keyword:
      m_output_file << writeXMLTag("keyword", m_curr_token);
      break;
    case JackTokenizer::TokenType::Symbol:
      if (m_curr_token == "<") {
        m_output_file << writeXMLTag("symbol", "&lt;");
      }
      else if (m_curr_token == ">") {
        m_output_file << writeXMLTag("symbol", "&gt;");
      }
      else if (m_curr_token == "&") {
        m_output_file << writeXMLTag("symbol", "&amp;");
      }
      else {
        m_output_file << writeXMLTag("symbol", m_curr_token);
      }
      break;
    case JackTokenizer::TokenType::IntegerConstant:
      m_output_file << writeXMLTag("integerConstant", m_curr_token);
      break;
    case JackTokenizer::TokenType::StringConstant:
      m_output_file << writeXMLTag("stringConstant", m_curr_token);
      break;
    case JackTokenizer::TokenType::Identifier:
      m_output_file << writeXMLTag("identifier", m_curr_token);
      break;
    case JackTokenizer::TokenType::Invalid:
    default:
      break;
    }
  }
}

//-------------------------------------------------------------------------
// JackTokenizer :: writeXMLTag
//-------------------------------------------------------------------------
std::string JackTokenizer::writeXMLTag(std::string_view tag, std::string_view val, bool opening, bool closing)
{
  std::ostringstream oss;
  if (opening) {
    oss << "<" << tag << '>';
  }
  if (val.size()) {
    oss << ' ' << val << ' ';
  }
  if (closing) {
    oss << "</" << tag << '>';
  }
  oss << '\n';
  return oss.str();
}
#endif

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
