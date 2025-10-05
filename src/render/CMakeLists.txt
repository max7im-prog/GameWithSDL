file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
)

add_library(game_render_lib STATIC ${SOURCES})
target_include_directories(game_render_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_render_lib PUBLIC SDL3::SDL3 box2d game_interfaces_lib game_physics_lib)
