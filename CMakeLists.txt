cmake_minimum_required(VERSION 3.21)
project(DummyLux VERSION 0.1.0)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# vcpkg integration
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Add source files
add_executable(DummyLux src/main.cpp)

# Include directories
target_include_directories(DummyLux PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
