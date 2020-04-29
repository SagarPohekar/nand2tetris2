#include "CompilationEngine.h"


CompilationEngine::CompilationEngine(const std::filesystem::path& sourceFilePath)
  : m_source_file_path{ sourceFilePath },
    m_output_file_path{ sourceFilePath },
    m_output_file{ m_output_file_path.replace_extension(".xmlt") },
    m_jack_tokenizer{ sourceFilePath }
{

}

CompilationEngine::~CompilationEngine() noexcept
{
}

void CompilationEngine::compileClass()
{
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
