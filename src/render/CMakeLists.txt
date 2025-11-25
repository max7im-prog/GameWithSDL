file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/renderer
  ${CMAKE_CURRENT_SOURCE_DIR}/textures
)

add_library(game_render_lib STATIC ${SOURCES})
target_include_directories(game_render_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_render_lib
  PUBLIC
  SDL3::SDL3
  SDL3_image::SDL3_image
  box2d
  game_interfaces_lib
  game_physics_lib
  game_body_lib
  game_connection_lib
  game_creature_lib
  game_terrain_lib
  game_fallback_visitor_lib
)
