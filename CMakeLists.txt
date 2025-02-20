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

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

if(WIN32)
    set(OpenCV_DIR "C:/opencv-4.10.0/build")
endif()

find_package(OpenCV REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Library
file(GLOB LIB_SOURCES 
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/detector/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/bindings/*.cpp")
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
