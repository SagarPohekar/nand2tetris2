#include "CompilationEngine.h"
#include <stack>
#include <sstream>
#include <string>
using namespace std::string_literals;

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
  
  auto fetchToken = [](CompilationEngine* cePtr) {
    if (!cePtr) return CompilationEngine::pair_t{};
    auto& q = cePtr->getNextToken();
    if (q.empty()) return CompilationEngine::pair_t{};
    else return q.front();
  };

  auto is_valid = [](const CompilationEngine::pair_t& p) { return p.second != JackTokenizer::TokenType::Invalid; };
}

using namespace Utils;
using namespace Tk;
using Indent = XMLWriter::Indent;
using KeywordSubtype = JackTokenizer::KeywordSubtype;
using TokenType = JackTokenizer::TokenType;

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
  if (m_jack_tokenizer.hasMoreTokens() && m_token_q.empty()) {
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

bool CompilationEngine::writeToken(bool res, const char* msg)
{
  pair_t tokentype = fetchToken(this);

  if (!res) {
    std::cerr << "Invalid token " << tokentype.second << '\n';
    return false;
  }
  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeTag(typeToString(tokentype.second), tokentype.first, Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
  m_token_q.pop();

  return true;
}

void CompilationEngine::InitalizeMethodMap()
{
  m_methods[JackTokenizer::KeywordSubtype::LET] = &CompilationEngine::compileLet;
  m_methods[JackTokenizer::KeywordSubtype::DO] = &CompilationEngine::compileDo;
  m_methods[JackTokenizer::KeywordSubtype::IF] = &CompilationEngine::compileIf;
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

  if (!m_output_file.is_open()) {
    std::cerr << "Unable to create output file" << '\n';
    return;
  }
  
  auto assertOut = [=](const bool res, std::string_view msg) {
    if (!res) {
      std::cerr << "Error illformed source file" << this->getSourceFilePath().filename() << ':' << msg << '\n';
      std::exit(-1);
    }
   
  };

  pair_t tokentype = fetchToken(this);

  // write <class>
  m_xml_writer.writeOpeningTag("class");
  if ( !writeToken( tokentype.second == TokenType::Keyword, "class") ) return;

  // className
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Identifier,
                    "className") ) return;

  // '{'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol && 
                    tokentype.first == "{"s,
                    "{") ) return;

  compileClassVarDec();
  compileSubroutine();

  // '}'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "}"s, 
                    "}") ) return;

  // write </class>
  m_xml_writer.writeClosingTag("class");
}

void CompilationEngine::compileClassVarDec()
{
  /*Grammar         :
  * classVarDec 	  : ('static' | 'field') type varName(',' varName)* ';'
  * type		        : 'int' | 'char' | 'boolean' | className
  * varName         : identifier
  */

  pair_t tokentype = fetchToken(this);

  while (is_valid(tokentype) && is_in(tokentype.first, Tk::STATIC, Tk::FIELD)) {
    // <classVarDec>
    m_xml_writer.setIndentation(1);
    m_xml_writer.writeOpeningTag("classVarDec", Indent::NoChange);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Keyword &&
                      is_in(tokentype.first, "static"s, "field"s),
                      "static | field") ) return;

    // type
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                      "type") ) return;

    // varName
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Identifier,
                      "varName") ) return;

    tokentype = fetchToken(this);
    while (is_valid(tokentype) && tokentype.first == ",") {
      // ','
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == ","s,
                        ",") ) return;

      // varName
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Identifier,
                        "varName") ) return;

      tokentype = fetchToken(this);
    }

    // ';'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol && 
                      tokentype.first == ";"s,
                      ";") ) return;

    // </classVarDec>
    m_xml_writer.setIndentation(1);
    m_xml_writer.writeClosingTag("classVarDec", Indent::NoChange);

    tokentype = fetchToken(this);
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

  pair_t tokentype = fetchToken(this);

  if (is_valid(tokentype) && is_in(tokentype.first, Tk::CONSTRUCTOR, Tk::FUNCTION, Tk::METHOD)) {
    while (is_valid(tokentype) && is_in(tokentype.first, Tk::CONSTRUCTOR, Tk::FUNCTION, Tk::METHOD)) {
      m_xml_writer.setIndentation(1);
      m_xml_writer.writeOpeningTag("subroutineDec", Indent::NoChange);

      // constructor | function | method
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Keyword && 
                        is_in(tokentype.first, "constructor"s, "function"s, "method"s),
                        "constructor | function | method") ) return;

      // type
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                        "type") ) return;

      // soubroutineName
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Identifier,
                        "soubroutineName") ) return;

      // '('
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == "("s,
                        "(") ) return;

      compileParameterList(); // parameterList	  : ((type varName) (',' type varName)*)?

      // ')'
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == ")"s,
                        ")") ) return;

      // subroutineBody	: '{' varDec* statements '}'
      m_xml_writer.setIndentation(2);
      m_xml_writer.writeOpeningTag("subroutineBody", Indent::NoChange);

      // '{'
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == "{"s,
                        "{") ) return;

      compileVarDec();
      compileStatements();

      // '}'
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == "}"s,
                        "}") ) return;

      m_xml_writer.setIndentation(2);
      m_xml_writer.writeClosingTag("subroutineBody", Indent::NoChange);

      m_xml_writer.setIndentation(1);
      m_xml_writer.writeClosingTag("subroutineDec", Indent::NoChange);
    }
  }
  
}

