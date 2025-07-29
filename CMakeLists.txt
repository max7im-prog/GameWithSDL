cmake_minimum_required(VERSION 3.10.0)
project(
  GameWithSDL
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address -fsanitize=undefined -g -ggdb3 -Wall -Wextra"
  )
  message(
    STATUS "Selected ${CMAKE_BUILD_TYPE} build type. Building with sanitizers.")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -ggdb3 -Wall -Wextra")
endif()

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

add_executable(
  Game
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
  src/controls/kinematics/kinematicUtils.cpp
  src/controls/PID/PIDScalarController.cpp
  src/controls/PID/PIDVectorController.cpp
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
  src/misc/cleanupSystem.cpp)

target_link_libraries(Game PRIVATE box2d SDL3-shared EnTT::EnTT)

target_include_directories(
  Game PRIVATE external/box2d-3.1.1/include external/SDL-release-3.2.16/include
  external/entt-3.15.0/single_include)

target_include_directories(
  Game
  PRIVATE src
  src/render
  src/events
  src/physics
  src/misc
  src/debug
  src/creature
  src/creature/bodyParts
  src/controls
  src/controls/kinematics
  src/controls/PID)

message(STATUS "Include directories: ${CMAKE_CXX_INCLUDE_DIRECTORIES}")
