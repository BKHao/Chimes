cmake_minimum_required(VERSION 3.17)

include_directories(${EIGEN3_INCLUDE_DIR})

set(Chimes_Optimization_SRC        
        line_search.cpp
        steepest_descent.cpp
        )


# Get static lib
add_library(Optimization STATIC ${Chimes_Optimization_SRC})
set_target_properties(Optimization PROPERTIES VERSION ${VERSION})
set_target_properties(Optimization PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_include_directories(Optimization PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
