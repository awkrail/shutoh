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
    REQUIRE(from_frame_nums(10, 0.5).get_frame_num() == 10);
    REQUIRE_THROWS_AS(from_frame_nums(-1, 1.0), std::invalid_argument);
    REQUIRE_THROWS_AS(from_frame_nums(10, 0), std::invalid_argument);
}

TEST_CASE("FrameTime initialization - from_seconds", "[Frametime init]") {
    REQUIRE(from_seconds(5.1f, 0.5).get_frame_num() == 3);
    REQUIRE(from_seconds(5, 0.5).get_frame_num() == 3);
    REQUIRE_THROWS_AS(from_seconds(-5.1f, 1.0), std::invalid_argument);
    REQUIRE_THROWS_AS(from_seconds(5, 0), std::invalid_argument);
}

TEST_CASE("FrameTime initialization - HH:MM:SS[.nnn]", "[Frametime init]") {
    REQUIRE(from_timecode_string("01:30:05", 1.0).get_frame_num()  == 5405); // 1 * 3600 + 30 * 60 + 5 = 5405
    REQUIRE(from_timecode_string("01:30:05.999", 1.0).get_frame_num()  == 5406);
    REQUIRE(from_timecode_string("00:00:00", 1.0).get_frame_num() == 0);
    REQUIRE(from_timecode_string("23:59:59", 1.0).get_frame_num() == 86399);
    REQUIRE_THROWS_AS(from_timecode_string("invalid", 1.0), std::invalid_argument); // Invalid case
}