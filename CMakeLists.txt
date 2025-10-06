cmake_minimum_required(VERSION 3.10)
project(EpiGIMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(src src/core src/tools src/ui)

add_executable(EpiGIMP ${SOURCES})

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(EpiGIMP sfml-graphics sfml-window sfml-system)
