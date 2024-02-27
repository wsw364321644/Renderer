cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME Editor)

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Private")

if(WIN32)
    AddSourceFolder(INCLUDE PRIVATE "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows")
endif()

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Private/")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/InputCore/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/InputCore/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Renderer/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Renderer/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Private")

AddSourceFolder(INCLUDE RECURSE PRIVATE "${PROJECT_SOURCE_DIR}/Source/Editor/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Editor/Private")

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SourceFiles})

file(GLOB_RECURSE VERTEX_SHADER_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Vertex/*.hlsl")
file(GLOB_RECURSE PIXEL_SHADER_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Pixel/*.hlsl")
file(GLOB_RECURSE COMPUTE_SHADER_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Compute/*.hlsl")
set(SHADER_FILES
    ${VERTEX_SHADER_FILES}
    ${PIXEL_SHADER_FILES}
    ${COMPUTE_SHADER_FILES}
)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SHADER_FILES})

set_source_files_properties(${SHADER_FILES}
    PROPERTIES
    VS_SHADER_OBJECT_FILE_NAME "${CMAKE_INSTALL_PREFIX}/bin/Data/Shaders/%(Filename).cso"
    VS_SHADER_MODEL 6.0
    VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:ON>
    VS_SHADER_ENABLE_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ON>
    VS_SHADER_FLAGS $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-Qembed_debug>
)

set_source_files_properties(${VERTEX_SHADER_FILES}
    PROPERTIES
    VS_SHADER_TYPE Vertex
)

set_source_files_properties(${PIXEL_SHADER_FILES}
    PROPERTIES
    VS_SHADER_TYPE Pixel
)
set_source_files_properties(${COMPUTE_SHADER_FILES}
    PROPERTIES
    VS_SHADER_TYPE Compute
)
list (APPEND SourceFiles ${SHADER_FILES})
add_executable(${TARGET_NAME} WIN32
    ${SourceFiles}
)
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

# Enable C++20 compiler features.
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# Remove those pesky min/max macros defined in global scope.
target_compile_definitions(${TARGET_NAME} PUBLIC NOMINMAX)

AddTargetInclude(${TARGET_NAME})

target_link_libraries(${TARGET_NAME} PRIVATE sutils)
target_link_libraries(${TARGET_NAME} PRIVATE DirectXTex)
target_link_libraries(${TARGET_NAME} PRIVATE DirectXTK)
target_link_libraries(${TARGET_NAME} PRIVATE spdlog)
target_link_libraries(${TARGET_NAME} PRIVATE assimp)
target_link_libraries(${TARGET_NAME} PRIVATE gainputstatic)
target_link_libraries(${TARGET_NAME} PRIVATE glm)
target_link_libraries(${TARGET_NAME} PRIVATE stduuid)
target_link_libraries(${TARGET_NAME} PRIVATE SDL2)
target_link_libraries(${TARGET_NAME} PRIVATE imgui_a)
target_link_libraries(${TARGET_NAME} PRIVATE d3d12.lib)
target_link_libraries(${TARGET_NAME} PRIVATE dxgi.lib)
target_link_libraries(${TARGET_NAME} PRIVATE d3dcompiler.lib)
target_link_libraries(${TARGET_NAME} PRIVATE dxgi.lib)
target_link_libraries(${TARGET_NAME} PRIVATE dxguid.lib)

#get_directory_property(ASSIMP_RUNTIME_OUTPUT_DIRECTORY DIRECTORY ${PROJECT_SOURCE_DIR}/ThirdParty/assimp DEFINITION ASSIMP_RUNTIME_OUTPUT_DIRECTORY)

#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSIMP_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>)

#set( COMMAND_ARGUMENTS "-wd \"${PROJECT_SOURCE_DIR}\"" )
#configure_file(${TARGET_NAME}.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY)