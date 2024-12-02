#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "opencv2/opencv.hpp"
#include <string>

#include "frame_timecode.hpp"

class VideoStream {
    public:
        VideoStream(const std::string& input_path);
        void read();
        frame_timecode::FrameTimeCode base_timecode();
    
    private:
        const std::string input_path_;
        double framerate_;
        cv::VideoCapture cap_;
};

#endif