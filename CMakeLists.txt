cmake_minimum_required(VERSION 3.17)
project(Chimes)
set(CMAKE_CXX_STANDARD 20)

add_executable(Main
    src/main.cpp
)

target_include_directories(Main
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)
