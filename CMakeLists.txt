cmake_minimum_required(VERSION 3.23.1)

project(console_game)

add_executable(console_game main.cpp)

add_library(console console.cpp)
add_library(ball bouncingball.cpp)
add_library(player player.cpp)
add_library(renderer renderer.cpp)
add_library(sprite sprite.cpp)
add_library(inputhandler inputhandler.cpp)

target_link_libraries(console_game PUBLIC console)
target_link_libraries(console_game PUBLIC ball)
target_link_libraries(console_game PUBLIC player)
target_link_libraries(console_game PUBLIC renderer)
target_link_libraries(console_game PUBLIC sprite)
target_link_libraries(console_game PUBLIC inputhandler)

target_include_directories(
  console_game PUBLIC
    "${PROJECT_BINARY_DIR}"
)
