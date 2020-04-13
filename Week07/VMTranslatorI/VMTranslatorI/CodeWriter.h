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
  inline void setDumpCommands(const bool value) noexcept { m_dump_commands = value; };
  std::string writeArithmetic(const std::string& instruction);
  std::string writePushPop(const std::vector<std::string>& tokens);
  std::string getCurrentFileName() const noexcept;

#if PROJECTPHASE > 1
  void writeBranching(const std::string& instruction);
  void writeFunctional(const std::string& instruction);
#endif
  
private:
  
private:
  

private:
  std::ofstream m_output_file;
  const std::filesystem::path& m_output_file_path;
  bool m_is_file_open;
  bool m_dump_commands;
  static const CommandSets  ms_command_set;
};

class CommandProcessor
{
public:
  CommandProcessor() :m_function{}  {}
  virtual ~CommandProcessor() = default;

public:
  virtual std::string processCmd(const std::string & cmd) = 0;
  virtual void loadFunctions() = 0;

protected:
  std::map<std::string, std::function<std::string()>> m_function;
};

class ArithmeticLogicalProcessor : public CommandProcessor
{
public:
  ArithmeticLogicalProcessor();
  virtual ~ArithmeticLogicalProcessor() = default;

public:
  virtual std::string processCmd(const std::string& cmd) override;
  virtual void loadFunctions() override;
};

class MemoryAccessProcessor 
{
public:
  using string_crt = const std::string&;
  explicit MemoryAccessProcessor(const CodeWriter& cw);
  ~MemoryAccessProcessor() = default;

public:
  std::string processCmd(const std::vector<std::string>& tokens);
  void loadFunctions();

private:
  std::map<std::string,
           std::map<std::string, std::function<std::string(int)>>> m_function;
  const CodeWriter& m_code_writer;
};