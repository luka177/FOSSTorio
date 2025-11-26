cmake_minimum_required(VERSION 3.20)
project(factorio-oss)

set(CMAKE_CXX_STANDARD 17)
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

# Sources
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*/*.cpp
)

include_directories(src)

add_executable(factorio-oss ${SOURCES})

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
