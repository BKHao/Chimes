cmake_minimum_required(VERSION 3.17)

set(Chimes_Geometry_SRC
        geometry_object.cpp
        point.cpp
        polygon.cpp
        triangle.cpp
        segment.cpp
        surface.cpp
        )


# Get static lib
add_library(Geometry STATIC ${Chimes_Geometry_SRC})
set_target_properties(Geometry PROPERTIES VERSION ${VERSION})
set_target_properties(Geometry PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_include_directories(Geometry PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
