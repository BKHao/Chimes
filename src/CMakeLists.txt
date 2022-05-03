cmake_minimum_required(VERSION 3.17)

add_subdirectory(Core)

if (USE_OPTIMIZATION)
    add_subdirectory(Optimization)
endif()
