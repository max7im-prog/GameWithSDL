file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/creature
)

add_library(game_creature_lib STATIC ${SOURCES})
target_include_directories(game_creature_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_creature_lib
  PRIVATE
  game_scene_node_lib
  game_physics_lib
  game_connection_lib
  game_body_lib
  game_interfaces_lib
  game_controls_lib
  game_misc_lib
  game_scene_node_lib
  game_components_lib
  game_common_lib
  box2d
  EnTT::EnTT
  nlohmann_json::nlohmann_json
)
