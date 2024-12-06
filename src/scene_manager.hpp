#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <queue>
#include <vector>

#include "opencv2/opencv.hpp"
#include "frame_timecode.hpp"
#include "video_frame.hpp"
#include "video_stream.hpp"
#include "blocking_queue.hpp"
#include "content_detector.hpp"

namespace scene_manager {

extern const int32_t DEFAULT_MIN_WIDTH;
extern const int32_t MAX_FRAME_QUEUE_LENGTH;

class SceneManager {
    public:
        SceneManager(content_detector::ContentDetector& detector);
        void detect_scenes(video_stream::VideoStream& video);

    private:
        void _process_frame(video_frame::VideoFrame& next_frame);
        void _decode_thread(video_stream::VideoStream& video,
                            int32_t downscale_factor,
                            BlockingQueue<video_frame::VideoFrame>& frame_queue);

        cv::Mat previous_frame_;
        std::vector<std::vector<frame_timecode::FrameTimeCode>> cut_list_;
        content_detector::ContentDetector detector_;
        // FrameTimeCode start_pos_;
        // FrameTimeCode end_pos_;

};

const int32_t compute_downscale_factor(const int32_t frame_width);

}

#endif