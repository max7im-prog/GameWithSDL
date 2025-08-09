file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(DEBUG_SRC ${SOURCES} PARENT_SCOPE)

set(DEBUG_INCLUDE_DIRS
    ${INCLUDE_DIRS}
    PARENT_SCOPE
)

add_library(game_debug_lib STATIC ${SOURCES})
target_include_directories(game_debug_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_debug_lib PUBLIC
    game_physics_lib
    game_interfaces_lib
    game_controls_lib
    game_render_lib
    game_events_lib
)
