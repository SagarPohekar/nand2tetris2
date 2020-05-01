#include "CompilationEngine.h"
#include <queue>

CompilationEngine::CompilationEngine(const std::filesystem::path& sourceFilePath)
  : m_source_file_path{ sourceFilePath },
    m_output_file_path{ sourceFilePath },
    m_output_file{ m_output_file_path.replace_extension(".xmlt") },
    m_jack_tokenizer{ sourceFilePath }
{
  compileClass();
}

CompilationEngine::~CompilationEngine() noexcept
{
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
  using tuple_t = std::tuple<std::string, JackTokenizer::TokenType>;
  std::queue<tuple_t> q;

  while (m_jack_tokenizer.hasMoreTokens()) {

    m_jack_tokenizer.advance();
    q.emplace(m_jack_tokenizer.getCurrentToken());
    const auto& [token, tokentype] = m_jack_tokenizer.getCurrentToken();
    if (!token.empty() && JackTokenizer::TokenType::Invalid != tokentype) {
      std::clog << std::setw(15) << std::left << token << tokentype << '\n';

    }
  }
}

void CompilationEngine::compileClassVarDesc()
{
}

void CompilationEngine::compileSubroutine()
{
}

void CompilationEngine::compileParameterList()
{
}

void CompilationEngine::compileVarDesc()
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
