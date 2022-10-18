option(ENABLE_CACHE "Enable cache through ccache if available" ON)

if (ENABLE_CACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        message(STATUS "Found ccache")
        set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    endif()
endif()