#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"
#include "error.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <optional>

class VideoStream {
    public:
        VideoStream(const std::string& input_path, float framerate, cv::VideoCapture cap);
        const FrameTimeCode base_timecode() const;
        const FrameTimeCode duration() const;
        const FrameTimeCode position() const;
        const int32_t frame_number() const;
        const bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_.value(); }
        const float get_framerate() const { return framerate_; }

        static WithError<VideoStream> initialize_video_stream(const std::string& input_path);

    private:
        const std::string input_path_;
        float framerate_;
        std::optional<cv::VideoCapture> cap_ = std::nullopt;
};

#endif