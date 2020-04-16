#include "CommandSets.h"

const CommandSets::string_set_t
CommandSets::m_arithmetic{ "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not" };

const CommandSets::string_set_t
CommandSets::m_memory{ "pop", "push" };

const CommandSets::string_set_t
CommandSets::m_branching{ "label", "goto", "if-goto" };

const CommandSets::string_set_t
CommandSets::m_functional{ "function", "call" , "return" };

const CommandSets::string_set_t
CommandSets::m_memory_segments{ "local", "argument", "this", "that", "constant", "static", "temp", "pointer"};