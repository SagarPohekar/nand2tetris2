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
void CodeWriter::writeArithmetic(const std::string& instruction)
{
  std::unique_ptr<CommandProcessor> upALP = std::make_unique<ArithmeticLogicalProcessor>();
  upALP->loadFunctions();
  m_output_file << upALP->processCmd(instruction) << '\n';
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writePushPop
//-------------------------------------------------------------------------------------------
void CodeWriter::writePushPop(const std::vector<std::string>& tokens)
{
  MemoryAccessProcessor mlp{ *this };
  mlp.loadFunctions();
  try {
    m_output_file << mlp.processCmd(tokens) << '\n';
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n'
      << "Unbale to process push/pop instruction" << '\n';
    throw;
  }
  
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

  m_function["add"] = []() { 
    std::ostringstream oss;
    oss << "// add"  << '\n'
      // pop arg2
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // store arg2 in D register
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1, SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // store arg1 + arg2 in D register
      << "@SP" << '\n'
      << "A=M" <<'\n'
      << "M=D+M" << " // D = arg1 + arg2" << '\n'
      << '\n'

      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';
    return oss.str(); 
  };

  m_function["sub"] = []() {
    std::ostringstream oss;
    oss << "// sub" << '\n'
      // pop arg2
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // store arg2 in D register
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1, SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // store arg1 - arg2 in RAM[SP] register
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=M-D" << " // *SP = arg1 - arg2" << '\n'
      << '\n'

      //// write result in *SP = D
      //<< "@SP" << '\n'
      //<< "A=M" << '\n'
      //<< "M=D" << " // *SP = D" << '\n'
      //<< '\n'
      
      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';
    return oss.str();
  };

  m_function["neg"] = []() {
    std::ostringstream oss;
    oss << "// neg" << '\n'

      // pop arg, SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg " << '\n'
      << '\n'

      // Negate the D = -RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=-M" << " // *SP = -arg" << '\n'
      << '\n'

      //// RAM[SP] = D
      //<< "@SP" << '\n'
      //<< "A=M" << '\n'
      //<< "M=D" << " // RAM[SP] = -arg" << '\n'
      //<< '\n'

      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';

    return oss.str();
  };

  m_function["eq"] = []() {
    static size_t index{ 0 };
    std::ostringstream oss;
    oss << "// eq" << '\n'

      //pop arg2 SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // store D = RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // D = arg1 - arg2
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M-D" << " // D = arg1 - arg2" << '\n'
      << '\n'

      // if D == 0 then goto _$EQUAL$EQ#index else continue
      << "@_$EQUAL$EQ" << std::to_string(index) << '\n'
      << "D;JEQ" << '\n'
      << '\n'

      // RAM[SP] = 0; arg1 != arg2, goto _$END$EQ#index
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=0" << " // arg1 != arg2" << '\n'
      << "@_$END$EQ" << std::to_string(index) << '\n'
      << "0;JMP" << '\n'
      << '\n'

      // label _$EQUAL$EQ#index
      // if arg1 == arg2, RAM[SP] = -1 (set all bits to 1), goto _$END$EQ#index
      << "(_$EQUAL$EQ" << std::to_string(index) <<")" << '\n'
      << "  @SP" << '\n'
      << "  A=M" << '\n'
      << "  M=-1" << "// arg1 == arg2" << '\n'
      << "  @_$END$EQ" << std::to_string(index) << '\n'
      << "  0;JMP" << '\n'
      << '\n'

      // Label _$END$EQ#index, SP++
      << "(_$END$EQ" << std::to_string(index) << ")" << '\n'
      << "  @SP" << '\n'
      << "  M=M+1" << " // SP++" << '\n'
      << '\n';

    index++;
    return oss.str();
  };

  m_function["gt"] = []() {
    static size_t index{ 0 };
    std::ostringstream oss;
    oss << "// gt" << '\n'

      // pop arg2 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // D = RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1, SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // if (arg1 > arg2)  goto _$GREATER$GT#index else continue
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M-D" << " // D = arg1 - arg2" << '\n'
      << "@_$GREATER$GT" << std::to_string(index) << '\n'
      << "D;JGT" << '\n'
      << '\n'

      // if !(arg1 > arg2) 
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=0" << " // arg1 < arg2" << '\n'
      << "@_$END$GT" << std::to_string(index) << '\n'
      << "0;JMP" << '\n'
      << '\n'

      // Label _$GREATER$GT#index
      << "(_$GREATER$GT" << std::to_string(index) <<")" << '\n'
      << "  @SP" << '\n'
      << "  A=M" << '\n'
      << "  M=-1" << "// arg1 == arg2" << '\n'
      << "  @_$END$GT" << std::to_string(index) << '\n'
      << "  0;JMP" << '\n'
      << '\n'

      // Label _$END$GT#index, SP++
      << "(_$END$GT" << std::to_string(index) << ")" << '\n'
      << "  @SP" << '\n'
      << "  M=M+1" << " // SP++" << '\n'
      << '\n';

    index++;
    return oss.str();
  };

  m_function["lt"] = []() {
    static size_t index{ 0 };
    std::ostringstream oss;
    oss << "// lt" << '\n'

      // pop arg2 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // D = RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // if (arg1 > arg2) then goto _$LESSER$LT#index
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M-D" << " // D = arg1 - arg2" << '\n'
      << "@_$LESSER$LT" << std::to_string(index) << '\n'
      << "D;JLT" << '\n'
      << '\n'

      // !(arg1 < arg2)  
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=0" << " // arg1 != arg2" << '\n'
      << "@_$END$LT" << std::to_string(index) << '\n'
      << "0;JMP" << '\n'
      << '\n'

      // Label _$LESSER$LT
      // if (arg1 < arg2) RAM[SP] = -1
      << "(_$LESSER$LT" << std::to_string(index) << ")" << '\n'
      << "  @SP" << '\n'
      << "  A=M" << '\n'
      << "  M=-1" << "// arg1 == arg2" << '\n'
      << "  @_$END$LT" << std::to_string(index) << '\n'
      << "  0;JMP" << '\n'
      << '\n'

      // Label _$END$LT, SP++
      << "(_$END$LT" << std::to_string(index) << ")" << '\n'
      << "  @SP" << '\n'
      << "  M=M+1" << " // SP++" << '\n'
      << '\n';
    index++;
    return oss.str();
  };

  m_function["and"] = []() {
    std::ostringstream oss;
    oss << "// and" << '\n'

      // pop arg2 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // D = RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // RAM[SP] = arg1 & arg2
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=D&M" << " // *SP = arg1 & arg2" << '\n'
      << '\n'

      //// RAM[SP] = D
      //<< "@SP" << '\n'
      //<< "A=M" << '\n'
      //<< "M=D" << " // RAM[SP] = arg1 & arg2" << '\n'
      //<< '\n'

      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';

    return oss.str();
  };

  m_function["or"] = []() {
    std::ostringstream oss;
    oss << "// or" << '\n'

      // pop arg2 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg2 " << '\n'
      << '\n'

      // D = RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "D=M" << " // D = *SP" << '\n'
      << '\n'

      // pop arg1 , SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg1 " << '\n'
      << '\n'

      // RAM[SP] = arg1 | arg2
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=D|M" << " // M = arg1 | arg2" << '\n'
      << '\n'

      /*// RAM[SP] = D
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=D" << " // RAM[SP] = arg1 | arg2" << '\n'
      << '\n'*/

      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';

    return oss.str();
  };

  m_function["neg"] = []() {
    std::ostringstream oss;
    oss << "// neg" << '\n'

      // pop arg, SP--
      << "@SP" << '\n'
      << "M=M-1" << " // SP--, pop arg " << '\n'
      << '\n'

      // Negate the RAM[SP] = -RAM[SP]
      << "@SP" << '\n'
      << "A=M" << '\n'
      << "M=!M" << " // RAM[SP] = -arg" << '\n'
      << '\n'

      // SP++
      << "@SP" << '\n'
      << "M=M+1" << " // SP++" << '\n'
      << '\n';

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

  m_function["push"]["local"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push local" << index << nl
      << loadIndexInD(index) << nl

      // D = RAM[LCL + index] 
      << "@LCL" << nl
      << "A=D+A" << " // A = LCL + index" << nl
      << "D=M" << " // D = RAM[LCL + index]" << nl
      << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["argument"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push argument" << index << nl
      << loadIndexInD(index) << nl

      // D = RAM[ARG + index] 
      << "@ARG" << nl
      << "A=D+A" << " // A = ARG + index" << nl
      << "D=M" << " // D = RAM[ARG + index]" << nl
      << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["this"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push this" << index << nl
      << loadIndexInD(index) << nl

      // D = RAM[THIS + index] 
      << "@THIS" << nl
      << "A=D+A" << " // A = THIS + index" << nl
      << "D=M" << " // D = RAM[THIS + index]" << nl
      << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["that"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push that" << index << '\n'
      << loadIndexInD(index) << nl

      // D = RAM[THAT + index] 
      << "@THAT" << nl
      << "A=D+A" << " // A = THAT + index" << nl
      << "D=M" << " // D = RAM[THAT + index]" << nl
      << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["constant"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push constant" << index << nl
      << loadIndexInD(index) << nl
      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["static"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push static " << index << '\n'
      << loadIndexInD(index) << nl

      << "@" << m_code_writer.getCurrentFileName() << '.' << index << nl
      << "D=M" << nl
      << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["temp"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push temp" << index << '\n'
      << loadIndexInD(index) << nl

      // *SP = RAM[5 + index]
      << "@R5" << nl
      << "A=D+A" << nl
      << "D=M" << nl

      << writeDtoRamSP() << nl
      << push() << nl;
    return oss.str();
  };

  m_function["push"]["pointer"] = [=](int index) {
    std::ostringstream oss;
    oss << "// push pointer" << index << '\n';
      
    if (0 == index) {
      oss << "@THIS" << nl
          << "D=A" << nl;
    }
    if (1 == index) {
      oss << "@THAT" << nl
          << "D=A" << nl;
    }
    oss << writeDtoRamSP() << nl
        << push() << nl;

    return oss.str();
  };

  m_function["pop"]["local"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop local" << index << nl

      // D = LCL + index
      << loadIndexInD(index) << nl
      << "@LCL" << nl
      << "D=D+A" << " // D = LCL + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // RAM[LCL + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[LCL + index] = *SP" << nl
      << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["argument"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop argument" << index << nl

      // D = ARG + index
      << loadIndexInD(index) << nl
      << "@ARG" << nl
      << "D=D+A" << " // D = ARG + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // RAM[ARG + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[ARG + index] = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["this"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop this" << index << nl

      // D = THIS + index
      << loadIndexInD(index) << nl
      << "@THIS" << nl
      << "D=D+A" << " // D = THIS + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // RAM[THIS + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[THIS + index] = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["that"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop that" << index << nl

      // D = THAT + index
      << loadIndexInD(index) << nl
      << "@THAT" << nl
      << "D=D+A" << " // D = THAT + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // RAM[THAT + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[THAT + index] = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["static"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop static" << index << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // @filename.index = D
      << "@" << m_code_writer.getCurrentFileName() << '.' << index << nl
      << "M=D" << " // " << "@" << m_code_writer.getCurrentFileName() << '.' << index << " = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["temp"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop temp" << index << nl

      // D = R5 + index
      << loadIndexInD(index) << nl
      << "@R5" << nl
      << "D=D+A" << " // D = THAT + index" << nl
      << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl
      << nl

      << pop() << nl
      << writeRamSPtoD() << nl

      // RAM[R5 + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[THAT + index] = *SP" << nl
      << nl;

    return oss.str();
  };

  m_function["pop"]["pointer"] = [=](int index) {
    std::ostringstream oss;
    oss << "// pop pointer" << index << nl
      << pop() << nl
      << writeRamSPtoD() << nl;

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
