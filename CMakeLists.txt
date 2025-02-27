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

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Library
if (EXISTS "${CMAKE_SOURCE_DIR}/src")
  file(GLOB LIB_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/detector/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/bindings/*.cpp")
else()
  FetchContent_MakeAvailable(shutoh)
  file(GLOB LIB_SOURCES
    "${shutoh_SOURCE_DIR}/src/*.cpp"
    "${shutoh_SOURCE_DIR}/src/detector/*.cpp"
    "${shutoh_SOURCE_DIR}/src/bindings/*.cpp"
  )
endif()
add_library(shutoh SHARED ${LIB_SOURCES})
target_compile_options(shutoh PRIVATE -O3 -Wall)
target_include_directories(shutoh PUBLIC include ${OpenCV_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(shutoh PUBLIC ${OpenCV_LIBS} fmt::fmt argparse pybind11::module ${Python3_LIBRARIES})

# CLI
add_executable(shutoh_cli src/main.cpp)
target_link_libraries(shutoh_cli PRIVATE shutoh)

# Tests
enable_testing()
add_subdirectory(tests)

# Install
install(TARGETS shutoh DESTINATION lib)
install(TARGETS shutoh_cli DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