void CompilationEngine::compileParameterList()
{
  /*
  * type		        : 'int' | 'char' | 'boolean' | className
  * parameterList	  : ((type varName) (',' type varName)*)?
  * subroutineBody	: '{' varDec* statements '}'
  * varDec		      : 'var' type varName (',' varName)* ';'
  * className	      : identifier
  * varName         : identifier
  */

  pair_t tokentype = fetchToken(this);

  m_xml_writer.setIndentation(3);
  m_xml_writer.writeOpeningTag("parameterList", Indent::NoChange);

  if (is_valid(tokentype) && !(tokentype.first == ")")) {

    // type
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                      "type") ) return;

    // varName
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Identifier,
                      "varName") ) return;

    tokentype = fetchToken(this);
    while (is_valid(tokentype) && tokentype.first == ",") {
      // ','
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first ==  ","s,
                        ",") ) return;

      // type
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                        "type") ) return;

      // varName
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Identifier,
                        "varName") ) return;

      tokentype = fetchToken(this);
    }
  } // End if (!(tokentype.first == ")")) {
  m_xml_writer.setIndentation(3);
  m_xml_writer.writeClosingTag("parameterList", Indent::NoChange);
}

void CompilationEngine::compileVarDec()
{
  /*
  * varDec		      : 'var' type varName (',' varName)* ';'
  * type		        : 'int' | 'char' | 'boolean' | className
  * className	      : identifier
  * varName         : identifier
  */

  pair_t tokentype = fetchToken(this);

  if (is_valid(tokentype) && tokentype.first == "var"s) {

    m_xml_writer.setIndentation(3);
    m_xml_writer.writeOpeningTag("varDec", Indent::NoChange);

    // var keyword
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Keyword && 
                      tokentype.first == "var"s,
                      "var") ) return;

    // type
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                      "type") ) return;

    // varName
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Identifier,
                      "varName") ) return;

    tokentype = fetchToken(this);
    while (is_valid(tokentype) && tokentype.first == ",") {

      // ','
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Symbol &&
                        tokentype.first == ","s,
                        ",") ) return;

      // type
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        is_in(tokentype.second, TokenType::Keyword, TokenType::Identifier),
                        "type") ) return;

      // varName
      tokentype = fetchToken(this);
      if ( !writeToken( is_valid(tokentype) &&
                        tokentype.second == TokenType::Identifier,
                        "varName") ) return;

      tokentype = fetchToken(this);
    }

    // ';'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == ";"s,
                      ";") ) return;

    m_xml_writer.setIndentation(3);
    m_xml_writer.writeClosingTag("varDec", Indent::NoChange);
  } // if (is_valid(tokentype) && tokentype.first == "var"s)

  // Recursively complie susequent varDec
  tokentype = fetchToken(this);
  if (is_valid(tokentype) && tokentype.first == "var"s) {
    compileVarDec();
  }
}

