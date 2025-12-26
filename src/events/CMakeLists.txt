file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_events_lib STATIC ${SOURCES})
target_include_directories(game_events_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_events_lib PRIVATE SDL3::SDL3 EnTT::EnTT game_components_lib)
