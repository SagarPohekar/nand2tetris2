#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

enum class ArgType : size_t { Invalid,
                              PositionalWithVal,
                              PositionalWithoutVal,
                              OptionalWithVal,
                              OptionalWithoutVal
};

class ArgParser
{
public: // aliases
  using string_t = std::string;
  using string_ct = const string_t;
  struct ArgInfo {
    ArgType m_type;
    string_t m_short_hand;
    string_t m_full_arg;
    string_t m_help;
    string_t m_value;
    ArgInfo() = default;
    ArgInfo(ArgType type, string_ct& short_hand, string_ct& full_arg, string_ct& help)
      : m_type{ type }, m_short_hand{ short_hand }, m_full_arg{ full_arg }, m_help{ help },
        m_value{ "" }
    {
    }
  };
  using map_t = std::map<std::string, ArgInfo>;
  using list_t = std::vector<std::string>;
  
public: // constructor
  ArgParser(int argc, char* argv[]) {
    m_arg_list.reserve(static_cast<size_t>(argc));
    readArgv(argc, argv);
  }
  ~ArgParser() = default;

public: // API
  bool addArg(const ArgInfo& argInfo) {
    auto its = m_arg_map.find(argInfo.m_short_hand);
    auto itf = m_arg_map.find(argInfo.m_full_arg);
    if (its == m_arg_map.end() && itf == m_arg_map.end()) {
      m_arg_map[argInfo.m_short_hand] = argInfo;
      m_arg_map[argInfo.m_full_arg] = argInfo;
    }
    else {
      std::cerr << "Argument " << argInfo.m_short_hand << " already exists" << '\n';
      return false;
    }
    return true;
  }

  const ArgInfo& getArg(const string_t& short_hand) const noexcept {
    const auto it = m_arg_map.find(short_hand);
    if (it != m_arg_map.end()) {
      return (*it).second; // m_arg_map[short_hand];
    }
    else {
      std::cerr << "Argument " << short_hand << " does not exists" << '\n';
      return {};
    }
  }

  void parse() { 

    return; 
  }
private: // members
  list_t m_arg_list;
  map_t m_arg_map;
  
private: // methods
  void readArgv(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
      m_arg_list.emplace_back(argv[i]);
    }
  }
};


