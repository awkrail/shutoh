cmake_minimum_required(VERSION 3.14)

project(shutoh VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

# Library
file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/detector/*.cpp")
# list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_library(shutoh_lib SHARED ${LIB_SOURCES})
target_compile_options(shutoh_lib PRIVATE -O3 -Wall)
target_include_directories(shutoh_lib PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(shutoh_lib PUBLIC ${OpenCV_LIBS} fmt::fmt argparse)

# CLI
add_executable(shutoh src/main.cpp)
target_link_libraries(shutoh PRIVATE shutoh_lib)

# Tests
enable_testing()
add_subdirectory(tests)

# Install
install(TARGETS shutoh_lib DESTINATION lib)
install(TARGETS shutoh DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)