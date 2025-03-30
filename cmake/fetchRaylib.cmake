FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(raylib)
if(NOT raylib_POPULATED)
    FetchContent_Populate(raylib)

    set(BUILD_EXAMPLES OFF)
    
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    
    if(MSVC)
        set_target_properties(raylib PROPERTIES FOLDER "3rdparty/raylib")
        if(TARGET glfw)
            set_target_properties(glfw PROPERTIES FOLDER "3rdparty/raylib/external")
            set_target_properties(update_mappings PROPERTIES FOLDER "3rdparty/raylib/external")
        endif()
    endif()
endif()
