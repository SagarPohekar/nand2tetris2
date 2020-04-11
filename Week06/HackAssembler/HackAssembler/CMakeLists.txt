# CMakeList.txt : CMake project for HackAssembler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (HackAssembler "HackAssembler.cpp" "HackAssembler.h"
	                          "Parser.cpp" "Parser.h"
							  "Code.h" "Code.cpp")
set (CMAKE_CXX_STANDARD 17)

# TODO: Add tests and install targets if needed.
