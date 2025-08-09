file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/creature
)

set(CREATURE_SRC ${SOURCES} PARENT_SCOPE)

set(CREATURE_INCLUDE_DIRS
    ${INCLUDE_DIRS}
    PARENT_SCOPE
)

add_library(game_creature_lib STATIC ${SOURCES})
target_include_directories(game_creature_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_creature_lib PUBLIC game_physics_lib game_body_lib game_interfaces_lib game_controls_lib)
