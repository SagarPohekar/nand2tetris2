#pragma once
#include <string>
#include <map>
#include "Code.h"

class Code
{
public:
  explicit Code() = default;
  ~Code() = default;

public:
  std::string toBinary(const std::string& instruction);
private:
  enum class InstructionType : size_t { Invalid, A, C};
  InstructionType getInstructionType(const std::string& instruction);
  std::string processAInstruction(const std::string& instruction);
  std::string processCInstruction(const std::string& instruction);

  static std::map<std::string, std::string> ms_comp_table;
  static std::map<std::string, std::string> ms_dest_table;
  static std::map<std::string, std::string> ms_jump_table;
};
