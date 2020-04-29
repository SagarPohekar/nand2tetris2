# CMakeList.txt : CMake project for JackAnalyzer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (JackAnalyzer "JackAnalyzer.cpp" "JackAnalyzer.h"
                             "JackTokenizer.h" "JackTokenizer.cpp"
                             "CompilationEngine.h" "CompilationEngine.cpp"
                             "DebugUtils.hpp")

# TODO: Add tests and install targets if needed.
# Set CPP Standard
set (CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /Zc:__cplusplus")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
endif()
