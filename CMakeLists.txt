cmake_minimum_required(VERSION 3.17)
project(cx_coreutils)
message("core_utils message: CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}\tPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
set(EXPORT_COMPILE_COMMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wdouble-promotion")

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG master
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/deps/fmt)

FetchContent_MakeAvailable(fmt)

# if(NOT fmt_POPULATED)
#     FetchContent_Populate(fmt)
#     add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
# endif()

include_directories(./src)
# My own dependencies, that's part of this core utils package, but not necessarily part of the source tree, are found in ./deps/local
# (to be frank, I don't know how to package up libraries nicely quite yet. C++ is horrible in that way)
add_subdirectory(./deps/local/instrumentation)

add_executable(core_test ./src/tests/core_test.cpp ./src/core/core.cpp)
add_executable(tree_test ./src/tests/tree_test.cpp ./src/containers/tree.hpp)

# This sets a variable that the C/C++ pre-processor can see, i.e. DEBUG.
# TODO(fix): This is a really bad name, change it to something else like INCLUDES_FMT etc, to more clearly specify what it does (or is supposed to do)
target_compile_definitions(tree_test PRIVATE DEBUG)



target_compile_options(core_test PRIVATE "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wdouble-promotion")
target_link_libraries(core_test fmt::fmt instrumentation)
target_link_libraries(tree_test fmt::fmt)




