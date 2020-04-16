#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include "Enums.h"
#include "CommandSets.h"

class Parser
{
public:
  explicit Parser(const std::filesystem::path& sourceFilePath);
  ~Parser() = default;
  Parser(const Parser&) = delete;
  Parser(Parser&&) noexcept = delete;
  Parser& operator=(const Parser&) = delete;
  Parser& operator=(Parser&&) noexcept = delete;

public:
  bool readSourceFile();
  [[nodiscard]] bool hasMoreCommands() const noexcept;
  std::string advance();
  std::string getCurrentInstruction() const noexcept;
  CommandType getCommandType() const noexcept;
  std::vector<std::string> getCommandTokens() const;


private:
  enum class InstructionType : size_t {

  };
  void computeCommandType();

private:
  std::filesystem::path     m_source_file_path;
  size_t                    m_current_instruction_count;
  std::vector<std::string>  m_source_lines;
  std::string               m_current_instruction;
  CommandType               m_command_type;
  static const CommandSets  ms_command_set;
};
