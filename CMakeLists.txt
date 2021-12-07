cmake_minimum_required (VERSION 3.16)

set( TARGET_NAME Editor )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

project (Renderer LANGUAGES C CXX)
if( MSVC )
    add_compile_options(/MP)
endif()

file(GLOB_RECURSE CORE_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Public/*.hpp")
file(GLOB_RECURSE CORE_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${CORE_INC} ${CORE_SRC} )


file(GLOB_RECURSE Engine_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public/*.hpp")
file(GLOB_RECURSE Engine_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Private/*.cpp")
source_group(  TREE ${PROJECT_SOURCE_DIR} FILES ${Engine_INC}  ${Engine_SRC} )


file(GLOB Launch_INC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/*.hpp")
file(GLOB Launch_SRC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Private/*.cpp")

if(WIN32)
	file(GLOB_RECURSE Launch_INC_PLAT LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows/*.hpp")  
    set(Launch_INC "${Launch_INC};${Launch_INC_PLAT}")
	file(GLOB_RECURSE Launch_SRC_PLAT LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Private/Windows/*.cpp")
    set(Launch_SRC "${Launch_SRC};${Launch_SRC_PLAT}")
    file(GLOB_RECURSE Launch_RES_PLAT LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.rc" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.ico" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.h")
    source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${Launch_RES_PLAT} )
endif()
source_group( TREE ${PROJECT_SOURCE_DIR} FILES ${Launch_INC} ${Launch_SRC}  )




file(GLOB_RECURSE ApplicationCore_INC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public/GenericPlatform/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public/GenericPlatform/*.hpp")
file(GLOB_RECURSE ApplicationCore_SRC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Private/GenericPlatform/*.cpp")
if(WIN32)
	file(GLOB_RECURSE ApplicationCore_INC_PLAT LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public/Windows/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public/Windows/*.hpp")  
    set(ApplicationCore_INC "${ApplicationCore_INC};${ApplicationCore_INC_PLAT}")
	file(GLOB_RECURSE ApplicationCore_SRC_PLAT LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Private/Windows/*.cpp")
    set(ApplicationCore_SRC "${ApplicationCore_SRC};${ApplicationCore_SRC_PLAT}")
endif()
source_group( TREE ${PROJECT_SOURCE_DIR} FILES ${ApplicationCore_INC} ${ApplicationCore_SRC}  )

file(GLOB_RECURSE Slate_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Public/*.hpp")
file(GLOB_RECURSE Slate_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${Slate_INC} ${Slate_SRC} )

file(GLOB_RECURSE D3D12RHI_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public/*.hpp")
file(GLOB_RECURSE D3D12RHI_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${D3D12RHI_INC} ${D3D12RHI_SRC} )


file(GLOB_RECURSE Editor_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Editor/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Editor/Public/*.hpp")
file(GLOB_RECURSE Editor_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Editor/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${Editor_INC} ${Editor_SRC} )


file(GLOB  Imgui_INC LIST_DIRECTORIES false    CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.h" "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.hpp" )
file(GLOB  Imgui_SRC LIST_DIRECTORIES false   CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.cpp" )

if(WIN32)
	set(Imgui_INC "${Imgui_INC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.h")
    set(Imgui_SRC "${Imgui_SRC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.cpp")
endif()
source_group(TREE ${PROJECT_SOURCE_DIR}  FILES ${Imgui_INC} ${Imgui_SRC} )



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

    ${CORE_INC} 
    ${CORE_SRC} 
    ${Launch_INC} 
    ${Launch_SRC}
    ${ApplicationCore_INC} 
    ${ApplicationCore_SRC}
    ${Engine_INC}
    ${Engine_SRC}
    ${Slate_INC}
    ${Slate_SRC}
    ${D3D12RHI_INC}
    ${D3D12RHI_SRC}
    ${Editor_INC}
    ${Editor_SRC}
    ${SHADER_FILES}
    ${Imgui_INC}
    ${Imgui_SRC}
)


# Enable C++17 compiler features.
target_compile_features(  ${TARGET_NAME}
    PUBLIC cxx_std_17
)

# Remove those pesky min/max macros defined in global scope.
target_compile_definitions(  ${TARGET_NAME}
    PUBLIC NOMINMAX 
)

target_include_directories( ${TARGET_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Core/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Slate/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Editor/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/imgui
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/gainput/lib/include
)
if(WIN32)
target_include_directories( ${TARGET_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/ApplicationCore/Public/Windows
)
endif()

target_link_libraries( ${TARGET_NAME}
    DirectXTex
	DirectXTK
	spdlog
	assimp
    gainput
    glm
    stduuid
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