#include "opencv2/opencv.hpp"
#include <string>
#include <filesystem>
#include <stdexcept>

#include "video_stream.hpp"

double MAX_FPS_DELTA = 1.0 / 100000;

VideoStream::VideoStream(const std::string& input_path)
    : input_path_{input_path}, framerate_{0.0}, cap_{} {

    if (!std::filesystem::exists(input_path)) {
        throw std::runtime_error("Video file not found.");
    }

    cv::VideoCapture cap(input_path);
    if(!cap.isOpened()) {
        throw std::runtime_error("Could not open video file.");
    }

    int codec = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    bool codec_unsupported = (std::abs(codec) == 0);

    if(codec_unsupported) {
        throw std::runtime_error("Video codec detection failed. Unsupported video codec.");
    }

    double framerate = cap.get(cv::CAP_PROP_FPS);
    if (framerate < MAX_FPS_DELTA) {
        throw std::runtime_error("Frame rate is over MAX_FPS_DELTA.");
    }

    framerate_ = framerate;
    cap_ = cap;
}

void VideoStream::read() {
}