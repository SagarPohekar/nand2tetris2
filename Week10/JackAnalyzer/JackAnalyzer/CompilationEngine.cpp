#include "CompilationEngine.h"
#include <queue>
#include <sstream>

namespace Utils
{
  std::string typeToString(JackTokenizer::TokenType type)
  {
    return [=](JackTokenizer::TokenType type) {
      std::ostringstream oss;
      oss << type;
      return oss.str();
    }(type);
  }
  std::string_view typeToString(std::string_view type)
  {
    return type;
  }
  
}
using Indent = XMLWriter::Indent;
using KeywordSubtype = JackTokenizer::KeywordSubtype;

CompilationEngine::CompilationEngine(const std::filesystem::path& sourceFilePath)
  : m_source_file_path{ sourceFilePath },
    m_output_file_path{ sourceFilePath },
    m_output_file{ m_output_file_path.replace_extension(".xmlt") },
    m_jack_tokenizer{ sourceFilePath },
    m_xml_writer{ m_output_file },
    m_token_q{},
    m_methods{}
{
  InitalizeMethodMap();
  compileClass();
}

CompilationEngine::~CompilationEngine() noexcept
{
}

std::queue<CompilationEngine::pair_t>& CompilationEngine::getNextToken()
{
  if (m_jack_tokenizer.hasMoreTokens()) {
    m_jack_tokenizer.advance();
    const auto& tokens = m_jack_tokenizer.getCurrentToken();
    const auto& [token, tokentype] = tokens;// m_jack_tokenizer.getCurrentToken();
    if (!token.empty() && JackTokenizer::TokenType::Invalid != tokentype) {
      std::clog << std::setw(15) << std::left << token << tokentype << '\n';
      m_token_q.emplace(tokens);
    }
  }
  return m_token_q;
}

void CompilationEngine::InitalizeMethodMap()
{
  
  m_methods[JackTokenizer::KeywordSubtype::CLASS] = &CompilationEngine::compileClass;
  m_methods[JackTokenizer::KeywordSubtype::CONSTRUCTOR] = &CompilationEngine::compileSubroutine;
  m_methods[JackTokenizer::KeywordSubtype::FUNCTION] = &CompilationEngine::compileSubroutine;
  m_methods[JackTokenizer::KeywordSubtype::METHOD] = &CompilationEngine::compileSubroutine;
  m_methods[JackTokenizer::KeywordSubtype::FIELD] = &CompilationEngine::compileClassVarDec;
  m_methods[JackTokenizer::KeywordSubtype::STATIC] = &CompilationEngine::compileClassVarDec;
  m_methods[JackTokenizer::KeywordSubtype::VAR] = &CompilationEngine::compileVarDec;
  //m_methods[JackTokenizer::KeywordSubtype::INT] = &CompilationEngine::compileVarStatements;
  //m_methods[JackTokenizer::KeywordSubtype::CHAR] = &CompilationEngine::compileVarStatements;
  //m_methods[JackTokenizer::KeywordSubtype::BOOLEAN] = &CompilationEngine::compileVarStatements;
  //m_methods[JackTokenizer::KeywordSubtype::VOID] = &CompilationEngine::
  //m_methods[JackTokenizer::KeywordSubtype::TRUE] = &CompilationEngine::
  //m_methods[JackTokenizer::KeywordSubtype::FALSE] = &CompilationEngine::
  //m_methods[JackTokenizer::KeywordSubtype::kNULL] = &CompilationEngine::
  m_methods[JackTokenizer::KeywordSubtype::THIS] = &CompilationEngine::compileExpression;
  m_methods[JackTokenizer::KeywordSubtype::LET] = &CompilationEngine::compileLet;
  m_methods[JackTokenizer::KeywordSubtype::DO] = &CompilationEngine::compileDo;
  m_methods[JackTokenizer::KeywordSubtype::IF] = &CompilationEngine::compileIf;
  //m_methods[JackTokenizer::KeywordSubtype::ELSE] = &CompilationEngine::compileIf;
  m_methods[JackTokenizer::KeywordSubtype::WHILE] = &CompilationEngine::compileWhile;
  m_methods[JackTokenizer::KeywordSubtype::RETURN] = &CompilationEngine::compileReturn;
}

