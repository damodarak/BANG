# CMakeList.txt : CMake project for bang, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (bang   "game/game.h" "game/game.cpp" "main.cpp" "game/player.cpp" "game/player.h" "game/card.h" "game/card.cpp" "game/char_willy.h" "game/char_willy.cpp" "game/char_calamity.h" "game/char_slab.h" "game/char_jourd.h" "game/blackj.h" "game/char_ketchum.h" "game/char_lucky.h" "game/char_vulture.h" "game/char_pedro.h" "game/char_bart.h" "game/char_rose.h" "game/char_suzy.h" "game/char_paul.h" "game/char_cringo.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET bang PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
