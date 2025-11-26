file(GLOB SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_scene_node_lib STATIC ${SOURCES})
target_include_directories(game_scene_node_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_scene_node_lib
  PUBLIC
  game_interfaces_lib
  box2d
  game_body_lib
  game_connection_lib
  game_misc_lib
  game_common_lib
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/creature)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/terrain)
