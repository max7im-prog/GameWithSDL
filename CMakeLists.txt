cmake_minimum_required(VERSION 3.20.0)
project(
  GameWithSDL
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/max7im-prog/game-box2d.git
  GIT_TAG v3.1.1
)
FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/max7im-prog/game-SDL.git
  GIT_TAG release-3.2.16
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/max7im-prog/game-entt.git
  GIT_TAG v3.15.0
)
FetchContent_MakeAvailable(entt)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/physics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/controls)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/body)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/creature)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/misc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/render)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/debug)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/systems)

set(EXTERNAL_LIBS
  box2d
  SDL3-shared
  EnTT::EnTT
)

set(GAME_LIBS
  game_body_lib
  game_controls_lib
  game_creature_lib
  game_debug_lib
  game_events_lib
  game_interfaces_lib
  game_misc_lib
  game_physics_lib
  game_render_lib
  game_systems_lib
)

add_library(game_run_lib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
)
target_include_directories(game_run_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(game_run_lib PUBLIC ${GAME_LIBS} ${EXTERNAL_LIBS})

add_executable(
  Game
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
target_link_libraries(Game PRIVATE game_run_lib)

target_compile_options(Game PRIVATE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -ggdb3 -fsanitize=address -fsanitize=undefined -fsanitize=leak>
  $<$<CONFIG:Release>:-Wall -Wextra -O3 -g -ggdb3>
)

target_link_options(Game PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined -fsanitize=leak>
)

