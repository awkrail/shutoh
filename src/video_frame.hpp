#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include "opencv2/opencv.hpp"
#include "frame_timecode.hpp"
#include <cstdint>

namespace video_frame {

struct VideoFrame {
    cv::Mat frame;
    frame_timecode::FrameTimeCode position;
    const bool is_end_frame = false;
    const int32_t num_pixels;
};

}

#endif