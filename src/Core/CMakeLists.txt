cmake_minimum_required(VERSION 3.17)

set(Chimes_Core_SRC        
        numerical.cpp
        MemoryPool/memory_common.cpp
        MemoryPool/page_cache.cpp
        MemoryPool/central_cache.cpp
        MemoryPool/thread_cache.cpp        
        )


# Get static lib
add_library(Core STATIC ${Chimes_Core_SRC})
set_target_properties(Core PROPERTIES VERSION ${VERSION})
set_target_properties(Core PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_include_directories(Core PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)
