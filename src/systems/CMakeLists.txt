file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/world
    ${CMAKE_CURRENT_SOURCE_DIR}/render
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/events
    ${CMAKE_CURRENT_SOURCE_DIR}/controls
    ${CMAKE_CURRENT_SOURCE_DIR}/creature
)

add_library(game_systems_lib STATIC ${SOURCES})
target_include_directories(game_systems_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_systems_lib PUBLIC
    game_body_lib
    game_creature_lib
    game_terrain_lib
    game_render_lib
    game_events_lib
    game_physics_lib
    game_misc_lib
    game_room_lib
)