void CompilationEngine::compileStatements()
{
  /*
  * statements      : statement*
  * statement       : letStatement | ifStatement | whileStatement | doStatement | returnStatement

  * letStatement    : 'let' varName ( '[' expression ']')? '=' expression ';'
  * ifStatement     : 'if' '(' expression ')' '{' statements '}'
  *                   ( 'else' '{' statements '}' )?
  * whileStatement  : 'while' '(' expression ') '{' statements '}'
  * doStatement     : 'do' subroutineCall ';'
  * returnStatement : 'return' expression? ';'
  */

  pair_t tokentype = fetchToken(this);
  std::stack<std::string> brackets{};
  brackets.emplace("{");

  while (!brackets.empty()) { // unless last '{' not found try to compile statements

    if (is_in(tokentype.first, "{"s, "}"s)) {
      if (tokentype.first == "{"s) brackets.emplace("{"s);
      else if (tokentype.first == "}"s && brackets.top() == "{"s) brackets.pop();
      else { std::cerr << "ill-formed code"; }
    }

    // Check if the statement is one of the allowed clause
    // If yes, then fetch its respective method and call it on 'this'
    if (is_in(tokentype.first, Tk::LET, Tk::WHILE, Tk::DO, Tk::IF, Tk::RETURN)) {
      auto it = m_methods.find(m_jack_tokenizer.getKeywordSubtype());
      if (m_methods.end() != it) {
        auto method = (*it).second;
        (this->*method)();
      }
    }
    else {
      std::cerr << "ill-formed code, token: " << tokentype.first << " is not a statement." << '\n';
      break;
    }
    //m_token_q.pop();
    tokentype = fetchToken(this);
  }

}

void CompilationEngine::compileDo()
{
  /*
  * doStatement     : 'do' subroutineCall ';'
  * subroutineCall  : subroutineName '(' expressionList ') |
  *                   (className | varName) '.' subroutineName '(' expressionList ')
  */

  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("doStatement", Indent::NoChange);

  pair_t tokentype = fetchToken(this);

  // do keyword
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Keyword &&
                    tokentype.first == "do"s,
                    "do")) return;

  // subroutineCall
  // subroutineName | (className | varName)
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Identifier,
                    "subroutineName | className | varName")) return;

  // '.' or '('
  tokentype = fetchToken(this);
  if (tokentype.first == "."s) {
    
    // '.'
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "."s,
                      ".")) return;

    // subroutineName | (className | varName)
    tokentype = fetchToken(this);
    if (!writeToken(is_valid(tokentype) &&
      tokentype.second == TokenType::Identifier,
      "subroutineName | className | varName")) return;

    // ',('
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "("s,
                      "(")) return;

    compileExpressionList();

    // ')'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == ")"s,
                      ")")) return;

  }
  else if (tokentype.first == "("s) {
    // '('
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "("s,
                      "(")) return;

    compileExpressionList();

    // ')'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == ")"s,
                      ")")) return;
  }
  else {
    std::cerr << "File: " << m_source_file_path.filename() << "invalid do statement" << '\n';
    return;
  }

  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == ";"s,
                    ";")) return;

  m_xml_writer.writeClosingTag("doStatement", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);

}

void CompilationEngine::compileLet()
{
  /*
  * letStatement    : 'let' varName ( '[' expression ']')? '=' expression ';'
  */
  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("letStatement", Indent::NoChange);

  pair_t tokentype = fetchToken(this);

  // let keyword
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Keyword &&
                    tokentype.first == "let"s,
                    "let")) return;

  // varName
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Identifier,
                    "varName")) return;

  // '['
  tokentype = fetchToken(this);
  if (tokentype.first == "["s) {
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "["s,
                      "[")) return;

    compileExpression();

    // ']'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "]"s,
                      "]")) return;
  }

  // '='
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "="s,
                    "=")) return;

  compileExpression();

  // ';'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == ";"s,
                    ";")) return;

  m_xml_writer.writeClosingTag("letStatement", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}

void CompilationEngine::compileWhile()
{
  /*
  * whileStatement  : 'while' '(' expression ') '{' statements '}'
  */
  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("whileStatement", Indent::NoChange);

  pair_t tokentype = fetchToken(this);

  // while keyword
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Keyword &&
                    tokentype.first == "while"s,
                    "while")) return;

  // '('
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "("s,
                    "(")) return;

  compileExpression();

  // ')'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == ")"s,
                    ")")) return;
  
  // '{'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "{"s,
                    "{")) return;

  compileStatements(); // compile all statements enclosing '{' ... '}'

  // '}'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "}"s,
                    "}")) return;

  m_xml_writer.writeClosingTag("whileStatement", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}

