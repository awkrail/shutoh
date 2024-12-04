#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "frame_timecode.hpp"
#include "opencv2/opencv.hpp"
#include <string>

namespace video_stream {

class VideoStream {
    public:
        VideoStream(const std::string& input_path);
        void read();
        const frame_timecode::FrameTimeCode base_timecode() const;
        const frame_timecode::FrameTimeCode duration() const;
        const frame_timecode::FrameTimeCode position() const;
        const int32_t frame_number() const;

        int32_t width() const;
        int32_t height() const;
        cv::VideoCapture& get_cap() { return cap_; }
        const float get_framerate() const { return framerate_; }

    private:
        const std::string input_path_;
        float framerate_;
        cv::VideoCapture cap_;
};

}

#endif