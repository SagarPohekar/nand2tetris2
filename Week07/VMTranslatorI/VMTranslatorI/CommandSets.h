#pragma once
#include <string>
#include <set>

struct CommandSets
{
  using string_set_t = std::set<std::string>;
  static const string_set_t m_arithmetic;
  static const string_set_t m_memory;
  static const string_set_t m_branching;
  static const string_set_t m_functional;
  static const string_set_t m_memory_segments;
};

