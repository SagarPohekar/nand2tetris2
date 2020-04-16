#include "Parser.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "CommandSets.h"

const CommandSets Parser::ms_command_set{};
//-------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------
Parser::Parser(const std::filesystem::path& sourceFilePath)
  : m_source_file_path{ sourceFilePath },
    m_current_instruction_count{ 0 },
    m_source_lines{},
    m_current_instruction{""},
    m_command_type{ CommandType::Invalid }

{
}

//-------------------------------------------------------------------------------------------
// Parser::readSourceFile
//-------------------------------------------------------------------------------------------
bool Parser::readSourceFile()
{
  std::ifstream sourceFile{ m_source_file_path };
  if (!sourceFile.is_open()) {
    std::cerr << "\nFailed to open file: " << m_source_file_path << '\n';
    return false;
  }

  int lineCount{ -1 };
  std::string line{ "" };

  while (std::getline(sourceFile, line))
  {
    if (line.empty() || (line.length() >= 2 && line.substr(0, 2) == "//")) continue;

    // remove inline comments if any
    auto pos = line.find("//");
    line = pos != line.npos ? line.substr(0, pos) : line;

    // remove leading white spaces
    auto whiteLeadPos = line.find_first_not_of(' ');
    if (whiteLeadPos != line.npos)
      line = line.substr(whiteLeadPos);

    // remove trailing white spaces
    auto whiteTrailPos = line.find_last_not_of(' ');
    if (whiteTrailPos != line.npos)
      line = line.substr(0, whiteTrailPos + 1ull);

    if (std::all_of(line.begin(), line.end(), [](const unsigned char c) { return std::isblank(c); }))
      line.clear();

    if (!line.empty()) m_source_lines.push_back(line);
    line.clear();
  }

  sourceFile.close();
  return true;
}

//-------------------------------------------------------------------------------------------
// Parser::hasMoreCommands
//-------------------------------------------------------------------------------------------
bool Parser::hasMoreCommands() const noexcept
{
  return m_current_instruction_count < m_source_lines.size();
}

//-------------------------------------------------------------------------------------------
// Parser::advance
//-------------------------------------------------------------------------------------------
std::string Parser::advance()
{
  try {
    m_current_instruction = m_source_lines.at(m_current_instruction_count++);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  computeCommandType();
  return m_current_instruction;
}

//-------------------------------------------------------------------------------------------
// Parser::getCurrentInstruction
//-------------------------------------------------------------------------------------------
std::string Parser::getCurrentInstruction() const noexcept
{
  return m_current_instruction;
}

//-------------------------------------------------------------------------------------------
// Parser::getCommandType
//-------------------------------------------------------------------------------------------
CommandType Parser::getCommandType() const noexcept
{
  return m_command_type;
}

//-------------------------------------------------------------------------------------------
// Parser::computeCommandType
//-------------------------------------------------------------------------------------------
void Parser::computeCommandType()
{
  if (m_current_instruction.empty()) return;

  std::istringstream iss{ m_current_instruction };
  std::string keyword{ "" };
  iss >> keyword;

  if (ms_command_set.m_arithmetic.end() != ms_command_set.m_arithmetic.find(keyword))
    m_command_type = CommandType::ArithmeticLogical;
  else if (ms_command_set.m_memory.end() != ms_command_set.m_memory.find(keyword))
    m_command_type = CommandType::MemoryAccess;
  else if (ms_command_set.m_branching.end() != ms_command_set.m_branching.find(keyword))
    m_command_type = CommandType::Branching;
  else if (ms_command_set.m_functional.end() != ms_command_set.m_functional.find(keyword))
    m_command_type = CommandType::Functional;
}

//-------------------------------------------------------------------------------------------
// Parser::getCommandTokens
//-------------------------------------------------------------------------------------------
std::vector<std::string> Parser::getCommandTokens() const
{
  std::string token{ "" };
  std::istringstream iss{ m_current_instruction };
  std::vector<std::string> tokens;

  switch (m_command_type)
  {
  case CommandType::ArithmeticLogical:
  {
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens.size() != 1)
      throw std::invalid_argument("Error! incorrect arithmetic/logical command.\n");

    break;
  }
  case CommandType::MemoryAccess:
  {
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens.size() != 3)
      throw std::invalid_argument("Error! incorrect number of arguments for memory segment command.\n");

    if (!ms_command_set.m_memory_segments.count(tokens[1]))
      throw std::invalid_argument("Error! incorrect memory segment.\n");

    // validity check for temp index
    int index{ std::stoi(tokens.at(2)) };
    if (tokens[1] == "temp" && !(0 <= index && index < 9))
      throw std::invalid_argument("temp index not within range\n");
    
    // validity check for pointer index
    if (tokens[1] ==  "pointer"   && !(0 == index || index == 1))
      throw std::invalid_argument("pointer index not within range\n");
   
    break;
  }
  case CommandType::Branching:
  {
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens.size() != 2)
      throw std::invalid_argument("Error! incorrect branching command.\n");

    break;
  }
  case CommandType::Functional:
  {
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens[0] != "return" && tokens.size() != 3)
      throw std::invalid_argument("Error! incorrect number function command.\n");
    break;
  }
  case CommandType::Invalid:
  default:
    std::cerr << "Error! Invalid keyword\n";
    break;
  }

  return tokens;
 
}
