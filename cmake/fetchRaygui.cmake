FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 4.0
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
    FetchContent_MakeAvailable(raygui)
    
    add_library(raygui INTERFACE)

    target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)
    target_link_libraries(raygui INTERFACE raylib)
    
    if(MSVC)
        set_target_properties(raygui PROPERTIES FOLDER "3rdparty/raygui")
    endif()
endif()