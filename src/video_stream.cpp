#include "opencv2/opencv.hpp"
#include <string>
#include <filesystem>
#include <stdexcept>

#include "video_stream.hpp"
#include "frame_timecode.hpp"

VideoStream::VideoStream(const std::string& input_path)
    : input_path_{input_path}, framerate_{0.0}, cap_{} {

    if (!std::filesystem::exists(input_path)) {
        throw std::runtime_error("Video file not found.");
    }

    cv::VideoCapture cap(input_path);
    if(!cap.isOpened()) {
        throw std::runtime_error("Could not open video file.");
    }

    int8_t codec = static_cast<int8_t>(cap.get(cv::CAP_PROP_FOURCC));
    bool codec_unsupported = (std::abs(codec) == 0);

    if(codec_unsupported) {
        throw std::runtime_error("Video codec detection failed. Unsupported video codec.");
    }

    float framerate = cap.get(cv::CAP_PROP_FPS);
    if (framerate < frame_timecode::MAX_FPS_DELTA) {
        throw std::runtime_error("Frame rate is over MAX_FPS_DELTA.");
    }

    framerate_ = framerate;
    cap_ = cap;
}

frame_timecode::FrameTimeCode VideoStream::base_timecode() {
    int32_t timecode = 0;
    return frame_timecode::FrameTimeCode(timecode, framerate_);
}

void VideoStream::read() {
}