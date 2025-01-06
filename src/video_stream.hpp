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
                    const FrameTimeCode& start, const FrameTimeCode& end,
                    cv::VideoCapture& cap);
        FrameTimeCode position() const;
        bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_; }
        float get_framerate() const { return framerate_; }
        const FrameTimeCode& get_start() const { return start_; }
        const FrameTimeCode& get_end() const { return end_; }
        WithError<void> seek(const int32_t frame_num);
        static WithError<VideoStream> initialize_video_stream(const std::filesystem::path& input_path,
                                                              const FrameTimeCode& start,
                                                              const FrameTimeCode& end);

    private:
        const std::string input_path_;
        const float framerate_;
        const FrameTimeCode start_;
        const FrameTimeCode end_;
        cv::VideoCapture cap_;
};

#endif