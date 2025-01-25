#include "video_stream.hpp"
#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <cmath>
#include <filesystem>

TEST_CASE("VideoStream initialization", "[VideoStream init]") {
    const std::string input_path = "../../video/input.mp4";
    WithError<VideoStream> video_stream = VideoStream::initialize_video_stream(input_path);
    std::cout << "hoge" << std::endl;
    REQUIRE(std::abs(video_stream.value().get_framerate() - 29.97f) < 0.001);
}