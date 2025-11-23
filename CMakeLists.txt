cmake_minimum_required(VERSION 3.14)

project(shutoh VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)

file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(shutoh ${SOURCE_FILES})
target_compile_options(shutoh PRIVATE -O3 -Wall)
target_include_directories(shutoh PUBLIC ${OpenCV_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(shutoh PUBLIC ${OpenCV_LIBS} fmt::fmt argparse)
