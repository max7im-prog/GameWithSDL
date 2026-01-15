file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/world)

add_library(game_world_lib STATIC ${SOURCES})
target_include_directories(game_world_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(
  game_world_lib PUBLIC box2d EnTT::EnTT SDL3::SDL3 SDL3_image::SDL3_image
                        game_interfaces_lib game_components_lib)
