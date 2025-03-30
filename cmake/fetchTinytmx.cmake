FetchContent_Declare(
    tinytmx
    GIT_REPOSITORY https://github.com/KaseyJenkins/tinytmx.git
    GIT_TAG ef0d4e1b5945d0c530f85a7b7aecc0665c8f707b #verified master
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(tinytmx)
if(NOT tinytmx_POPULATED)
    FetchContent_Populate(tinytmx)

    add_library(tinytmx STATIC)
    
    target_sources(tinytmx
        PRIVATE
        ${tinytmx_SOURCE_DIR}/lib/src/base64.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxColor.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxDataChunkTile.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxEllipse.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxGrid.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxGroupLayer.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxImage.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxImageLayer.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxMap.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxObject.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxObjectGroup.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxPolyObject.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxProperty.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxPropertySet.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxText.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxTile.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxLayer.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxTileLayer.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxTileOffset.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxTileset.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxTransformations.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxUtil.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxWangColor.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxWangSet.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxWangSetArray.cpp
        ${tinytmx_SOURCE_DIR}/lib/src/tinytmxWangTile.cpp
    )
    
    target_include_directories(tinytmx PUBLIC ${tinytmx_SOURCE_DIR}/lib/include
                                       PRIVATE ${tinyxml2_SOURCE_DIR} ${zstd_SOURCE_DIR}/lib)

    set_target_properties (tinytmx PROPERTIES 
                                   CXX_STANDARD_REQUIRED TRUE
                                   CXX_STANDARD 23)

    target_link_libraries(tinytmx PRIVATE
                                  tinyxml2
                                  zlibstatic
                                  libzstd_static)
    

    if(MSVC)
        set_target_properties(tinytmx PROPERTIES FOLDER "3rdparty/tinytmx")
        target_compile_definitions(tinytmx PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()
