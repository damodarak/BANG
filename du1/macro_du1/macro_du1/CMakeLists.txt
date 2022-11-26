# CMakeList.txt : CMake project for macro_du1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (macro_du1 "macro_du1.cpp" "macro_du1.h" "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET macro_du1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
