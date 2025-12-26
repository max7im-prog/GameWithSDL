file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp"
)

set(INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_common_lib STATIC ${SOURCES})
target_include_directories(game_common_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_common_lib
  PRIVATE
  box2d
  EnTT::EnTT
  SDL3::SDL3
  SDL3_image::SDL3_image
)
