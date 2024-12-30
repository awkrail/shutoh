#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>

template <typename T> struct WithError;

class VideoStream {
    public:
        VideoStream(const std::string& input_path, const float framerate, 
                    const FrameTimeCode& base_timecode_, const FrameTimeCode& duration,
                    cv::VideoCapture& cap);
        FrameTimeCode position() const;
        bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_; }
        float get_framerate() const { return framerate_; }
        const FrameTimeCode& get_base_timecode() const { return base_timecode_; }
        const FrameTimeCode& get_duration() const { return duration_; }
        WithError<void> seek(const int32_t frame_num);
        static WithError<VideoStream> initialize_video_stream(const std::filesystem::path& input_path);

    private:
        const std::string input_path_;
        const float framerate_;
        const FrameTimeCode base_timecode_;
        const FrameTimeCode duration_;
        cv::VideoCapture cap_;
};

#endif