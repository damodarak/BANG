# CMakeList.txt : CMake project for bang, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (bang   "game/game.h" "game/game.cpp" "main.cpp" "game/player.cpp" "game/player.h" "game/card.h"  "game/characters/char_willy.h" "game/characters/char_willy.cpp" "game/characters/char_calamity.h" "game/characters/char_slab.h" "game/characters/char_jourd.h" "game/characters/char_blackj.h" "game/characters/char_ketchum.h" "game/characters/char_lucky.h" "game/characters/char_vulture.h" "game/characters/char_pedro.h" "game/characters/char_bart.h" "game/characters/char_rose.h" "game/characters/char_suzy.h" "game/characters/char_paul.h" "game/characters/char_cringo.h" "game/characters/characters.h" "game/characters/char_lucky.cpp"   "game/card_gun.h" "game/card_horse.h" "game/card_dostavnik.h" "game/card_dostavnik.cpp" "game/card_gun.cpp" "game/card_horse.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET bang PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
