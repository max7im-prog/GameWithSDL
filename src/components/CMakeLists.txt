file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)
set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_components_lib INTERFACE ${SOURCES})
target_include_directories(game_components_lib INTERFACE ${INCLUDE_DIRS})
target_link_libraries(game_components_lib 
  INTERFACE 
  box2d 
  EnTT::EnTT 
  game_interfaces_lib 
  game_physics_lib 
  game_controls_lib 
  game_misc_lib
  game_common_lib
)
