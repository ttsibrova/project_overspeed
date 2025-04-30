FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG 11.0.0
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(tinyxml2)
if(NOT tinyxml2_POPULATED)

    set(BUILD_TESTING OFF)
    set(tinyxml2_BUILD_TESTING OFF)
    set(tinyxml2_SHARED_LIBS OFF)
    
    FetchContent_MakeAvailable(tinyxml2)

    if(MSVC)
        set_target_properties(tinyxml2 PROPERTIES FOLDER "3rdparty/tinyxml2")
    endif()
endif()
