cmake_minimum_required(VERSION 3.17)

if (USE_OPTIMIZATION)
    add_subdirectory(Optimization)
endif()

if (USE_GEOMETRY)
    add_subdirectory(Geometry)
endif()
