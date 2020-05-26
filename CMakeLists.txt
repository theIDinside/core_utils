cmake_minimum_required(VERSION 3.17)
project(cx_coreutils)
set(EXPORT_COMPILE_COMMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wdouble-promotion")

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG master
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/deps/fmt)

FetchContent_MakeAvailable(fmt)

if(NOT fmt_POPULATED)
    FetchContent_Populate(fmt)
    add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif()

include_directories(./src)
# My own dependencies, that's part of this core utils package, but not necessarily part of the source tree, are found in ./deps/local
# (to be frank, I don't know how to package up libraries nicely quite yet. C++ is horrible in that way)
add_subdirectory(./deps/local/instrumentation)
add_executable(core_test ./src/main.cpp ./src/core/core.cpp)
target_link_libraries(core_test fmt::fmt instrumentation)
