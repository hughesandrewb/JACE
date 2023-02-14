cmake_minimum_required(VERSION 2.8)
project(jace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

include_directories(../..)

add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(arch)

set(jace_source_files main.cc)
add_executable(jace ${jace_source_files})

target_link_libraries(jace game)
