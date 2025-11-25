file(GLOB SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(game_interfaces_lib STATIC ${SOURCES})
target_include_directories(game_interfaces_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(game_interfaces_lib PUBLIC game_misc_lib EnTT::EnTT nlohmann_json::nlohmann_json box2d)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fallbackVisitor)
