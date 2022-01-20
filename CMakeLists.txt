cmake_minimum_required (VERSION 3.16)
include(${CMAKE_CURRENT_SOURCE_DIR}/Cmake/CMakeLists.txt)

set( TARGET_NAME Editor )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

project (Renderer LANGUAGES C CXX)
if( MSVC )
    add_compile_options(/MP)
endif()

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

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Private")

AddSourceFolder(INCLUDE RECURSE PUBLIC "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Private")

AddSourceFolder(INCLUDE RECURSE PRIVATE "${PROJECT_SOURCE_DIR}/Source/Editor/Public")
AddSourceFolder(RECURSE "${PROJECT_SOURCE_DIR}/Source/Editor/Private")


AddSourceFolder(INCLUDE PRIVATE "${PROJECT_SOURCE_DIR}/ThirdParty/imgui")

set(SourceFiles "${SourceFiles};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.h;${Imgui_SRC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.cpp")
list(APPEND PrivateIncludeFolders "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends") 


source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${SourceFiles})



file(GLOB_RECURSE VERTEX_SHADER_FILES LIST_DIRECTORIES false   CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Vertex/*.hlsl")
file(GLOB_RECURSE PIXEL_SHADER_FILES LIST_DIRECTORIES false   CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Pixel/*.hlsl")
file(GLOB_RECURSE COMPUTE_SHADER_FILES LIST_DIRECTORIES false   CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Shaders/Compute/*.hlsl")
set( SHADER_FILES
    ${VERTEX_SHADER_FILES}
    ${PIXEL_SHADER_FILES}
    ${COMPUTE_SHADER_FILES}
)
source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${SHADER_FILES} )

set_source_files_properties( ${SHADER_FILES} 
    PROPERTIES
        VS_SHADER_OBJECT_FILE_NAME "${CMAKE_SOURCE_DIR}/Data/Shaders/%(Filename).cso"
        VS_SHADER_MODEL 6.0
        VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:ON>
        VS_SHADER_ENABLE_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ON>
        VS_SHADER_FLAGS $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-Qembed_debug>
)

set_source_files_properties( ${VERTEX_SHADER_FILES}
    PROPERTIES 
        VS_SHADER_TYPE Vertex
)

set_source_files_properties( ${PIXEL_SHADER_FILES}
    PROPERTIES 
        VS_SHADER_TYPE Pixel
)
set_source_files_properties( ${COMPUTE_SHADER_FILES}
    PROPERTIES
        VS_SHADER_TYPE Compute
)


add_subdirectory( ThirdParty/DirectXTex )
add_subdirectory( ThirdParty/DirectXTK )
add_subdirectory( ThirdParty/spdlog )
add_subdirectory( ThirdParty/assimp )
add_subdirectory( ThirdParty/gainput )
add_subdirectory( ThirdParty/glm)
add_subdirectory( ThirdParty/stduuid)
add_subdirectory( ThirdParty/taskflow)
add_subdirectory( ThirdParty/SDL)


add_executable( ${TARGET_NAME} WIN32
    ${SourceFiles}
)

# Enable C++17 compiler features.
target_compile_features(  ${TARGET_NAME}
    PUBLIC cxx_std_17
)

# Remove those pesky min/max macros defined in global scope.
target_compile_definitions(  ${TARGET_NAME}
    PUBLIC NOMINMAX 
)


foreach(folderpath IN LISTS PublicIncludeFolders)
    target_include_directories( ${TARGET_NAME}
        PUBLIC ${folderpath}
    )
endforeach()
foreach(folderpath IN LISTS PrivateIncludeFolders)
    target_include_directories( ${TARGET_NAME}
        PRIVATE ${folderpath}
    )
endforeach()
foreach(folderpath IN LISTS InterfaceIncludeFolders)
    target_include_directories( ${TARGET_NAME}
        INTERFACE ${folderpath}
    )
endforeach()

target_link_libraries( ${TARGET_NAME}
    DirectXTex
	DirectXTK
	spdlog
	assimp
    gainput
    glm
    stduuid
	Taskflow
	SDL2
    d3d12.lib
    dxgi.lib
    d3dcompiler.lib
    dxgi.lib
    dxguid.lib
)


get_directory_property(ASSIMP_RUNTIME_OUTPUT_DIRECTORY DIRECTORY ThirdParty/assimp DEFINITION ASSIMP_RUNTIME_OUTPUT_DIRECTORY)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory    ${ASSIMP_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>)


set( COMMAND_ARGUMENTS "-wd \"${CMAKE_SOURCE_DIR}\"" )
configure_file( ${TARGET_NAME}.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )

message("${CMAKE_CURRENT_BINARY_DIR}" )