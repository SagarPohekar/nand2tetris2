#include "Code.h"
#include <iostream>
#include <stdexcept>
#include <bitset>

std::map<std::string, std::string> Code::ms_comp_table = {
  {"0",   "0101010"},
  {"1",   "0111111"},
  {"-1",  "0111010"},
  {"D",   "0001100"},
  {"A",   "0110000"},
  {"!D",  "0001101"},
  {"!A",  "0110001"},
  {"-D",  "0001111"},
  {"-A",  "0110011"},
  {"D+1", "0011111"},
  {"A+1", "0110111"},
  {"D-1", "0001110"},
  {"A-1", "0110010"},
  {"D+A", "0000010"},
  {"D-A", "0010011"},
  {"A-D", "0000111"},
  {"D&A", "0000000"},
  {"D|A", "0010101"},

  {"M",   "1110000"},
  {"!M",  "1110001"},
  {"-M",  "1110011"},
  {"M+1", "1110111"},
  {"M-1", "1110010"},
  {"D+M", "1000010"},
  {"D-M", "1010011"},
  {"M-D", "1000111"},
  {"D&M", "1000000"},
  {"D|M", "1010101"}
};

std::map<std::string, std::string> Code::ms_dest_table = {
  {"null" ,"000"},
  {"M"    ,"001"},
  {"D"    ,"010"},
  {"MD"   ,"011"},
  {"DM"   ,"011"},
  {"A"    ,"100"},
  {"AM"   ,"101"},
  {"MA"   ,"101"},
  {"AD"   ,"110"},
  {"DA"   ,"110"},
  {"AMD"  ,"111"},
  {"ADM"  ,"111"},
  {"MAD"  ,"111"},
  {"MDA"  ,"111"},
  {"DAM"  ,"111"},
  {"DMA"  ,"111"},
};

std::map<std::string, std::string> Code::ms_jump_table = {
 {"null", "000"},
  {"JGT", "001"},
  {"JEQ", "010"},
  {"JGE", "011"},
  {"JLT", "100"},
  {"JNE", "101"},
  {"JLE", "110"},
  {"JMP", "111"}
};

std::string Code::toBinary(const std::string& instruction)
{
  std::string out{ "" };
  auto type = getInstructionType(instruction);
  static const auto error_msg = std::string{ "Invalid instruction " } + instruction + "\n";
  switch (type)
  {
    
  case Code::InstructionType::Invalid:
    throw std::invalid_argument(error_msg.c_str());
    break;
  case Code::InstructionType::A:
    out = processAInstruction(instruction);
    break;
  case Code::InstructionType::C:
    out = processCInstruction(instruction);
    break;
  default:
    break;
  }
  return out;
}

Code::InstructionType Code::getInstructionType(const std::string& instruction)
{
  if (instruction.empty())
    return Code::InstructionType::Invalid;
  else if (instruction[0] == '@')
    return Code::InstructionType::A;
  else
    return Code::InstructionType::C;
}

std::string Code::processAInstruction(const std::string& instruction)
{
  auto location = instruction.substr(1);
  auto number = 0;
  try {
    number = std::stoi(location);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  std::bitset<16> bits = static_cast<int16_t>(number);
  return bits.to_string();
}

std::string Code::processCInstruction(const std::string& instruction)
{
  auto desPos = instruction.find('=');
  std::string desCmd = desPos != std::string::npos ? instruction.substr(0, desPos) : "null";

  auto jmpPos = instruction.find(';');
  std::string jmpCmd = jmpPos != std::string::npos ? instruction.substr(jmpPos + 1ull) : "null";

  std::string compCmd{ "" };
  if (desCmd != "null" && jmpCmd != "null")
    compCmd = instruction.substr(desPos,jmpPos);
  else if (desCmd == "null" && jmpCmd == "null")
    compCmd = instruction;
  else if (desCmd != "null")
    compCmd = instruction.substr(desPos + 1ull);
  else if (jmpCmd != "null")
    compCmd = instruction.substr(0, jmpPos);

  std::string binary{ "111" };

  if (ms_comp_table.end() != ms_comp_table.find(compCmd))
    binary += ms_comp_table[compCmd];
  else {
    std::string errorMsg{ "invalid instruction " + compCmd + '\n' };
    throw std::invalid_argument(errorMsg.c_str());
  }

  if (ms_dest_table.end() != ms_dest_table.find(desCmd))
    binary += ms_dest_table[desCmd];
  else {
    std::string errorMsg{ "invalid destination location " + desCmd + '\n' };
    throw std::invalid_argument(errorMsg.c_str());
  }

  if (ms_jump_table.end() != ms_jump_table.find(jmpCmd))
    binary += ms_jump_table[jmpCmd];
  else {
    std::string errorMsg{ "invalid jump instruction " + jmpCmd + '\n' };
    throw std::invalid_argument(errorMsg.c_str());
  }

  return binary;
}
