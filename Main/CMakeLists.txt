cmake_minimum_required(VERSION 3.17)

set(LINK_CHIMES_LIB)
if (USE_OPTIMIZATION)
	include_directories(${EIGEN3_INCLUDE_DIR})
	list(APPEND LINK_CHIMES_LIB Optimization)
endif()

add_executable(Main main.cpp)
target_link_libraries(Main ${LINK_CHIMES_LIB})
target_include_directories(Main PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
