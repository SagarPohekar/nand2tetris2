#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include "Parser.h"
#include "Code.h"

Parser::Parser(const std::string& iFileName)
  : m_instruction_count{ 0 },
    m_file_name{ iFileName },
    m_source_lines{ }
{

}

Parser::~Parser()
{
}

std::string Parser::advance()
{
  if (m_source_lines.size() > m_instruction_count)
    return m_source_lines[m_instruction_count++];
  else
    return "";
}

void Parser::readSourceFileContent()
{
  std::ifstream sourceFile{ m_file_name };
  if (!sourceFile.is_open()) {
    std::cerr << "\nFailed to open file: " << m_file_name << '\n';
    return;
  }
  
  int lineCount{ -1 };
  std::string line{ "" };
 
  while (std::getline(sourceFile, line))
  {
    if (line.empty() || (line.length() >= 2 && line.substr(0, 2) == "//")) continue;

    // remove inline comments if any
    auto pos = line.find("//");
    line = pos != line.npos ? line.substr(0, pos) : line;

    // remove white spaces
    auto it = std::remove_if(line.begin(), line.end(), [](const char c) { return ' ' == c; });
    line.erase(it,line.end());
    
    if (!line.empty()) m_source_lines.push_back(line);
    line.clear();
  }
  
  sourceFile.close();
}

void Parser::parse()
{
  readSourceFileContent();
  auto defaultSymbolTable{ buildSymbolTable() };
  try { 
    stripLabels(defaultSymbolTable);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    throw e;
  }
  
  try {
    stripSymbols(defaultSymbolTable);
  }
  catch (const std::exception& e){
    std::cerr << e.what() << '\n';
    throw e;
  }
}

void Parser::writeToBinary(const char* outFile)
{
  Code code;
  std::string outFileName{ outFile };
  std::ofstream out{ outFileName + ".hack"  };
  if (!out.is_open()) {
    std::cerr << "File not opened" << '\n';
    return;
  }

  for (const auto& instruction : m_source_lines) {
    try {
      auto binaryString = code.toBinary(instruction);
      out << binaryString << '\n';
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      out.close();
      throw;
    }
  }
  out.close();;
}

void Parser::dump() const noexcept
{
  for (const auto& line : m_source_lines) {
    std::cout << line << '\n';
  }
}

std::map<std::string, size_t> Parser::buildSymbolTable()
{
  static std::map<std::string, size_t> symbolTable{};
  symbolTable["R0"] = 0;
  symbolTable["R1"] = 1;
  symbolTable["R2"] = 2;
  symbolTable["R3"] = 3;
  symbolTable["R4"] = 4;
  symbolTable["R5"] = 5;
  symbolTable["R6"] = 6;
  symbolTable["R7"] = 7;
  symbolTable["R8"] = 8;
  symbolTable["R9"] = 9;
  symbolTable["R10"] = 10;
  symbolTable["R11"] = 11;
  symbolTable["R12"] = 12;
  symbolTable["R13"] = 13;
  symbolTable["R14"] = 14;
  symbolTable["R15"] = 15;

  symbolTable["SP"]   = 0x0;
  symbolTable["LCL"]  = 0x1;
  symbolTable["ARG"]  = 0x2;
  symbolTable["THIS"] = 0x3;
  symbolTable["THAT"] = 0x4;
  symbolTable["SCREEN"] = 0x4000; // 16384;
  symbolTable["KBD"] = 0x6000; // 24576;

  return symbolTable;
}

void Parser::stripLabels(std::map<std::string, size_t>& iSymbolTable)
{
  std::vector<std::string> sourceLabelsStripped;
  sourceLabelsStripped.reserve(m_source_lines.size());

  size_t instructionCount{ 0 };
  for (const auto& line : m_source_lines)
  {
    auto last = line.size() - static_cast<size_t>(1);
    if (line[0] == '(' && line[last] == ')') {
      auto label = line.substr(1, last - static_cast<size_t>(1));
      if (label.empty()) throw std::invalid_argument("No Label");
      auto it = iSymbolTable.find(label);
      if (it == iSymbolTable.end())
        iSymbolTable[label] = instructionCount;
    }
    else {
      sourceLabelsStripped.emplace_back(line);
      instructionCount++;
    }
  }
  m_source_lines = sourceLabelsStripped;
}

void Parser::stripSymbols(std::map<std::string, size_t>& iSymbolTable)
{
  using namespace std::string_literals;
  size_t memoryLocation = 0x10;
  for (const auto& line : m_source_lines)
  {
    auto symbol = line.substr(1);
    if (line[0] == '@' && !std::all_of(symbol.cbegin(), symbol.cend(), [](const char c) { return std::isdigit(c); })) {
      if (symbol.empty()) throw std::invalid_argument("invalid location");
      auto it = iSymbolTable.find(symbol);
      if (it == iSymbolTable.end())
        iSymbolTable[symbol] = memoryLocation++;
    }
  }

  for (auto& line : m_source_lines)
  {
    auto symbol = line.substr(1);
    if (line[0] == '@') {
      auto it = iSymbolTable.find(symbol);
      if (it != iSymbolTable.end())
        line = "@"s + std::to_string(iSymbolTable[symbol]);
    }
  }
}