void CompilationEngine::compileClass()
{


  /*
  * class		        : 'class' className '{'classVarDec* subroutineDec*'}'
  * classVarDec 	  : ('static' | 'field') type varName(',' varName)* ';'
  * type		        : 'int' | 'char' | 'boolean' | className
  * subroutineDec	  : ('constructor' | 'function' | 'method')
	*                   ('void' | type) soubroutineName '(' parameterList ')
	*                   subroutineBody
  * parameterList	  : ((type varName) (',' type varName)*)?
  * subroutineBody	: '{' varDesc* statements '}'
  * varDesc		      : 'var' type varName (',' varName)* ';'
  * className	      : identifier
  * subroutineName	: identifier
  * varName         : identifier
  */
  using namespace Utils;

  if (!m_output_file.is_open()) {
    std::cerr << "Unable to create output file" << '\n';
  }
  
  auto assertOut = [=](const bool res, std::string_view msg) {
    if (!res) {
      std::cerr << "Error illformed source file" << this->getSourceFilePath().filename() << ':' << msg << '\n';
      std::exit(-1);
    }
   
  };

  // write <class>
  m_xml_writer.writeOpeningTag("class", Indent::Default);

  pair_t tokentype{};

  // write class keyword
  auto& q = getNextToken();
  tokentype = q.front();
  assertOut(tokentype.first == "class", "");
  m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::Increase);
  q.pop();

  // write class identifier
  q = getNextToken();
  tokentype = q.front();
  m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
  q.pop();
  
  // write {
  q = getNextToken();
  tokentype = q.front();
  m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
  q.pop();

  // classVarDec
  compileClassVarDec();

  // subroutineDec
  compileSubroutine();

  // write }
  q = getNextToken();
  tokentype = q.front();
  m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
  q.pop();

  // write </class>
  m_xml_writer.writeClosingTag("class", Indent::Default);
}

void CompilationEngine::compileClassVarDec()
{
  /*Grammar         :
  * classVarDec 	  : ('static' | 'field') type varName(',' varName)* ';'
  * type		        : 'int' | 'char' | 'boolean' | className
  * varName         : identifier
  */

  using namespace Utils;

  auto& q = getNextToken();
  pair_t tokentype = q.front();
  while (Tk::CompareOR<true>(tokentype.first, Tk::STATIC, Tk::FIELD)) {
    // <classVarDec>
    m_xml_writer.writeOpeningTag("classVarDec", Indent::NoChange);
    m_xml_writer.setIndentation(2);

    // static or field
    m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
    q.pop();

    // type
    q = getNextToken();
    tokentype = q.front();
    m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
    q.pop();

    // varName
    q = getNextToken();
    tokentype = q.front();
    m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
    q.pop();

    // check token == ','
    q = getNextToken();
    tokentype = q.front();
    while (tokentype.first == ",") {
      // ','
      m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
      q.pop();

      // varName
      q = getNextToken();
      tokentype = q.front();
      m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
      q.pop();

      // next token
      q = getNextToken();
      tokentype = q.front();
    }

    // ';'
    m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
    q.pop();

    // next token
    q = getNextToken();
    tokentype = q.front();

    // </classVarDec>
    m_xml_writer.setIndentation(1);
    m_xml_writer.writeClosingTag("classVarDec", Indent::NoChange);
  }
}

void CompilationEngine::compileSubroutine()
{
  /*
  * type		        : 'int' | 'char' | 'boolean' | className
  * subroutineDec	  : ('constructor' | 'function' | 'method')
  *                   ('void' | type) soubroutineName '(' parameterList ')
  *                   subroutineBody
  * parameterList	  : ((type varName) (',' type varName)*)?
  * subroutineBody	: '{' varDec* statements '}'
  * varDec		      : 'var' type varName (',' varName)* ';'
  * className	      : identifier
  * subroutineName	: identifier
  * varName         : identifier
  */
  // next token
  auto& q = m_token_q;
  if (!m_token_q.empty()) {
    q = m_token_q;
  } 
  else {
    q = getNextToken();
  }
  
  pair_t tokentype = q.front();
  m_xml_writer.writeOpeningTag("subroutineDec", Indent::NoChange);

  m_xml_writer.writeClosingTag("subroutineDec", Indent::NoChange);
}

void CompilationEngine::compileParameterList()
{
}

void CompilationEngine::compileVarDec()
{
}

void CompilationEngine::compileVarStatements()
{
}

void CompilationEngine::compileDo()
{
}

void CompilationEngine::compileLet()
{
}

void CompilationEngine::compileWhile()
{
}

void CompilationEngine::compileReturn()
{
}

void CompilationEngine::compileIf()
{
}

void CompilationEngine::compileExpression()
{
}

void CompilationEngine::compileTerm()
{
}

void CompilationEngine::compileExpressionList()
{
}
