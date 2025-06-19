cmake_minimum_required(VERSION 3.10.0)
project(GameWithSDL VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(external/box2d)
add_subdirectory(external/SDL)
add_subdirectory(external/entt)

add_executable(Game
    src/main.cpp
    src/game.cpp
    src/game.hpp

    src/render/renderContext.hpp
    src/render/renderComponents.hpp
    src/render/renderBackgroundSystem.cpp
    src/render/renderPhysicsBodiesSystem.cpp
    src/render/renderPhysicsJointsSystem.cpp
    src/render/renderUtils.cpp

    src/events/pollEventSystem.cpp
    src/events/quitSystem.cpp

    src/physics/physicsComponents.hpp
    src/physics/worldUpdateSystem.cpp
    src/physics/physicsBodyCreationSystem.cpp
    src/physics/mouseJointSystem.cpp
    src/physics/physicsUtils.cpp

    src/debug/debugUtils.cpp

    src/misc/cleanupSystem.cpp
)

target_link_libraries(Game PRIVATE box2d SDL3-shared EnTT::EnTT)

target_include_directories(Game PRIVATE
    external/box2d/include
    external/SDL/include
    external/entt/single_include
)

target_include_directories(Game PRIVATE
    src
    src/render
    src/events
    src/physics
    src/misc
    src/debug
)

message(STATUS "Include directories: ${CMAKE_CXX_INCLUDE_DIRECTORIES}")