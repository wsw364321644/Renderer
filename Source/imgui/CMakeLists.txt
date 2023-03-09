
NewTargetSource()

set(SourceFiles ${SourceFiles} ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui.h
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui.cpp
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imconfig.h
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_draw.cpp
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_internal.h
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_widgets.cpp
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_tables.cpp
${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_demo.cpp
)
set(SourceFiles "${SourceFiles};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.h;${Imgui_SRC};${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends/imgui_impl_win32.cpp")
        
add_library (imgui_a STATIC ${SourceFiles})
TARGET_INCLUDE_DIRECTORIES(imgui_a PUBLIC "${PROJECT_SOURCE_DIR}/ThirdParty/imgui/backends;${PROJECT_SOURCE_DIR}/ThirdParty/imgui")