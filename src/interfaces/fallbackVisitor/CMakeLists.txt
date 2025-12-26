file(GLOB SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_fallback_visitor_lib STATIC ${SOURCES})
target_include_directories(game_fallback_visitor_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_fallback_visitor_lib
  PRIVATE
  game_misc_lib
  EnTT::EnTT
  nlohmann_json::nlohmann_json
  game_interfaces_lib
  game_physics_lib
  game_creature_lib
  game_terrain_lib
  game_connection_lib
  game_body_lib
  game_scene_node_lib
  game_components_lib
  game_controls_lib
  game_common_lib
  box2d
)
