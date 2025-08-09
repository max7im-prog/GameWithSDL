file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(INTERFACES_SRC ${SOURCES} PARENT_SCOPE)

set(INTERFACES_INCLUDE_DIRS
    ${INCLUDE_DIRS}
    PARENT_SCOPE
)

add_library(game_interfaces_lib STATIC ${SOURCES})
target_include_directories(game_interfaces_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_interfaces_lib PUBLIC EnTT::EnTT)
