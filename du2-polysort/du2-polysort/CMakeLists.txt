# CMakeList.txt : CMake project for du2-polysort, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (du2-polysort "main.cpp" "du2-polysort.h" "du2-polysort.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET du2-polysort PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
