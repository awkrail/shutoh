#include <catch2/catch_test_macros.hpp>
#include "frame_timecode.hpp"

TEST_CASE("FrameTime initialization - FrameTimeCode", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime2 = FrameTimeCode(frametime);

    REQUIRE(frametime.get_framerate() == frametime2.get_framerate());
    REQUIRE(frametime.get_framenum() == frametime2.get_framenum());
}

TEST_CASE("FrameTime initialization - int32_t", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(1, 0.5);
    REQUIRE(frametime.get_framerate() == 0.5);
    REQUIRE(frametime.get_framenum() == 1);
}

TEST_CASE("FrameTime initialization - double", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(1.25, 0.5);
    REQUIRE(frametime.get_framerate() == 0.5);
    REQUIRE(frametime.get_framenum() == 1.25);
}