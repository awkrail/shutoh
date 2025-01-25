#include <catch2/catch_test_macros.hpp>
#include "frame_timecode.hpp"

// Initialization
TEST_CASE("FrameTime initialization - FrameTimeCode", "[Frametime init]") {
    FrameTimeCode frametime = FrameTimeCode(0, 0.5);
    FrameTimeCode frametime2 = FrameTimeCode(frametime);
    REQUIRE(frametime.get_framerate() == frametime2.get_framerate());
    REQUIRE(frametime.get_frame_num() == frametime2.get_frame_num());
}

TEST_CASE("FrameTime initialization - from_frame_num", "[Frametime init]") {
    REQUIRE(frame_timecode::from_frame_nums(10, 0.5).value().get_frame_num() == 10);
}

TEST_CASE("FrameTime initialization - from_seconds", "[Frametime init]") {
    REQUIRE(frame_timecode::from_seconds(5.1f, 0.5).value().get_frame_num() == 3);
    REQUIRE(frame_timecode::from_seconds(5, 0.5).value().get_frame_num() == 3);
}

TEST_CASE("FrameTime initialization - from_timecode_string", "[Frametime init]") {
    REQUIRE(frame_timecode::from_timecode_string("01:30:05", 1.0).value().get_frame_num()  == 5405); // 1 * 3600 + 30 * 60 + 5 = 5405
    REQUIRE(frame_timecode::from_timecode_string("01:30:05.999", 1.0).value().get_frame_num()  == 5406);
    REQUIRE(frame_timecode::from_timecode_string("00:00:00", 1.0).value().get_frame_num() == 0);
    REQUIRE(frame_timecode::from_timecode_string("10:59:59", 1.0).value().get_frame_num() == 39599);
    REQUIRE(frame_timecode::from_timecode_string("10:59:59.999", 1.0).value().get_frame_num() == 39600);

    REQUIRE(frame_timecode::from_timecode_string("01:30:05", 60.0).value().get_frame_num()  == 324300); // 1 * 3600 + 30 * 60 + 5 = 5405
    REQUIRE(frame_timecode::from_timecode_string("01:30:05.999", 60.0).value().get_frame_num()  == 324360);
    REQUIRE(frame_timecode::from_timecode_string("00:00:00", 60.0).value().get_frame_num() == 0);
    REQUIRE(frame_timecode::from_timecode_string("10:59:59", 60.0).value().get_frame_num() == 2375940);
    REQUIRE(frame_timecode::from_timecode_string("10:59:59.999", 60.0).value().get_frame_num() == 2376000);
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
    list_a.push_back(frame_timecode::from_timecode_string("00:00:10", 1.0).value());
    list_a.push_back(frame_timecode::from_frame_nums(10, 1.0).value());
    list_a.push_back(frame_timecode::from_seconds(10.0f, 1.0).value());
    list_a.push_back(frame_timecode::from_seconds(10, 1.0).value());

    std::vector<FrameTimeCode> list_b;
    list_b.push_back(FrameTimeCode(5, 1.0));
    list_b.push_back(frame_timecode::from_timecode_string("00:00:05", 1.0).value());
    list_b.push_back(frame_timecode::from_frame_nums(5, 1.0).value());
    list_b.push_back(frame_timecode::from_seconds(5.0f, 1.0).value());
    list_b.push_back(frame_timecode::from_seconds(5, 1.0).value());

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