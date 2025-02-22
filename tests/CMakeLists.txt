include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain shutoh)

include(CTest)
include(Catch)
catch_discover_tests(tests)
