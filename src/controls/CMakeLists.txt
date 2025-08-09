file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/kinematics
    ${CMAKE_CURRENT_SOURCE_DIR}/PID
)

set(CONTROLS_SRC ${SOURCES} PARENT_SCOPE)

set(CONTROLS_INCLUDE_DIRS
    ${INCLUDE_DIRS}
    PARENT_SCOPE
)

add_library(game_controls_lib STATIC ${SOURCES})
target_include_directories(game_controls_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_controls_lib PUBLIC box2d )
