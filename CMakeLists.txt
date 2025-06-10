cmake_minimum_required(VERSION 3.10.0)
project(GameWithSDL VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(external/box2d)
add_subdirectory(external/SDL)
add_subdirectory(external/entt)

add_executable(Game
    src/main.cpp
    src/game.cpp
    src/game.hpp
    src/renderContext.hpp
)

target_link_libraries(Game PRIVATE box2d SDL3-shared EnTT::EnTT)

target_include_directories(Game PRIVATE
    external/box2d/include
    external/SDL/include
    external/entt/single_include
)

message(STATUS "Include directories: ${CMAKE_CXX_INCLUDE_DIRECTORIES}")