#include "content_detector.hpp"
#include "frame_timecode.hpp"

namespace content_detector {

    frame_timecode::FrameTimeCode ContentDetector::process_frame(video_frame::VideoFrame& next_frame) {
        return frame_timecode::from_frame_nums(0, 0.5);
    }

}