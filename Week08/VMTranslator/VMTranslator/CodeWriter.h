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
  ~CodeWriter() = default;
  CodeWriter(const CodeWriter&) = delete;
  CodeWriter(CodeWriter&&) noexcept = delete;
  CodeWriter& operator=(const CodeWriter&) = delete;
  CodeWriter& operator=(CodeWriter&&) noexcept = delete;

public:
  const std::filesystem::path& getCurrentSourceFile() const noexcept { return m_source_file_path; }
  const std::string getCurrentSourceFileName() const noexcept { return m_source_file_path.stem().string(); }
  inline void setCurrentSourceFile(const std::filesystem::path& fileNPath) noexcept { m_source_file_path = fileNPath; }
  inline void setDumpCommands(const bool value) noexcept { m_dump_commands = value; };

  void writeInit();
  std::string writeArithmetic(const std::vector<std::string>& tokens);
  std::string writePushPop(const std::vector<std::string>& tokens);
  std::string writeBranching(const std::vector<std::string>& tokens);
  std::string writeFunctional(const std::vector<std::string>& tokens);
  void writeTextToOutputStream(const std::string& txt);

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
  CommandProcessor() :m_function{}  {}
  virtual ~CommandProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens) {}
  void loadFunctions() {}

protected:
  std::map<std::string, Ky> m_function;
};

class ArithmeticLogicalProcessor : public CommandProcessor< std::function<std::string()>>
{
public:
  ArithmeticLogicalProcessor();
  ~ArithmeticLogicalProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();
};

class MemoryAccessProcessor : public CommandProcessor<std::map<std::string, std::function<std::string(int)>>>
{
public:
  using string_crt = const std::string&;
  explicit MemoryAccessProcessor(const CodeWriter& cw);
  ~MemoryAccessProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();

private:
  const CodeWriter& m_code_writer;
};

class BranchingProcessor : public CommandProcessor<std::function<std::string(const std::string&)>>
{
public:
  using string_crt = const std::string&;
  explicit BranchingProcessor(const CodeWriter& cw);
  ~BranchingProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();

private:
  const CodeWriter& m_code_writer;
};

class FunctionalProcessor : public CommandProcessor<std::function<std::string(const std::string&, int)>>
{
public:
  using string_crt = const std::string&;
  explicit FunctionalProcessor(const CodeWriter& cw);
  ~FunctionalProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();

private:
  const CodeWriter& m_code_writer;
};