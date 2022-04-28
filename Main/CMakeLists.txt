cmake_minimum_required(VERSION 3.17)


add_executable(Main main.cpp)
target_link_libraries(Main Optimization)
target_include_directories(Main PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
