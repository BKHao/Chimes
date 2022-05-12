cmake_minimum_required(VERSION 3.17)

set(Chimes_HEADERS
        Core/numerical.h
        Core/template_concept.h
        Core/iterator_base.h
        Core/MemoryPool/memory_common.h
        Core/MemoryPool/page_cache.h
        Core/MemoryPool/central_cache.h
        Core/MemoryPool/thread_cache.h
        Core/MemoryPool/memory_alloc.h
)

if (USE_OPTIMIZATION)
        list(APPEND Chimes_HEADERS 
                Optimization/line_search.h 
                ptimization/steepest_descent.h
        )
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
