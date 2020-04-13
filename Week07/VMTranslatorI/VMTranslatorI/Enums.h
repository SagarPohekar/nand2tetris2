#pragma once
#define PROJECTPHASE 1

enum class CommandType : int {
  Invalid = -1,
  ArithmeticLogical,
  MemoryAccess,
  Branching,
  Functional
};
