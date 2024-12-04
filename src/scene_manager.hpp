#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <queue>

#include "opencv2/opencv.hpp"
#include "frame_timecode.hpp"
#include "video_stream.hpp"

namespace scene_manager {

extern const int32_t DEFAULT_MIN_WIDTH;
extern const int32_t MAX_FRAME_QUEUE_LENGTH;

struct VideoFrame {
    cv::Mat frame;
}

class SceneManager {
    public:
        SceneManager();
        void detect_scenes(VideoStream& video);

    private:
        void _decode_thread(VideoStream& video,
                            int32_t downscale_factor, 
                            std::queue<int32_t>& frame_queue);

        // FrameTimeCode start_pos_;
        // FrameTimeCode end_pos_;

};

const int32_t compute_downscale_factor(const int32_t frame_width);

}

#endif