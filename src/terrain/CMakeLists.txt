file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/terrain
)

add_library(game_terrain_lib STATIC ${SOURCES})
target_include_directories(game_terrain_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_terrain_lib PUBLIC game_physics_lib game_render_lib game_connection_lib game_misc_lib game_body_lib game_interfaces_lib)
