cmake_minimum_required(VERSION 3.15)

project(AsiPlugin)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(PSDK_EXAMPLES OFF)
set(PSDK_INSTALL OFF)
set(PSDK_GTASA ON)

include(FetchContent)

FetchContent_Declare(
    kthook
    GIT_REPOSITORY https://github.com/kin4stat/kthook.git
    GIT_TAG 4331456543f408c42122ced22144cfb698031266
)

FetchContent_Declare(
    rakhook
    GIT_REPOSITORY https://github.com/aIcantara/RakHook.git
    GIT_TAG d69970eb8daea4dface3d3b316e86ed2050fb47e
)

FetchContent_Declare(
    sampapi
    GIT_REPOSITORY https://github.com/BlastHackNet/SAMP-API.git
    GIT_TAG 6d4db99ab41f19d1a6a7c6cd48f5878bd1e14b62
)

FetchContent_Declare(
    plugin-sdk
    GIT_REPOSITORY https://github.com/imring/plugin-sdk.git
    GIT_TAG 81b18b744b64db3058f48344c144591c9030205b
)

FetchContent_MakeAvailable(
    kthook

    rakhook
    sampapi
    plugin-sdk
)

file(GLOB_RECURSE PROJECT_SOURCES Source/*.cpp)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE Source)

target_link_libraries(${PROJECT_NAME} PUBLIC
    kthook

    rakhook
    sampapi
    pluginsdk::gtasa
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".asi"
)