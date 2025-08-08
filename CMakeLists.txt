cmake_minimum_required(VERSION 3.20.0)
project(
  GameWithSDL
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/controls)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/body)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/debug)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/misc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/physics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/render)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/systems)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces)

set(EXTERNAL_LIBS
  box2d
  SDL3-shared
  EnTT::EnTT
)

set(GAME_ALL_SOURCES
  ${CONTROLS_SRC}
  ${CREATURE_SRC}
  ${DEBUG_SRC}
  ${EVENTS_SRC}
  ${MISC_SRC}
  ${PHYSICS_SRC}
  ${RENDER_SRC}
  ${SHAPE_SRC}
  ${SYSTEMS_SRC}
  ${CREATURE_SRC}
  ${BODY_SRC}
  ${INTERFACES_SRC}
  src/game.cpp
  src/game.hpp
  src/main.cpp
)

set(GAME_ALL_INCLUDE_DIRS
  ${CONTROLS_INCLUDE_DIRS}
  ${CREATURE_INCLUDE_DIRS}
  ${DEBUG_INCLUDE_DIRS}
  ${EVENTS_INCLUDE_DIRS}
  ${MISC_INCLUDE_DIRS}
  ${PHYSICS_INCLUDE_DIRS}
  ${RENDER_INCLUDE_DIRS}
  ${SHAPE_INCLUDE_DIRS}
  ${SYSTEMS_INCLUDE_DIRS}
  ${BODY_INCLUDE_DIRS}
  ${CREATURE_INCLUDE_DIRS}
  ${INTERFACES_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(
  Game
  ${GAME_ALL_SOURCES}
)
target_link_libraries(Game PRIVATE ${EXTERNAL_LIBS})
target_include_directories(Game PRIVATE ${GAME_ALL_INCLUDE_DIRS})

target_compile_options(Game PRIVATE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -ggdb3 -fsanitize=address -fsanitize=undefined -fsanitize=leak>
  $<$<CONFIG:Release>:-Wall -Wextra -O3 -g -ggdb3>
)

target_link_options(Game PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined -fsanitize=leak>
)

