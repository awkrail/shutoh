#include <catch2/catch_test_macros.hpp>
#include "frame_timecode.hpp"

using namespace frame_timecode;

// Initialization
TEST_CASE("FrameTime initialization - FrameTimeCode", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime2 = FrameTimeCode(frametime);
    REQUIRE(frametime.get_framerate() == frametime2.get_framerate());
    REQUIRE(frametime.get_frame_num() == frametime2.get_frame_num());
}

TEST_CASE("FrameTime initialization - from_frame_num", "[Frametime init]") {
    REQUIRE(from_frame_nums(10, 0.5).get_frame_num() == 10);
}

TEST_CASE("FrameTime initialization - from_seconds", "[Frametime init]") {
    REQUIRE(from_seconds(5.1f, 0.5).get_frame_num() == 3);
    REQUIRE(from_seconds(5, 0.5).get_frame_num() == 3);
}

TEST_CASE("FrameTime initialization - HH:MM:SS[.nnn]", "[Frametime init]") {
    REQUIRE(from_timecode_string("01:30:05", 1.0).get_frame_num()  == 5405); // 1 * 3600 + 30 * 60 + 5 = 5405
    REQUIRE(from_timecode_string("01:30:05.999", 1.0).get_frame_num()  == 5406);
    REQUIRE(from_timecode_string("00:00:00", 1.0).get_frame_num() == 0);
    REQUIRE(from_timecode_string("23:59:59", 1.0).get_frame_num() == 86399);
}

// Compare, Add, and substract
TEST_CASE("FrameTime comparison - equal", "[FrameTime comparison]") {
    FrameTimeCode frametime = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime2 = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime3 = FrameTimeCode(5, 0.5);
    REQUIRE(frametime == frametime2);
    REQUIRE(frametime != frametime3);
}

TEST_CASE("FrameTime comparison - Add and substract", "[FrameTime comparison]") {
    std::vector<FrameTimeCode> list_a;
    list_a.push_back(FrameTimeCode(10, 1.0));
    list_a.push_back(from_timecode_string("00:00:10", 1.0));
    list_a.push_back(from_frame_nums(10, 1.0));
    list_a.push_back(from_seconds(10.0f, 1.0));
    list_a.push_back(from_seconds(10, 1.0));

    std::vector<FrameTimeCode> list_b;
    list_b.push_back(FrameTimeCode(5, 1.0));
    list_b.push_back(from_timecode_string("00:00:05", 1.0));
    list_b.push_back(from_frame_nums(5, 1.0));
    list_b.push_back(from_seconds(5.0f, 1.0));
    list_b.push_back(from_seconds(5, 1.0));

    for(const FrameTimeCode& a : list_a) {
        for(const FrameTimeCode& b : list_b) {
            REQUIRE(a > b);
            REQUIRE(b < a);
            REQUIRE(a >= b);
            REQUIRE(b <= a);
            REQUIRE(b >= b);
            REQUIRE(b <= b);
            REQUIRE(a + b == FrameTimeCode(15, 1.0));
            REQUIRE(a - b == FrameTimeCode(5, 1.0));
            REQUIRE(b - a == FrameTimeCode(0, 1.0));
        }
    }
}