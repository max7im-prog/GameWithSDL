file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/models
                 ${CMAKE_CURRENT_SOURCE_DIR}/models/human)

add_library(game_behavior_lib STATIC ${SOURCES})
target_include_directories(game_behavior_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_behavior_lib PUBLIC game_body_lib box2d EnTT::EnTT)
