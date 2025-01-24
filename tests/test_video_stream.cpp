#include "video_stream.hpp"
#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <cmath>
#include <filesystem>

TEST_CASE("VideoStream initialization", "[VideoStream init]") {
    const std::string input_path = "../../video/input.mp4";

    
    const FrameTimeCode start = frame_timecode::from_timecode_string("00:00:00",);
    const FrameTimeCode end = "00:02:30";

    WithError<VideoStream> video_stream = VideoStream::initialize_video_stream();
    


    VideoStream video_stream = VideoStream(input_path);
    REQUIRE(std::abs(video_stream.get_framerate() - 29.97f) < 0.001);
}