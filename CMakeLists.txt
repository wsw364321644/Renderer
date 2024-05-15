cmake_minimum_required(VERSION 3.16)

include(FetchContent)

set(UTILPP_TAG 2e9b0b99b94cc1fc32427b4ab2c3a1fe58d59df3)
set(OCmakeUtil_TAG 6df33d075d033b9ce341e54176154f250290ceff)
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
    GIT_REPOSITORY git@ssh.github.com:wsw364321644/OCmakeUtil.git
    GIT_TAG ${OCmakeUtil_TAG}
)
FetchContent_MakeAvailable(OCmakeUtil)

ImportProject(LIBUV STATIC_CRT SSH TAG be6b81a352d17513c95be153afcb3148f1a451cd) # 1.47.0
ImportProject(SDL2 STATIC_CRT SSH TAG f461d91cd265d7b9a44b4d472b1df0c0ad2855a0) # 2.30.2
ImportTarget(imgui SSH TAG 6ccc561a2ab497ad4ae6ee1dbd3b992ffada35cb) # v1.90.6

set(UTILPP_WITH_STATIC_LIBUV OFF CACHE BOOL "" FORCE)
set(UTILPP_STATIC_CRT OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    utilpp
    GIT_REPOSITORY git@ssh.github.com:wsw364321644/utilpp.git
    GIT_TAG ${UTILPP_TAG}
)
FetchContent_MakeAvailable(utilpp)

FetchContent_Declare(
  sigslot
  GIT_REPOSITORY https://github.com/palacaze/sigslot
  GIT_TAG        33802bb3e94c4dfe07bd41b537b36806f94c3e3a # v1.2.2
)
FetchContent_MakeAvailable(sigslot)

ExternalProject_Add(
    assets
    URL https://github.com/jpvanoosten/LearningDirectX12/releases/download/v1.1.0/Assets.7z
    SOURCE_DIR ${CMAKE_INSTALL_PREFIX}/bin/Assets
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

# FetchContent_Declare(
# assets
# URL
# )
# FetchContent_MakeAvailable(assets)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(ThirdParty/assimp)

set(UUID_BUILD_TESTS OFF)
set(UUID_USING_CXX20_SPAN ON)
add_subdirectory(ThirdParty/stduuid)

add_subdirectory(ThirdParty/DirectXTex)
add_subdirectory(ThirdParty/DirectXTK)
add_subdirectory(ThirdParty/gainput)
add_subdirectory(ThirdParty/glm)
add_subdirectory(Source)