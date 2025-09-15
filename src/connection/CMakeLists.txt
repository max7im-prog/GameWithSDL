file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/connection
)

add_library(game_connection_lib STATIC ${SOURCES})
target_include_directories(game_connection_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_connection_lib PUBLIC box2d EnTT::EnTT game_interfaces_lib game_physics_lib game_controls_lib)
