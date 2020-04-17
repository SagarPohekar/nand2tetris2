# CMakeList.txt : CMake project for VMTranslator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (VMTranslator "VMTranslator.cpp" "VMTranslator.h" "Parser.h" "Parser.cpp" "CodeWriter.h" "CodeWriter.cpp" "Enums.h" "CommandSets.h" "CommandSets.cpp")

# Set CPP Standard
set (CXX_STANDARD 17)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /Zc:__cplusplus")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
endif()

