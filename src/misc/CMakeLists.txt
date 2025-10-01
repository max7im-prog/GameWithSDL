file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_misc_lib INTERFACE)
target_include_directories(game_misc_lib INTERFACE ${INCLUDE_DIRS})