void CompilationEngine::compileReturn()
{
  /*
  * returnStatement : 'return' expression? ';'
  */
  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("returnStatement", Indent::NoChange);

  pair_t tokentype = fetchToken(this);

  // return keyword
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Keyword &&
                    tokentype.first == "return"s,
                    "return")) return;

  // check if token is an expression?
  tokentype = fetchToken(this);
  if (tokentype.first != ";"s) { // expression
    compileExpression();
  }

  // ';'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == ";"s,
                    ";")) return;

  m_xml_writer.writeClosingTag("returnStatement", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}

void CompilationEngine::compileIf()
{
  /*
  * ifStatement     : 'if' '(' expression ')' '{' statements '}'
  *                   ( 'else' '{' statements '}' )?
  */
  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("ifStatement", Indent::NoChange);

  pair_t tokentype = fetchToken(this);

  // if keyword
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Keyword &&
                    tokentype.first == "if"s,
                    "if")) return;

  // '('
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "("s,
                    "(")) return;

  compileExpression();

  // ')'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == ")"s,
                    ")")) return;

  // '{'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "{"s,
                    "{")) return;

  compileStatements(); // compile all statements enclosing '{' ... '}'

  // '}'
  tokentype = fetchToken(this);
  if ( !writeToken( is_valid(tokentype) &&
                    tokentype.second == TokenType::Symbol &&
                    tokentype.first == "}"s,
                    "}")) return;

  tokentype = fetchToken(this);
  if (tokentype.first == "else"s) {

    // else keyword
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Keyword &&
                      tokentype.first == "else"s,
                      "else")) return;

    // '{'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "{"s,
                      "{")) return;

    compileStatements(); // compile all statements enclosing '{' ... '}'

    // '}'
    tokentype = fetchToken(this);
    if ( !writeToken( is_valid(tokentype) &&
                      tokentype.second == TokenType::Symbol &&
                      tokentype.first == "}"s,
                      "}")) return;

  }

  m_xml_writer.writeClosingTag("ifStatement", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}

void CompilationEngine::compileExpression()
{
  /*
  * expression      : term (op term)*
  *
  */

  auto is_op = [=](const auto& str) {
    return Tk::is_in(str, "+"s , "-"s , "*"s , "/"s , "&"s , "|"s , "<"s , ">"s , "="s);
  };

  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("expression", Indent::NoChange);

  compileTerm();

  pair_t tokentype = fetchToken(this);
  while (is_op(tokentype.first)) {
    compileTerm();
    tokentype = fetchToken(this);
  }

  m_xml_writer.writeClosingTag("expression", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}

void CompilationEngine::compileTerm()
{
  /*
  * term            : intergerConstant | stringConstant | keywordConstant | varName
  *                   varName '[' expression ']' | subroutineCall | '(' expression ') |
  *                   unaryOp term
  *
  * subroutineCall  : subroutineName '(' expressionList ') |
  *                   (className | varName) '.' subroutineName '(' expressionList ')
  *
  * expressionList  : (expression( ',' expression)*)?
  *
  * op              : '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
  *
  * unaryOp         : '-' | '~'
  *
  * keywordConstant : 'true' | 'false' | 'null' | 'this'
  */
  // intergerConstant     : [0-9]+
  // stringConstant       : ^\".+$\"
  // keywordConstant      : "true" | "false" | "null" | "this"
  // varName
  // varName '[' expression ']'
  // subroutineCall
  // '(' expression ')
  // unaryOp term


}

void CompilationEngine::compileExpressionList()
{
  /*
  * expressionList  : (expression( ',' expression)*)?
  */

  m_xml_writer.setIndentation(Indent::Increase);
  m_xml_writer.writeOpeningTag("expressionList", Indent::NoChange);
  pair_t tokentype = fetchToken(this);

  if (tokentype.first != ")"s) {
    compileExpression();
    tokentype = fetchToken(this);
    while (tokentype.first == ","s) {
      compileExpression();
      tokentype = fetchToken(this);
    }
  }

  m_xml_writer.writeClosingTag("expressionList", Indent::NoChange);
  m_xml_writer.setIndentation(Indent::Decrease);
}
