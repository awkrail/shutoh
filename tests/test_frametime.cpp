#include <catch2/catch_test_macros.hpp>
#include "frame_timecode.hpp"

using namespace frame_timecode;

TEST_CASE("FrameTime initialization - FrameTimeCode", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime2 = FrameTimeCode(frametime);

    REQUIRE(frametime.get_framerate() == frametime2.get_framerate());
    REQUIRE(frametime.get_frame_num() == frametime2.get_frame_num());
}

TEST_CASE("FrameTime initialization - from_frame_num", "[Frametime init]") {
    FrameTimeCode frametime = from_frame_nums(10, 0.5);
    REQUIRE(frametime.get_frame_num() == 10);
}

TEST_CASE("FrameTime initialization - from_seconds - double", "[Frametime init]") {
    FrameTimeCode frametime = from_seconds(5.0, 0.5);
    REQUIRE(frametime.get_frame_num() == 3);
}

TEST_CASE("FrameTime initialization - from_seconds - int", "[Frametime init]") {
    FrameTimeCode frametime = from_seconds(5, 0.5);
    REQUIRE(frametime.get_frame_num() == 3);
}

TEST_CASE("FrameTime initialization - HH:MM:SS", "[Frametime init]") {
    std::string timecode_str = "01:30:05";
    FrameTimeCode frametime = from_timecode_string(timecode_str, 1.0);
    REQUIRE(frametime.get_framerate() == 1.0);
    REQUIRE(frametime.get_frame_num() == 5405);
}

TEST_CASE("FrameTime initialization - HH:MM:SS.MMM", "[Frametime init]") {
    std::string timecode_str = "01:30:05.500";
    FrameTimeCode frametime = from_timecode_string(timecode_str, 1.0);
    REQUIRE(frametime.get_framerate() == 1.0);
    REQUIRE(frametime.get_frame_num() == 5405);
}