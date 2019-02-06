cmake_minimum_required(VERSION 3.1)

project(tictactoe LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Readline REQUIRED)
add_subdirectory(third_party)
add_subdirectory(src)
