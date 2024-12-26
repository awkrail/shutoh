#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"
#include "error.hpp"
#include "opencv2/opencv.hpp"
#include <string>

class VideoStream {
    public:
        VideoStream(const std::string& input_path, const float framerate, 
                    cv::VideoCapture& cap, const FrameTimeCode& base_timecode_,
                    const FrameTimeCode& duration);
        const FrameTimeCode position() const;
        const bool is_end_frame() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_; }
        const float get_framerate() const { return framerate_; }
        const FrameTimeCode& get_base_timecode() const { return base_timecode_; }
        const FrameTimeCode& get_duration() const { return duration_; }

        static WithError<VideoStream> initialize_video_stream(const std::string& input_path);

    private:
        const std::string input_path_;
        const float framerate_;
        const FrameTimeCode base_timecode_;
        const FrameTimeCode duration_;
        cv::VideoCapture cap_;
};

#endif