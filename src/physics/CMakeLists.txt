file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/joint
    ${CMAKE_CURRENT_SOURCE_DIR}/shape
    ${CMAKE_CURRENT_SOURCE_DIR}/world
)

add_library(game_physics_lib STATIC ${SOURCES})
target_include_directories(game_physics_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_physics_lib 
    PUBLIC 
    box2d 
    EnTT::EnTT
    nlohmann_json::nlohmann_json
    game_interfaces_lib 
    game_components_lib
)
