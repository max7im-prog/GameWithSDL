cmake_minimum_required(VERSION 3.10.0)
project(GameWithSDL VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(external/box2d-3.1.1)
add_subdirectory(external/SDL-release-3.2.16)
add_subdirectory(external/entt-3.15.0)

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

    src/kinematics/kinematicUtils.cpp

    src/creature/baseCreature.cpp
    src/creature/humanoid.cpp
    src/creature/creatureUpdateSystem.cpp
    src/creature/bodyParts/bodyPart.cpp
    src/creature/bodyParts/capsuleBodyPart.cpp
    src/creature/bodyParts/polygonBodyPart.cpp
    src/creature/bodyParts/circleBodyPart.cpp
    src/creature/bodyParts/limbBodyPart.cpp

    src/controls/controllerUpdateSystem.cpp
    src/controls/creatureControlSystem.cpp

    src/debug/debugUtils.cpp

    src/misc/cleanupSystem.cpp
)

target_link_libraries(Game PRIVATE box2d SDL3-shared EnTT::EnTT)

target_include_directories(Game PRIVATE
    external/box2d-3.1.1/include
    external/SDL-release-3.2.16/include
    external/entt-3.15.0/single_include
)

target_include_directories(Game PRIVATE
    src
    src/render
    src/events
    src/physics
    src/kinematics
    src/misc
    src/debug
    src/creature
    src/creature/bodyParts
    src/controls
)

message(STATUS "Include directories: ${CMAKE_CXX_INCLUDE_DIRECTORIES}")