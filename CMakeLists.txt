cmake_minimum_required(VERSION 4.0.0)
project(
  GameWithSDL
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


# External libraries

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/max7im-prog/game-box2d.git
  GIT_TAG v3.1.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(box2d)
FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/max7im-prog/game-SDL.git
  GIT_TAG release-3.2.16
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL)
FetchContent_Declare(
  SDL_image
  GIT_REPOSITORY https://github.com/max7im-prog/game-SDL-image.git
  GIT_TAG release-3.2.x
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL_image)
FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/max7im-prog/game-entt.git
  GIT_TAG v3.15.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(entt)
FetchContent_Declare(
  nlohmann-json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann-json)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.x
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

# Add subdirectories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/physics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/controls)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/body)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sceneNode)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/misc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/render)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/debug)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/systems)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/connection)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/room)

# Game libraries
set(EXTERNAL_LIBS
  box2d
  SDL3::SDL3
  SDL3_image::SDL3_image
  EnTT::EnTT
  nlohmann_json::nlohmann_json
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
  game_terrain_lib
  game_connection_lib
  game_room_lib
  game_common_lib
)
add_library(game_run_lib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
)
target_include_directories(game_run_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(game_run_lib PUBLIC ${GAME_LIBS} ${EXTERNAL_LIBS})

# Create executable
add_executable(
  Game
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
target_link_libraries(Game PRIVATE game_run_lib)

# Compiler options
target_compile_options(Game PRIVATE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -ggdb3 -fsanitize=address -fsanitize=undefined -fsanitize=leak>
  $<$<CONFIG:Release>:-Wall -Wextra -O3 -g -ggdb3>
)
target_link_options(Game PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined -fsanitize=leak>
)

# Copy resources into build
add_custom_target(copy_resources)
add_custom_command(TARGET copy_resources POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  "${CMAKE_CURRENT_SOURCE_DIR}/res"
  "$<TARGET_FILE_DIR:Game>/res"
)
add_dependencies(Game copy_resources)

# Testing
include(CTest)
include(GoogleTest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

