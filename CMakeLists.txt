cmake_minimum_required(VERSION 3.20)
project(factorio-oss)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#vulkan
find_package(Vulkan REQUIRED)

#GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(external
                    external/bgfx/bgfx/include
                    external/bgfx/bx/include)
include_directories(${GLFW_INCLUDE_DIRS})

include_directories(external/sol2/include
                    src)
find_package(Lua REQUIRED)
set(BGFX_WITH_WAYLAND ON CACHE INTERNAL "" FORCE)
set(BX_CONFIG_DEBUG ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
add_subdirectory(external/bgfx)

set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_OUT_DIR ${CMAKE_BINARY_DIR}/assets)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SRC_DIR} ${ASSETS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${ASSETS_OUT_DIR}/shaders/src
    COMMENT "Copying assets to build directory"
)

# Shaders
set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/assets/shaders/src)
set(SHADER_OUT_DIR ${ASSETS_OUT_DIR}/shaders)
set(BGFX_SHADER_INCLUDE ${CMAKE_SOURCE_DIR}/external/bgfx/bgfx/src)
file(MAKE_DIRECTORY ${SHADER_OUT_DIR})

add_custom_command(
    OUTPUT ${SHADER_OUT_DIR}/simple.vb
    COMMAND shaderc -f ${SHADER_SRC_DIR}/vs_simple.sc -o ${SHADER_OUT_DIR}/simple.vb
            --type vertex --platform linux -p spirv
            -i ${BGFX_SHADER_INCLUDE} --varyingdef ${SHADER_SRC_DIR}/varying.def.sc
    DEPENDS shaderc ${SHADER_SRC_DIR}/vs_simple.sc ${SHADER_SRC_DIR}/varying.def.sc
    COMMENT "Compiling vertex shader simple.vb"
    VERBATIM
)

add_custom_command(
    OUTPUT ${SHADER_OUT_DIR}/simple.fg
    COMMAND shaderc -f ${SHADER_SRC_DIR}/fs_simple.sc -o ${SHADER_OUT_DIR}/simple.fg
            --type fragment --platform linux -p spirv
            -i ${BGFX_SHADER_INCLUDE} --varyingdef ${SHADER_SRC_DIR}/varying.def.sc
    DEPENDS shaderc ${SHADER_SRC_DIR}/fs_simple.sc ${SHADER_SRC_DIR}/varying.def.sc
    COMMENT "Compiling fragment shader simple.fg"
    VERBATIM
)

add_custom_target(shaders ALL DEPENDS ${SHADER_OUT_DIR}/simple.vb ${SHADER_OUT_DIR}/simple.fg)

# Sources
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*/*.cpp
)

include_directories(src)

add_executable(factorio-oss ${SOURCES})
add_dependencies(factorio-oss shaders copy_assets)

# Link libraries
target_link_libraries(factorio-oss
    bgfx
    bx
    Vulkan::Vulkan
    ${GLFW_LIBRARIES}
    ${LUA_LIBRARIES}
    dl
    pthread
)

# Linux-specific compile definitions
if(UNIX AND NOT APPLE)
    target_compile_definitions(factorio-oss PRIVATE PLATFORM_LINUX)
endif()

# Windows-specific (TBD)
if(WIN32)
    target_compile_definitions(factorio-oss PRIVATE PLATFORM_WINDOWS)
endif()

# Android-specific (TBD)
if(ANDROID)
    target_compile_definitions(factorio-oss PRIVATE PLATFORM_ANDROID)
endif()
