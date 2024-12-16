#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"
#include "error.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <optional>

namespace video_stream {

class VideoStream {
    public:
        VideoStream(const std::string& input_path, float framerate, cv::VideoCapture cap);
        const frame_timecode::FrameTimeCode base_timecode() const;
        const frame_timecode::FrameTimeCode duration() const;
        const frame_timecode::FrameTimeCode position() const;
        const int32_t frame_number() const;
        const bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_.value(); }
        const float get_framerate() const { return framerate_; }

    private:
        const std::string input_path_;
        float framerate_;
        std::optional<cv::VideoCapture> cap_ = std::nullopt;
        std::optional<Error> error_ = std::nullopt;
};

WithError<VideoStream> initialize_video_stream(const std::string& input_path);

}

#endif