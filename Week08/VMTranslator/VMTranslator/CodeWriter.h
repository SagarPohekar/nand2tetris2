#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <functional>
#include "Enums.h"
#include "CommandSets.h"


class CodeWriter
{

public:
  explicit CodeWriter(const std::filesystem::path& outputFilePath);
  ~CodeWriter();
  CodeWriter(const CodeWriter&) = delete;
  CodeWriter(CodeWriter&&) noexcept = delete;
  CodeWriter& operator=(const CodeWriter&) = delete;
  CodeWriter& operator=(CodeWriter&&) noexcept = delete;

public:
  inline const std::filesystem::path& getCurrentSourceFile() const noexcept { return m_source_file_path; }
  inline const std::string getCurrentSourceFileName() const noexcept { return m_source_file_path.stem().string(); }
  inline const bool isOutputFileOpen() const noexcept { return m_is_file_open; }
  inline void setCurrentSourceFile(const std::filesystem::path& fileNPath) noexcept { m_source_file_path = fileNPath; }
  inline void setDumpCommands(const bool value) noexcept { m_dump_commands = value; };

  void writeInit();
  std::string writeArithmetic(const std::vector<std::string>& tokens);
  std::string writePushPop(const std::vector<std::string>& tokens);
  std::string writeBranching(const std::vector<std::string>& tokens);
  std::string writeFunctional(const std::vector<std::string>& tokens);
  void writeCommentToOutputStream(const std::string& txt);

  template<typename CommandHandler>
  std::string writeCommand(const std::vector<std::string>& tokens, const char* errMsg)
  {
    CommandHandler cp{ *this };
    cp.loadFunctions();
    std::string instructions{ "" };
    try {
      instructions = cp.processCmd(tokens);
      this->m_output_file << instructions << '\n';
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n'
        << "Unbale to process " << errMsg << " instruction." << '\n';
      throw;
    }
    return instructions;
  }
private:
  std::ofstream m_output_file;
  std::filesystem::path m_source_file_path;
  const std::filesystem::path& m_output_file_path;
  bool m_is_file_open;
  bool m_dump_commands;
  static const CommandSets  ms_command_set;
};

template<typename Ky>
class CommandProcessor
{
public:
  CommandProcessor(const CodeWriter& cw) : m_code_writer{ cw }, m_function{}  {}
  virtual ~CommandProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens) {}
  void loadFunctions() {}

protected:
  const CodeWriter& m_code_writer;
  std::map<std::string, Ky> m_function;
};

class ArithmeticLogicalProcessor : public CommandProcessor< std::function<std::string()>>
{
public:
  explicit ArithmeticLogicalProcessor(const CodeWriter& cw) : CommandProcessor{ cw } {}
  ~ArithmeticLogicalProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();

};

class MemoryAccessProcessor : public CommandProcessor<std::map<std::string, std::function<std::string(int)>>>
{
public:
  using string_crt = const std::string&;
  explicit MemoryAccessProcessor(const CodeWriter& cw) : CommandProcessor{ cw } {}
  ~MemoryAccessProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();
};

class BranchingProcessor : public CommandProcessor<std::function<std::string(const std::string&)>>
{
public:
  using string_crt = const std::string&;
  explicit BranchingProcessor(const CodeWriter& cw) : CommandProcessor{ cw } {}
  ~BranchingProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();
};

class FunctionalProcessor : public CommandProcessor<std::function<std::string(const std::string&, int)>>
{
public:
  using string_crt = const std::string&;
  explicit FunctionalProcessor(const CodeWriter& cw) : CommandProcessor{ cw } {}
  ~FunctionalProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();
};