cmake_minimum_required(VERSION 3.14)

project(shutoh VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(shutoh ${SOURCES})

target_compile_options(shutoh PRIVATE -O3 -Wall)
target_include_directories(shutoh PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(shutoh PRIVATE ${OpenCV_LIBS} argparse fmt::fmt)

# tests
enable_testing()
add_subdirectory(tests)