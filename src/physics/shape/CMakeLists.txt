file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shape)

add_library(game_shape_lib STATIC ${SOURCES})
target_include_directories(game_shape_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(
  game_shape_lib
  PUBLIC game_physics_lib
         game_body_lib
         game_interfaces_lib
         game_controls_lib
         game_misc_lib
         game_components_lib
         game_common_lib
         box2d
         EnTT::EnTT
         nlohmann_json::nlohmann_json)
