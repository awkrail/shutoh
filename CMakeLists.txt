cmake_minimum_required(VERSION 3.14)

project(shutoh VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

find_package(OpenCV REQUIRED)

add_executable(shutoh
    src/main.cpp
    src/video_stream.cpp
    src/scene_manager.cpp
    src/frame_timecode.cpp
)

target_include_directories(shutoh PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(shutoh PRIVATE ${OpenCV_LIBS} argparse)