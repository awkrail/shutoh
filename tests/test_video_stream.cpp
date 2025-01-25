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
    REQUIRE(std::abs(video_stream.value().get_framerate() - 29.97f) < 0.001);
}

TEST_CASE("VideoStream initialize - faiked to open file", "[VideoStream init]") {
    const std::string input_path = "unexisting_file.mp4";
    WithError<VideoStream> video_stream = VideoStream::initialize_video_stream(input_path);
    REQUIRE(video_stream.has_error());
}