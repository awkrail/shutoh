#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include <optional>

template <typename T> struct WithError;

class VideoStream {
    public:
        VideoStream(const std::string& input_path, const float framerate, cv::VideoCapture& cap);
        FrameTimeCode position() const;
        bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        WithError<void> set_time(const std::optional<std::string>& start, const std::optional<std::string>& end, 
                                 const std::optional<std::string>& duration);
        cv::VideoCapture& get_cap() { return cap_; }
        float get_framerate() const { return framerate_; }
        const FrameTimeCode& get_start() const { return start_; }
        const FrameTimeCode& get_end() const { return end_; }
        WithError<void> seek(const int32_t frame_num);
        static WithError<VideoStream> initialize_video_stream(const std::filesystem::path& input_path);

    private:
        const std::string input_path_;
        const float framerate_;
        cv::VideoCapture cap_;
        FrameTimeCode start_;
        FrameTimeCode end_;
};

#endif