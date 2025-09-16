file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_events_lib INTERFACE)
target_include_directories(game_events_lib INTERFACE ${INCLUDE_DIRS})
target_link_libraries(game_events_lib INTERFACE SDL3::SDL3)
