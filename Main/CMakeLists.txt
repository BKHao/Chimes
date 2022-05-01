cmake_minimum_required(VERSION 3.17)

if (USE_OPTIMIZATION)
	include_directories(${EIGEN3_INCLUDE_DIR})
endif()

add_executable(Main main.cpp)
target_link_libraries(Main ${CHIMES_LIBS})
target_include_directories(Main PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
