cmake_minimum_required(VERSION 3.15)

project(AsiPluginImGui)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(FetchContent)

FetchContent_Declare(
    kthook
    GIT_REPOSITORY https://github.com/kin4stat/kthook.git
    GIT_TAG 4331456543f408c42122ced22144cfb698031266
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 8c0b41037151d3b3c0f81d8dbcb508bda8b0b6f5
)

FetchContent_MakeAvailable(
    kthook

    imgui
)

file(GLOB_RECURSE PROJECT_SOURCES Source/*.cpp)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_SOURCES}

    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp    
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

if (NOT "${DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Lib/x86)
elseif (NOT "$ENV{DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Lib/x86)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    Source

    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    kthook

    d3d9
    d3dx9
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".asi"
)