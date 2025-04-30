FetchContent_Declare(
    zstd
    GIT_REPOSITORY https://github.com/facebook/zstd.git
    GIT_TAG v1.5.7
    GIT_PROGRESS TRUE
    SOURCE_SUBDIR "./build/cmake"
)

FetchContent_GetProperties(zstd)
if(NOT zstd_POPULATED)

    # zstd specific options
    set(ZSTD_BUILD_PROGRAMS OFF)
    set(ZSTD_BUILD_TESTS OFF)
    set(ZSTD_BUILD_SHARED OFF)
    set(ZSTD_BUILD_STATIC ON)

    FetchContent_MakeAvailable(zstd)

    if(MSVC)
        set_target_properties(libzstd_static PROPERTIES FOLDER "3rdparty/zstd")
        if(TARGET uninstall)
            set_target_properties(uninstall PROPERTIES FOLDER "3rdparty/zstd")
        endif()
        get_directory_property(ZSTD_TARGETS DIRECTORY ${zstd_SOURCE_DIR}/build/cmake BUILDSYSTEM_TARGETS)
        foreach(TARGET ${ZSTD_TARGETS})
             set_target_properties(${TARGET} PROPERTIES FOLDER "3rdparty/zstd")
        endforeach()
    endif()
endif()
