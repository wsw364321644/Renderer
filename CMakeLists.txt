cmake_minimum_required (VERSION 3.16)

set( TARGET_NAME Editor )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

project (Renderer LANGUAGES CXX)
if( MSVC )
    add_compile_options(/MP)
endif()

file(GLOB_RECURSE Engine_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public/*.hpp")
source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${Engine_INC} )
file(GLOB_RECURSE Engine_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Private/*.cpp")
source_group(  TREE ${PROJECT_SOURCE_DIR} FILES ${Engine_SRC} )


if(WIN32)
	file(GLOB_RECURSE Launch_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows/*.hpp")
    source_group( TREE ${PROJECT_SOURCE_DIR} FILES ${Launch_INC} )
	file(GLOB_RECURSE Launch_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Private/Windows/*.cpp")
    source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${Launch_SRC} )
    file(GLOB_RECURSE Launch_RES LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.rc" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.ico" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/*.h")
    source_group( TREE ${PROJECT_SOURCE_DIR}  FILES ${Launch_RES} )
else()
	file(GLOB_RECURSE Launch_INC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/*.hpp")
	file(GLOB_RECURSE Launch_SRC LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Private/*.cpp")
endif()


file(GLOB_RECURSE D3D12RHI_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public/*.hpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${D3D12RHI_INC} )
file(GLOB_RECURSE D3D12RHI_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${D3D12RHI_SRC} )

file(GLOB_RECURSE Editor_INC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Editor/Public/*.h" "${PROJECT_SOURCE_DIR}/Source/Editor/Public/*.hpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${Editor_INC} )
file(GLOB_RECURSE Editor_SRC LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Source/Editor/Private/*.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${Editor_SRC} )


file(GLOB  Imgui_INC LIST_DIRECTORIES false    CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.h" "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.hpp" )
set(Imgui_INC "${Imgui_INC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.h")
file(GLOB  Imgui_SRC LIST_DIRECTORIES false   CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/*.cpp" )
set(Imgui_INC "${Imgui_SRC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.cpp")
source_group(TREE ${PROJECT_SOURCE_DIR}  FILES ${Imgui_INC} ${Imgui_SRC} ${Editor_SRC}  )



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
#add_subdirectory( ThirdParty/rttr )
add_subdirectory( ThirdParty/gainput )


add_executable( ${TARGET_NAME} WIN32
    ${Launch_INC} 
    ${Launch_SRC}
	${Engine_INC} 
    ${Engine_SRC}
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
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Engine/Public
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/D3D12RHI/Public
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Editor/Public
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/imgui
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/gainput/lib/include
)
if(WIN32)
target_include_directories( ${TARGET_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Public/Windows
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Runtime/Launch/Resources/Windows/
)
endif()

target_link_libraries( ${TARGET_NAME}
    DirectXTex
	DirectXTK
	spdlog
	assimp
    gainput
    d3d12.lib
    dxgi.lib
    d3dcompiler.lib
    dxgi.lib
    dxguid.lib
)


set( COMMAND_ARGUMENTS "-wd \"${CMAKE_SOURCE_DIR}\"" )
configure_file( ${TARGET_NAME}.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )

message("${CMAKE_CURRENT_BINARY_DIR}" )