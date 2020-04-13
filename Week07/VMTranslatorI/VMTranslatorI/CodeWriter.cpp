#include "CodeWriter.h"
#include <stdexcept>
#include <sstream>
#include <memory>
#include <iostream>

//-------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------
CodeWriter::CodeWriter(const std::filesystem::path& outputFilePath)
  : m_output_file{ outputFilePath },
    m_output_file_path{ outputFilePath },
    m_is_file_open{ m_output_file.is_open() },
    m_dump_commands{ false }
{
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeArithmetic
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writeArithmetic(const std::string& instruction)
{
  std::unique_ptr<CommandProcessor> upALP = std::make_unique<ArithmeticLogicalProcessor>();
  upALP->loadFunctions();
  std::string instructions{ "" };

  try {
    instructions = upALP->processCmd(instruction);
    m_output_file << instructions << '\n';
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n'
      << "Unbale to process instruction" << instruction << '\n';
    throw;
  }
  return instructions;
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writePushPop
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writePushPop(const std::vector<std::string>& tokens)
{
  MemoryAccessProcessor mlp{ *this };
  mlp.loadFunctions();
  std::string instructions{ "" };
  try {
    instructions = mlp.processCmd(tokens);
    m_output_file << instructions << '\n';
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n'
      << "Unbale to process push/pop instruction" << '\n';
    throw;
  }
  return instructions;
}

//-------------------------------------------------------------------------------------------
// CodeWriter::getCurrentFileName
//-------------------------------------------------------------------------------------------
std::string CodeWriter::getCurrentFileName() const noexcept
{
  return m_output_file_path.stem().string();
}


//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::Constructor
//-------------------------------------------------------------------------------------------
ArithmeticLogicalProcessor::ArithmeticLogicalProcessor()
  : CommandProcessor{}
{
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string ArithmeticLogicalProcessor::processCmd(const std::string& cmd)
{
  return m_function[cmd]();
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::loadFunctions
//-------------------------------------------------------------------------------------------
void ArithmeticLogicalProcessor::loadFunctions()
{
  if (m_function.size()) return;
  const char nl{ '\n' };

  auto arithmetic = [=](const std::string& cmd) {
    std::ostringstream oss;
    oss << "@SP\n"
        << "AM=M-1"   << " // SP--\n"
        << "D=M"      << " // D = *SP\n"
        << "A=A-1"    << " // A = SP - 1\n"
        << cmd << '\n'  << '\n';

    return oss.str();
  };

  auto logical = [=](const std::string& cmd, const std::string& jmp) {
    static size_t index{ 0 };
    std::ostringstream oss;
    oss << "@SP\n"
      << "AM=M-1" << " // SP--\n"
      << "D=M" << " // D = *SP\n"
      << "A=A-1" << " // A = SP - 1\n"
      << "D=M-D" << " // D = arg1 - arg2\n"

      // if D jmp 0 then goto _$EQUAL$EQ#index else continue
      << "@_$FALSESTATE$" << std::to_string(index) << nl
      << "D;" << jmp << nl
      << nl

      // RAM[SP] = -1; arg1 cmd arg2, goto _$CONTINUESTATE$#index
      << "@SP\n"
      << "A=M-1\n"
      << "M=-1" << " // arg1 " << cmd <<" arg2\n"
      << "@_$CONTINUESTATE$" << std::to_string(index) << nl
      << "0;JMP\n"
      << nl

      // label _$FALSESTATE$#index
      // if arg1 != arg2, RAM[SP] = 0 
      << "(_$FALSESTATE$" << std::to_string(index) << ")" << nl
      << "  @SP\n"
      << "  A=M-1\n"
      << "  M=0" << " // !(arg1 " << cmd << " arg2)\n"

      // label _$FALSESTATE$#index
      << "(_$CONTINUESTATE$" << std::to_string(index) << ")" << nl
      << nl;

    index++;
    return oss.str();
  };

  m_function["add"] = [=]() { 
    std::ostringstream oss;
    oss << "// add\n" << arithmetic("M=D+M");
    return oss.str(); 
  };

  m_function["sub"] = [=]() {
    std::ostringstream oss;
    oss << "// sub\n" << arithmetic("M=M-D");
    return oss.str();
  };

  m_function["neg"] = [=]() {
    std::ostringstream oss;
    oss << "// neg\n"
      << "@SP\n"
      << "A=M-1\n"
      << "M=-M\n" << nl;
    return oss.str();
  };

  m_function["eq"] = [=]() {
    std::ostringstream oss;
    oss << "// eq\n" << logical("==", "JNE") << nl;
    return oss.str();
  };

  m_function["gt"] = [=]() {
    std::ostringstream oss;
    oss << "// gt\n" << logical(">", "JLE") << nl;
    return oss.str();
  };

  m_function["lt"] = [=]() {
    std::ostringstream oss;
    oss << "// lt\n" << logical("<", "JGE") << nl;
    return oss.str();
  };

  m_function["and"] = [=]() {
    std::ostringstream oss;
    oss << "// and\n" << arithmetic("M=D&M");
    return oss.str();
  };

  m_function["or"] = [=]() {
    std::ostringstream oss;
    oss << "// or\n" << arithmetic("M=D|M");
    return oss.str();
  };

  m_function["not"] = [=]() {
    std::ostringstream oss;
    oss << "// not\n"
      << "@SP\n"
      << "A=M-1\n"
      << "M=!M\n" << nl;
    return oss.str();
  };
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::Constructor
//-------------------------------------------------------------------------------------------
MemoryAccessProcessor::MemoryAccessProcessor(const CodeWriter& cw)
  : m_function{},
    m_code_writer{ cw }

{
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string MemoryAccessProcessor::processCmd(const std::vector<std::string>& tokens)
{
  std::string vmcmd{ "" };
  std::string segment{ "" };
  int argValue{ 0 };
  try {
    vmcmd = tokens.at(0);
    segment = tokens.at(1);
    argValue = std::stoi(tokens.at(2));
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw;
  }

  return m_function[vmcmd][segment](argValue);
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::loadFunctions
//-------------------------------------------------------------------------------------------
void MemoryAccessProcessor::loadFunctions()
{
  if (m_function.size()) return;
  const char nl{ '\n' };

  auto push = [=]() {
    std::ostringstream oss;
    oss << "@SP" << nl
        << "M=M+1" << " // SP++" << nl
        << nl;
    return oss.str();
  };

  auto pop = [=]() {
    std::ostringstream oss;
    oss << "@SP" << nl
        << "M=M-1" << " // SP--, pop arg2 " << nl
        << nl;
    return oss.str();
  };

  auto loadIndexInD = [=](int index) {
    std::ostringstream oss;
    oss << "@" << std::to_string(index) << nl
        << "D=A" << nl
        << nl;
    return oss.str();
  };

  auto writeDtoRamSP = [=]() {
    std::ostringstream oss;
    oss << "// *SP = D" << nl
      << "@SP" << nl
      << "A=M" << nl
      << "M=D" << nl
      << nl;
    return oss.str();
  };

  auto writeRamSPtoD = [=]() {
    std::ostringstream oss;
    oss << "// D = *SP" << nl
      << "@SP" << nl
      << "A=M" << nl
      << "D=M" << nl
      << nl;
    return oss.str();
  };

  auto pushSegment = [=](const std::string& id, const std::string& segment, int index) {
    std::ostringstream oss;
    oss << "// push " << id << ' ' << index << nl

      // D = index
      << "@" << std::to_string(index) << nl
      << "D=A" << nl
      << nl

      // D = RAM[segment + index] 
      << "@" << segment << nl
      << "A=D+M" << " // A = " << segment <<  " + " << index << nl
      << "D=M" << " // D = RAM[" << segment << " + " << index << nl
      << nl

      // *SP = D
      << "@SP" << nl
      << "A=M" << nl
      << "M=D" << nl
      << nl

      // SP++
      << "@SP" << nl
      << "M=M+1" << " // SP++" << nl

      << nl;
      return oss.str();
  };

  auto popToSegment = [=](const std::string& id, const std::string& segment, int index) {
    std::ostringstream oss;
    oss << "// pop " << id << ' '<< index << nl

      // D = index
      << "@" << std::to_string(index) << nl
      << "D=A" << nl
      << nl

      // D = segment + index
      << "@" << segment << nl
      << "D=D+M" << " // D = " << segment << " + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << "// D = RAM[SP--]\n"
      << "@SP\n" 
      << "AM=M-1\n"
      << "D=M\n"
      << nl

      // RAM[LCL + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[LCL + index] = *SP" << nl
      << nl
      << nl;

    return oss.str();
  };

  m_function["push"]["local"] = [=](int index) {
    std::ostringstream oss;
    oss << pushSegment("local","LCL", index);
    return oss.str();
  };

  m_function["push"]["argument"] = [=](int index) {
    std::ostringstream oss;
    oss << pushSegment("argument", "ARG", index);
    return oss.str();
  };

  m_function["push"]["this"] = [=](int index) {
    std::ostringstream oss;
    oss << pushSegment("this", "THIS", index);
    return oss.str();
  };

  m_function["push"]["that"] = [=](int index) {
    std::ostringstream oss;
    oss << pushSegment("that", "THAT", index);
    return oss.str();
  };

  m_function["push"]["constant"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push constant " << index << nl
      << loadIndexInD(index)
      << writeDtoRamSP()
      << push();
    return oss.str();
  };

  m_function["push"]["static"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push static " << index << '\n'
      << loadIndexInD(index)

      << "@" << m_code_writer.getCurrentFileName() << '.' << index << nl
      << "D=M" << nl
      << nl

      << writeDtoRamSP()
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["temp"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push temp " << index << '\n'
      << loadIndexInD(index)

      // *SP = RAM[5 + index]
      << "@R5" << nl
      << "A=D+A" << nl
      << "D=M" << nl

      << writeDtoRamSP()
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["pointer"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push pointer " << index << '\n';
      
    if (0 == index) {
      oss << "@THIS" << nl
          << "D=M" << nl;
    }
    if (1 == index) {
      oss << "@THAT" << nl
          << "D=M" << nl;
    }
    oss << writeDtoRamSP()
        << push() << nl;

    return oss.str();
  };


  m_function["pop"]["local"] = [=](int index) {
    std::ostringstream oss;
    oss << popToSegment("local", "LCL", index);
    return oss.str();
  };

  m_function["pop"]["argument"] = [=](int index) {
    std::ostringstream oss;
    oss << popToSegment("argument", "ARG", index);
    return oss.str();
  };

  m_function["pop"]["this"] = [=](int index) {
    std::ostringstream oss;
    oss << popToSegment("this", "THIS", index);
    return oss.str();
  };

  m_function["pop"]["that"] = [=](int index) {
    std::ostringstream oss;
    oss << popToSegment("that", "THAT", index);
    return oss.str();
  };

  m_function["pop"]["static"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop static " << index << nl

      // D = *SP
      << "@SP\n"
      << "AM=M-1\n"
      << "D=M\n"

      // @filename.index = D
      << "@" << m_code_writer.getCurrentFileName() << '.' << index << nl
      << "M=D" << " // " << "@" << m_code_writer.getCurrentFileName() << '.' << index << " = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["temp"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop temp " << index << nl

      // D = R5 + index
      << loadIndexInD(index)
      << "@R5" << nl
      << "D=D+A" << " // D = R5 + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      // D = *SP
      << "@SP\n"
      << "AM=M-1\n"
      << "D=M\n"

      // RAM[R5 + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[R5 + index] = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["pointer"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop pointer " << index << nl

      // D = *SP
      << "@SP\n"
      << "AM=M-1\n"
      << "D=M\n";

    if (0 == index) {
      oss << "@THIS" << nl
          << "M=D" << nl;
    }
    if (1 == index) {
      oss << "@THAT" << nl
          << "M=D" << nl;
    }
    
      oss << nl;
    return oss.str();
  };

}
