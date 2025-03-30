FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG 11.0.0
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(tinyxml2)
if(NOT tinyxml2_POPULATED)
    FetchContent_Populate(tinyxml2)
    
    set(BUILD_TESTING OFF)
    set(tinyxml2_BUILD_TESTING OFF)
    set(tinyxml2_SHARED_LIBS OFF)
    
    add_subdirectory(${tinyxml2_SOURCE_DIR} ${tinyxml2_BINARY_DIR})
    
    if(MSVC)
        set_target_properties(tinyxml2 PROPERTIES FOLDER "3rdparty/tinyxml2")
    endif()
endif()
