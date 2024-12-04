#include <catch2/catch_test_macros.hpp>
#include <string>
#include <cmath>
#include <filesystem>
#include "video_stream.hpp"

using namespace video_stream;

TEST_CASE("VideoStream initialization", "[VideoStream init]") {
    std::string input_path = "../../video/input.mp4";
    VideoStream video_stream = VideoStream(input_path);
    REQUIRE(std::abs(video_stream.get_framerate() - 29.97f) < 0.001);
}