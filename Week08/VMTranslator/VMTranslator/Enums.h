#pragma once
#define PROJECTPHASE 2

enum class CommandType : int {
  Invalid = -1,
  ArithmeticLogical,
  MemoryAccess,
  Branching,
  Functional
};
