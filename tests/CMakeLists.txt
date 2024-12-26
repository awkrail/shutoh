include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

find_package(OpenCV REQUIRED)

add_executable(tests 
    #test_frametime.cpp
    #test_scene_manager.cpp
    #test_video_stream.cpp
    # test_content_detector.cpp
    ${SOURCES})

target_include_directories(shutoh PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(tests PRIVATE ${OpenCV_LIBS} Catch2::Catch2WithMain fmt::fmt)

include(CTest)
include(Catch)
catch_discover_tests(tests)