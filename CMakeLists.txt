cmake_minimum_required(VERSION 3.16)
include(ExternalProject)
include(FetchContent)
set(UTILPP_TAG bbe5a2400dca91b1b8a93b0230cd99254b038ec2)
set(OCmakeUtil_TAG da88ebcc080f0374b0fb6e9e05abdc26541d4fdb)

project(Renderer LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_BINARY_DIR}/rundir"
        CACHE STRING "Directory to install after building" FORCE)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

FetchContent_Declare(
    OCmakeUtil
    GIT_REPOSITORY https://github.com/wsw364321644/OCmakeUtil.git
    GIT_TAG ${OCmakeUtil_TAG}
)
FetchContent_MakeAvailable(OCmakeUtil)

ImportProject(LIBUV STATIC_CRT SSH TAG be6b81a352d17513c95be153afcb3148f1a451cd) # 1.47.0

set(UTILPP_WITH_STATIC_LIBUV OFF CACHE BOOL "" FORCE)
set(UTILPP_STATIC_CRT OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    utilpp
    GIT_REPOSITORY git@ssh.github.com:wsw364321644/utilpp.git
    GIT_TAG ${UTILPP_TAG}
)
FetchContent_MakeAvailable(utilpp)

set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    sdl
    GIT_REPOSITORY git@ssh.github.com:libsdl-org/SDL.git
    GIT_TAG 859844eae358447be8d66e6da59b6fb3df0ed778
)
FetchContent_MakeAvailable(sdl)

ExternalProject_Add(
    assets
    URL https://github.com/jpvanoosten/LearningDirectX12/releases/download/v1.1.0/Assets.7z
    SOURCE_DIR ${CMAKE_INSTALL_PREFIX}/bin/Assets
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

#FetchContent_Declare(
#    assets
#    URL      
#)
#FetchContent_MakeAvailable(assets)

set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(UUID_USING_CXX20_SPAN ON)
add_subdirectory(ThirdParty/assimp)

set(UUID_BUILD_TESTS OFF)
add_subdirectory(ThirdParty/stduuid)

add_subdirectory(ThirdParty/DirectXTex)
add_subdirectory(ThirdParty/DirectXTK)
add_subdirectory(ThirdParty/gainput)
add_subdirectory(ThirdParty/glm)
add_subdirectory(Source/imgui)
add_subdirectory(Source)