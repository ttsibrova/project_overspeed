FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG v1.3.1
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(zlib)
if(NOT zlib_POPULATED)
    set(ZLIB_BUILD_EXAMPLES OFF)
    
    FetchContent_MakeAvailable(zlib)

    if(MSVC)
        set_target_properties(zlibstatic PROPERTIES FOLDER "3rdparty/zlib")
        set_target_properties(zlib PROPERTIES FOLDER "3rdparty/zlib")
    endif()
endif()
