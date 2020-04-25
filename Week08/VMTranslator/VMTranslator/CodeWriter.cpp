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
    m_source_file_path{ },
    m_output_file_path{ outputFilePath },
    m_is_file_open{ m_output_file.is_open() },
    m_dump_commands{ false }
{
}

//-------------------------------------------------------------------------------------------
// CodeWriter::~CodeWriter
//-------------------------------------------------------------------------------------------
CodeWriter::~CodeWriter()
{
  if (m_is_file_open)
    m_output_file.close();
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeInit
//-------------------------------------------------------------------------------------------
void CodeWriter::writeInit()
{
  static bool called{ false };

  auto setValue = [=](const std::string& pointer, int value = 0) {
    std::ostringstream oss;
    oss << "// Set " << pointer << " to " << value << '\n'
      << "@" << value << '\n'
      << "D=A" << '\n'
      << "@" << pointer << '\n'
      << "M=-D" << '\n'
      << "@SP" << '\n'
      << "M=M+1" << '\n';
    return oss.str();
  };

  if (called) return;
  else {
    m_output_file << "// Bootstrap code\n"

      // Set SP = 256
      << "@261" << '\n'
      << "D=A" << '\n'
      << "@SP" << '\n'
      << "M=D" << '\n'
      //<< "@SP" << '\n'
      //<< "M=M+1" << '\n'
      
      //<< setValue("LCL",1)
      //<< setValue("ARG",2)
      //<< setValue("THIS",3)
      //<< setValue("THAT",4)
      //<< '\n'

      // call Sys.main
      << "@Sys.init" << '\n'
      << "0;JMP" << '\n'
      << '\n';

    called = true;
  }
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeArithmetic
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writeArithmetic(const std::vector<std::string>& tokens)
{
  return writeCommand<ArithmeticLogicalProcessor>(tokens, tokens[0].c_str());
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writePushPop
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writePushPop(const std::vector<std::string>& tokens)
{
  return writeCommand<MemoryAccessProcessor>(tokens, "push/pop");
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeBranching
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writeBranching(const std::vector<std::string>& tokens)
{
  return writeCommand<BranchingProcessor>(tokens, "branching");
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeFunctional
//-------------------------------------------------------------------------------------------
std::string CodeWriter::writeFunctional(const std::vector<std::string>& tokens)
{
  return writeCommand<FunctionalProcessor>(tokens, "functional");
}

//-------------------------------------------------------------------------------------------
// CodeWriter::writeCommnetToOutputStream
//-------------------------------------------------------------------------------------------
void CodeWriter::writeCommentToOutputStream(const std::string& txt)
{
  this->m_output_file << "// " << txt << '\n' << '\n';
}

//-------------------------------------------------------------------------------------------
// ArithmeticLogicalProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string ArithmeticLogicalProcessor::processCmd(const std::vector<std::string>& tokens)
{
  try {
    const std::string& vmcmd = tokens.at(0);
    return m_function[vmcmd]();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw;
  }
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
// MemoryAccessProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string MemoryAccessProcessor::processCmd(const std::vector<std::string>& tokens)
{
  try {
    const std::string& vmcmd{ tokens.at(0) };
    const std::string& segment{ tokens.at(1) };
    const int argValue{ std::stoi(tokens.at(2)) };
    return m_function[vmcmd][segment](argValue);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw;
  }
}

//-------------------------------------------------------------------------------------------
// MemoryAccessProcessor::loadFunctions
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

    if (0 == index) {
      oss << "D=0" << nl;
    }
    else if (1 == index) {
      oss << "D=1" << nl;
    }
    else {
      oss << "@" << std::to_string(index) << nl
        << "D=A" << nl;
    }
    return oss.str();
  };

  auto writeDtoRamSP = [=]() {
    std::ostringstream oss;
    oss
      << "@SP" << nl
      << "A=M" << nl
      << "M=D" << " // *SP = D" << nl;
    return oss.str();
  };

  auto writeRamSPtoD = [=]() {
    std::ostringstream oss;
    oss 
      << "@SP" << nl
      << "A=M" << nl
      << "D=M" << "// D = *SP" << nl;
 
    return oss.str();
  };

  auto pushSegment = [=](const std::string& id, const std::string& segment, int index) {
    std::ostringstream oss;
    oss << "// push " << id << ' ' << index << nl;

    // D = index
    if (0 == index) {
      oss << "D=0" << nl;
    }
    else if (1 == index) {
      oss << "D=1" << nl;
    }
    else {
      oss << "@" << std::to_string(index) << nl
        << "D=A" << nl;
    }
      
    // D = RAM[segment + index] 
    oss << "@" << segment << nl
      << "A=D+M" << " // A = " << segment <<  " + " << index << nl
      << "D=M" << " // D = RAM[" << segment << " + " << index << "]" << nl

      // *SP = D
      << "@SP" << nl
      << "A=M" << nl
      << "M=D" << nl

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

      // D = segment + index
      << "@" << segment << nl
      << "D=D+M" << " // D = " << segment << " + index" << nl

      // R13 = D (SP)
      << "@R13" << nl
      << "M=D" << " // R13 = D (SP)" << nl

      << "// D = RAM[SP--]" << nl
      << "@SP" << nl
      << "AM=M-1" << nl
      << "D=M" << nl

      // RAM[LCL + index] = *SP
      << "@R13" << nl
      << "A=M" << nl
      << "M=D" << " // RAM[LCL + index] = *SP" << nl
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

      << "@" << this->m_code_writer.getCurrentSourceFileName() << '.' << index << nl
      << "D=M" << nl
      << nl

      << writeDtoRamSP()
      << push();

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
      << push();
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
        << push();

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
      << "@" << this->m_code_writer.getCurrentSourceFileName() << '.' << index << nl
      << "M=D" << " // " << "@" << this->m_code_writer.getCurrentSourceFileName() << '.' << index << " = *SP" << nl
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


//-------------------------------------------------------------------------------------------
// BranchingProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string BranchingProcessor::processCmd(const std::vector<std::string>& tokens)
{
  try {
    const std::string& cmd{ tokens.at(0) };
    const std::string& arg{ tokens.at(1) };
    return m_function[cmd](arg);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw;
  }
}

//-------------------------------------------------------------------------------------------
// BranchingProcessor::loadFunctions
//-------------------------------------------------------------------------------------------
void BranchingProcessor::loadFunctions()
{
  if (m_function.size()) return;
  
  m_function["label"] = [=](const std::string& arg) {

    std::ostringstream oss;
    oss << "// label " << m_code_writer.getCurrentSourceFileName() << "$" << arg << '\n'
      << "(" << m_code_writer.getCurrentSourceFileName() << "$" << arg << ")" << '\n';
    return oss.str();
  };

  m_function["goto"] = [=](const std::string& arg) {

    std::ostringstream oss;
    oss << "// goto " << this->m_code_writer.getCurrentSourceFileName() << "$" << arg << '\n'
      << "@" << this->m_code_writer.getCurrentSourceFileName() << "$" << arg << '\n'
      << "0;JMP" << '\n';
    return oss.str();
  };

  m_function["if-goto"] = [=](const std::string& arg) {

    std::ostringstream oss;
    oss << "// if-goto " << this->m_code_writer.getCurrentSourceFileName() << "$" << arg << '\n'
      << "@SP" << '\n'
      << "AM=M-1" << '\n'
      << "D=M" << '\n'
      << "@" << this->m_code_writer.getCurrentSourceFileName() << "$" << arg << '\n'
      << "D;JNE" << '\n'
      << '\n';
    return oss.str();
  };
}


//-------------------------------------------------------------------------------------------
// FunctionalProcessor::processCmd
//-------------------------------------------------------------------------------------------
std::string FunctionalProcessor::processCmd(const std::vector<std::string>& tokens)
{
  try {
    const std::string& cmd{ tokens.at(0) };
    const std::string& func{ tokens.size() > 2 ? tokens.at(1) : "" };
    int nArgs{ std::stoi(tokens.size() > 2 ? tokens.at(2) : "-1") };
    return m_function[cmd](func, nArgs);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw;
  }
}

//-------------------------------------------------------------------------------------------
// FunctionalProcessor::loadFunctions
//-------------------------------------------------------------------------------------------
void FunctionalProcessor::loadFunctions()
{
  if (m_function.size()) return;

  MemoryAccessProcessor mcp{ m_code_writer };
  mcp.loadFunctions();
  std::vector<std::string> tokens{ "push", "","" };
  const char nl{ '\n' };

  m_function["function"] = [=](const std::string& arg, const int n) mutable {
    std::ostringstream oss;
    oss << "// function " << arg << nl
      << "(" << arg << ")" << nl
      // *LCL = *SP
      << "@SP" << nl
      << "D=M" << nl
      << "@LCL" << nl
      << "M=D" << nl;

    if (n) {
      // push 0th locals and initialize to 0
      oss << "D=0" << nl
          << "// *(LCL + 0) = 0" << nl
          << "@SP" << nl
          << "A=M" << nl
          << "M=D" << nl;

      //push n-1 local variables and initialize to 0
      for (int i = 1; i < n; i++) {
        oss << "// push local " << i << " = 0" << nl
          << "A=A+1" << nl
          << "M=D" << nl;
      }
      // SP = top of the stack

      oss << "// SP = top of the stack" << nl
        << "A=A+1" << nl
        << "D=A" << nl
        << "@SP" << nl
        << "M=D" << nl
        << nl;
    }
      
    return oss.str();
  };

  auto pushSegment = [=](const std::string& segment, const bool pushLabel = false) {
    std::ostringstream oss;
    oss << "// call : push " << segment << nl

      // D = RAM[segment] 
      << "@" << segment << nl
      << (pushLabel ? "D=A" : "D=M") <<  nl

      // *SP = D
      << "@SP" << nl
      << "A=M" << nl
      << "M=D" << nl

      // SP++
      << "@SP" << nl
      << "M=M+1" << nl
      << nl;

    return oss.str();
  };

  m_function["call"] = [=](const std::string& functionName, const int n) {
    static size_t index{ 0 };
    static std::map<std::string, int> cmd_map{};

    std::string retLabel{ m_code_writer.getCurrentSourceFileName() + "$ret." + std::to_string(index++) };
    std::ostringstream oss;
    std::string cmd{ "call$" };
    cmd += (functionName + "$" + std::to_string(n));

    if (!cmd_map.count(cmd)) {
      cmd_map[cmd] += 1;
      oss << "// call " << functionName << '\n'
        << "(" << cmd << ")" << nl
        // push all segments and return address
        << pushSegment("R15")
        << pushSegment("LCL")
        << pushSegment("ARG")
        << pushSegment("THIS")
        << pushSegment("THAT")

        // LCL = SP
        << "// set up LCL = SP" << nl
        << "@SP" << nl
        << "D=M" << nl
        << "@LCL" << nl
        << "M=D" << nl

        // ARG = LCL - 5 - nArgs
        << "// set up ARG = LCL - 5 - nArgs" << nl
        << "@" << (5 + n) << nl
        << "D=D-A" << nl
        << "@ARG" << nl
        << "M=D" << nl

        // goto arg
        << "// goto " << functionName << nl
        << "@" << functionName << nl
        << "0;JMP" << nl

        // return address label
        //<< "(" << retLabel << ")" << nl
        << nl;
    }
    
    oss << "// call " << cmd << nl
      << "@" << retLabel << nl
      << "D=A" << nl
      << "@R15" << nl
      << "M=D" << nl
      << "@" << cmd << nl
      << "0;JMP" << nl
      // return address label
      << "(" << retLabel << ")" << nl
      << nl;

    return oss.str();
  };

  m_function["return"] = [=](const std::string& arg, const int n) {

    static bool called{ false };
    /*
      To optimize return call, we'll write retrun block with label
      which will be generic for each return call
     */
    std::ostringstream oss;
    if (!called) {
      oss << "// return " << nl

        // store LCL in R13
        << "(" << "__$ReturnBlock" << ")" << nl
        << "// store LCL in R13" << nl
        << "@LCL" << nl
        << "D=M" << nl
        << "@R13" << nl
        << "M=D" << nl

        // store return address (LCL - 5) in R14
        << "// store return address (LCL - 5) in R14" << nl
        << "@5" << nl
        << "A=D-A" << nl
        << "D=M" << nl
        << "@R14" << nl
        << "M=D" << nl // store LCL - 5 in R14

        //Reposition return value in *(ARG + 0) = *(SP--)
        << "//Reposition return value in *(ARG + 0) = *(SP--)" << nl
        << "@SP" << nl
        << "A=M-1" << nl // pop SP--
        << "D=M" << nl
        << "@ARG" << nl
        << "A=M" << nl
        << "M=D" << nl

        // SP = ARG + 1
        << "// SP = ARG + 1" << nl
        << "@ARG" << nl
        << "D=M+1" << nl
        << "@SP" << nl
        << "M=D" << nl

        // THAT = *(R13) - 1
        << "// THAT = *(R13) - 1" << nl
        << "@R13" << nl
        << "D=M-1" << nl
        << "AM=D" << nl
        << "D=M" << nl
        << "@THAT" << nl
        << "M=D" << nl

        //THIS = *(R13) - 2
        << "//THIS = *(R13) - 2" << nl
        << "@R13" << nl
        << "D=M-1" << nl
        << "AM=D" << nl
        << "D=M" << nl
        << "@THIS" << nl
        << "M=D" << nl

        //ARG = *(R13) - 3
        << "//ARG = *(R13) - 3" << nl
        << "@R13" << nl
        << "D=M-1" << nl
        << "AM=D" << nl
        << "D=M" << nl
        << "@ARG" << nl
        << "M=D" << nl

        //LCL = *(R13) - 4
        << "//LCL = *(R13) - 4" << nl
        << "@R13" << nl
        << "D=M-1" << nl
        << "AM=D" << nl
        << "D=M" << nl
        << "@LCL" << nl
        << "M=D" << nl

        // goto ret address
        << "// goto ret address" << nl
        << "@R14" << nl
        << "A=M" << nl
        << "0;JMP" << nl
        << '\n';
    }
    
    oss << "// return " << nl
      << "@" << "__$ReturnBlock" << nl
      << "0;JMP" << nl
      << nl;

    called = true;
    return oss.str();
  };
}
