#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>

struct VideoFrame {
    cv::Mat frame;
    FrameTimeCode position;
    const bool is_end_frame = false;
    const int32_t num_pixels;
};

#endif