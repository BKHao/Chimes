cmake_minimum_required(VERSION 3.17)

set(Chimes_HEADERS
        Core/numerical.h
        Core/numerical.h
)

if (USE_OPTIMIZATION)
        list(APPEND Chimes_HEADERS Optimization/line_search.h Optimization/steepest_descent.h)
endif()

if (USE_GEOMETRY)
        list(APPEND Chimes_HEADERS 
                Geometry/geometry_object.h
                Geometry/point.h
                Geometry/polygon.h
                Geometry/triangle.h
                Geometry/surface.h
        )
endif()
