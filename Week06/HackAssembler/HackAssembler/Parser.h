#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <map>

class Parser
{
public:
  explicit Parser(const std::string& iFileName);
  ~Parser();

public:
  void parse();
  void writeToBinary(const char* outFile);
  void dump() const noexcept;
  
private:
  size_t m_instruction_count;
  std::vector<std::string> m_source_lines;
  std::string m_file_name;

  void readSourceFileContent();
  std::map<std::string, size_t> buildSymbolTable();
  void stripLabels(std::map<std::string, size_t>&);
  void stripSymbols(std::map<std::string, size_t>&);
  
  std::string advance();
  [[nodiscard]] inline auto getCurrentInstructionCount() const noexcept { return m_instruction_count; }
};
