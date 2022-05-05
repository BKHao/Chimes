cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_CMAKE_PATH})
project(Chimes)
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

# Default Build_type RELEASE
set(CMAKE_BUILD_TYPE RELEASE)
# CXX STANDARD
set(CMAKE_CXX_STANDARD 20)
# Set Version
set(VERSION 1.0)
# Set lib output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


# include header from project_source_dir
include_directories(${PROJECT_SOURCE_DIR}/include)

option(USE_OPTIMIZATION "use the optimization lib" ON)
option(USE_GEOMETRY "use the geometry lib" ON)

set(CHIMES_LIBS Core)

if (USE_OPTIMIZATION)
  list(APPEND CHIMES_LIBS Optimization)
  find_package(Eigen3 REQUIRED)
  if (Eigen3_FOUND)
    message(STATUS "find Eigen3: ${EIGEN3_VERSION_STRING}")
  else ()
    message(SEND_ERROR "Can't find Eigen3.")
  endif ()
endif ()

if (USE_OPTIMIZATION)
  list(APPEND CHIMES_LIBS Geometry)
endif ()

if(WIN32 AND NOT MINGW)
  if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
  endif()
  if(NOT DEFINED CMAKE_RELWITHDEBINFO_POSTFIX)
    set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
  endif()
  if(NOT DEFINED CMAKE_MINSIZEREL_POSTFIX)
    set(CMAKE_MINSIZEREL_POSTFIX "s")
  endif()
endif()



add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(Main)


install(TARGETS ${CHIMES_LIBS}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include
	)	
install(DIRECTORY ./include/ DESTINATION include
        FILES_MATCHING PATTERN "*.h")
install(EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}-config.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION lib/cmake/${PROJECT_NAME}
	)

