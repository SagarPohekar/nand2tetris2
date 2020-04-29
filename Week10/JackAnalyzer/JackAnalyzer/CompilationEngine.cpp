#include "CompilationEngine.h"


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
