#include "shutoh/video_stream.hpp"
#include "shutoh/error.hpp"

#include <string>
#include <filesystem>
#include <cmath>

VideoStream::VideoStream(const std::string& input_path, const float framerate, cv::VideoCapture& cap) 
                        : input_path_{input_path}, framerate_{framerate}, cap_{cap}, 
                          start_{0, framerate_}, end_{0, framerate_} {
    const int32_t total_frame_num = static_cast<int32_t>(cap_.get(cv::CAP_PROP_FRAME_COUNT));
    end_ = FrameTimeCode(total_frame_num, framerate_);
}

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

WithError<void> VideoStream::set_time(const std::optional<std::string>& start, const std::optional<std::string>& end, 
                                      const std::optional<std::string>& duration) {
    if (start.has_value()) {
        const WithError<FrameTimeCode> start_err = frame_timecode::from_timecode_string(start.value(), framerate_);
        if (start_err.has_error())
            return WithError<void> { start_err.error };
        start_ = start_err.value();
    }

    const int32_t frame_num = static_cast<int32_t>(cap_.get(cv::CAP_PROP_POS_FRAMES));
    const FrameTimeCode video_end = frame_timecode::from_frame_nums(frame_num, framerate_).value();

    if (end.has_value()) {
        const WithError<FrameTimeCode> end_err = frame_timecode::from_timecode_string(end.value(), framerate_);
        if (end_err.has_error())
            return WithError<void> { end_err.error };
        end_ = end_err.value() <= video_end ? end_err.value() : video_end;

    } else if (duration.has_value()) {
        const WithError<FrameTimeCode> duration_err = frame_timecode::from_timecode_string(duration.value(), framerate_);
        if (duration_err.has_error())
            return WithError<void> { duration_err.error };
        
        end_ = start_ + duration_err.value() <= video_end ? start_ + duration_err.value() : video_end;
    }

    if (start_ >= end_) {
        std::string error_msg = "--start is smaller than video length.";
        return WithError<void> { Error(ErrorCode::InvalidArgument, error_msg) };
    }

    return WithError<void> { Error(ErrorCode::Success, "") };
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

    const int8_t codec = static_cast<int8_t>(cap.get(cv::CAP_PROP_FOURCC));
    const bool codec_unsupported = (std::abs(codec) == 0);
    if (codec_unsupported) {
        const std::string error_msg = "Not supported codec.";
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::NotSupportedCodec, error_msg) };
    }

    float framerate = cap.get(cv::CAP_PROP_FPS);
    if (framerate < frame_timecode::MIN_FPS_DELTA) {
        const std::string error_msg = "Framerate should be larger than MIN_FPS_DELTA = " + std::to_string(frame_timecode::MIN_FPS_DELTA);
        return WithError<VideoStream> { std::nullopt, Error(ErrorCode::TooSmallFpsValue, error_msg) };
    }

    return WithError<VideoStream> { VideoStream(input_path.string(), framerate, cap), Error(ErrorCode::Success, "") };
}