cmake_minimum_required(VERSION 3.17)

set(Chimes_HEADERS
        Core/numerical.h
        Core/template_concept.h
)

if (USE_OPTIMIZATION)
        list(APPEND Chimes_HEADERS 
                Optimization/line_search.h 
                Optimization/steepest_descent.h
                Optimization/lbfgs.h
        )
endif()
