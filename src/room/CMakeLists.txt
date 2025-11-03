file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_room_lib STATIC ${SOURCES})
target_include_directories(game_room_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_room_lib PUBLIC EnTT::EnTT game_misc_lib game_terrain_lib game_creature_lib game_interfaces_lib nlohmann_json::nlohmann_json box2d game_physics_lib)
