#include "video_stream.hpp"
#include "error.hpp"

#include <string>
#include <filesystem>
#include <cmath>

VideoStream::VideoStream(const std::string& input_path, const float framerate,
                        const FrameTimeCode& start, const FrameTimeCode& end,
                        cv::VideoCapture& cap) : input_path_{input_path}, framerate_{framerate}, 
                        start_{start}, end_{end}, cap_{cap} {}

FrameTimeCode VideoStream::position() const {
    const int32_t frame_num = static_cast<int32_t>(cap_.get(cv::CAP_PROP_POS_FRAMES));
    if (frame_num < 1)
        return start_;

    WithError<FrameTimeCode> cur_timecode = frame_timecode::from_frame_nums(frame_num - 1, framerate_);
    return cur_timecode.value();
}

bool VideoStream::is_end_frame() const {
    return position().get_frame_num() == end_.get_frame_num() - 1;
}

int32_t VideoStream::width() const {
    return static_cast<int32_t>(cap_.get(cv::CAP_PROP_FRAME_WIDTH));
}

int32_t VideoStream::height() const {
    return static_cast<int32_t>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT));
}

WithError<void> VideoStream::seek(const int32_t frame_num) {
    if (frame_num < 0) {
        std::string error_msg = "";
        return WithError<void> { Error(ErrorCode::NegativeFrameNum, error_msg) };
    }

    if (frame_num >= cap_.get(cv::CAP_PROP_FRAME_COUNT)) {
        std::string error_msg = "Target frame num is over the maximum frame count.";
        return WithError<void> { Error(ErrorCode::OverMaximumFrameNum, error_msg) };
    }

    if (!cap_.set(cv::CAP_PROP_POS_FRAMES, frame_num)) {
        std::string error_msg = "Failed to set the frame position.";
        return WithError<void> { Error(ErrorCode::FailedToSetFramePosition, error_msg) };
    }

    return WithError<void> { Error(ErrorCode::Success, "") };
}

WithError<VideoStream> VideoStream::initialize_video_stream(const std::filesystem::path& input_path,
                                                            const FrameTimeCode& start,
                                                            const FrameTimeCode& end) {
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
        const std::string error_msg = "Not supported codec.";
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::NotSupportedCodec, error_msg) };
    }

    float framerate = cap.get(cv::CAP_PROP_FPS);
    if (framerate < frame_timecode::MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }

    return WithError<VideoStream> { VideoStream(input_path.string(), framerate, start, end, cap), Error(ErrorCode::Success, "") };
}