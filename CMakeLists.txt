cmake_minimum_required(VERSION 3.10)
project(MiniGIMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(src src/core src/tools)

add_executable(MiniGIMP ${SOURCES})

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(MiniGIMP sfml-graphics sfml-window sfml-system)
