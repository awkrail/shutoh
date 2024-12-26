#include <string>
#include <filesystem>
#include <stdexcept>
#include <cmath>

#include "opencv2/opencv.hpp"
#include "video_stream.hpp"
#include "frame_timecode.hpp"
#include "error.hpp"

VideoStream::VideoStream(const std::string& input_path, const float framerate,
                         cv::VideoCapture& cap, const FrameTimeCode& base_timecode,
                         const FrameTimeCode& duration) 
                         : input_path_{input_path}, framerate_{framerate}, cap_{cap},
                         base_timecode_{base_timecode}, duration_{duration} {}

const FrameTimeCode VideoStream::position() const {
    const int32_t frame_num = static_cast<int32_t>(cap_.get(cv::CAP_PROP_POS_FRAMES));
    if (frame_num < 1) {
        return base_timecode_;
    }
    WithError<FrameTimeCode> cur_timecode = frame_timecode::from_frame_nums(frame_num - 1, framerate_);
    return base_timecode_ + cur_timecode.value();
}

const bool VideoStream::is_end_frame() const {
    return position().get_frame_num() == duration_.get_frame_num() - 1;
}

int32_t VideoStream::width() const {
    return static_cast<int32_t>(cap_.get(cv::CAP_PROP_FRAME_WIDTH));
}

int32_t VideoStream::height() const {
    return static_cast<int32_t>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT));
}

WithError<VideoStream> VideoStream::initialize_video_stream(const std::filesystem::path& input_path) {
    if (!std::filesystem::exists(input_path)) {
        const std::string error_msg = "No such file: " + input_path.string();
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::NoSuchFile, error_msg) };
    }

    cv::VideoCapture cap(input_path);
    if (!cap.isOpened()) {
        const std::string error_msg = "Failed to open the video: " + input_path.string();
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::FailedToOpenFile, error_msg) };
    }

    int8_t codec = static_cast<int8_t>(cap.get(cv::CAP_PROP_FOURCC));
    bool codec_unsupported = (std::abs(codec) == 0);
    if (codec_unsupported) {
        const std::string error_msg = "Not supported codec";
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::NotSupportedCodec, error_msg) };
    }

    float framerate = cap.get(cv::CAP_PROP_FPS);
    if (framerate < frame_timecode::MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }

    const FrameTimeCode base_timecode = FrameTimeCode(0, framerate);
    const int32_t total_frame_num = static_cast<int32_t>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    WithError<FrameTimeCode> end_timecode = frame_timecode::from_frame_nums(total_frame_num, framerate);
    const FrameTimeCode duration = base_timecode + end_timecode.value();

    return WithError<VideoStream> { VideoStream(input_path.string(), framerate, cap, base_timecode, duration), Error(ErrorCode::Success, "") };
}