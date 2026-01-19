file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_library(game_structure_lib STATIC ${SOURCES})
target_include_directories(game_structure_lib PUBLIC ${INCLUDE_DIRS})
target_link_libraries(
  game_structure_lib
  PUBLIC game_interfaces_lib
         box2d
         game_body_lib
         game_connection_lib
         game_misc_lib
         game_common_lib
         EnTT::EnTT
         spdlog::spdlog
         nlohmann_json::nlohmann_json)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/body)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/connection)
