# CMakeList.txt : CMake project for VMTranslatorI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (VMTranslatorI "VMTranslatorI.cpp" "VMTranslatorI.h" "Parser.h" "Parser.cpp" "CodeWriter.h" "CodeWriter.cpp" "Enums.h" "CommandSets.h" "CommandSets.cpp")

# Set CPP Standard
set (CXX_STANDARD 17)

#if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
#    target_compile_options(stxxl INTERFACE "/Zc:__cplusplus")
#endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /Zc:__cplusplus")
endif()