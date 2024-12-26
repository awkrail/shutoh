#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <queue>
#include <vector>
#include <cstdint>
#include <optional>

#include "opencv2/opencv.hpp"
#include "frame_timecode.hpp"
#include "video_frame.hpp"
#include "video_stream.hpp"
#include "blocking_queue.hpp"
#include "content_detector.hpp"
#include "error.hpp"

extern const int32_t DEFAULT_MIN_WIDTH;
extern const int32_t MAX_FRAME_QUEUE_LENGTH;

using FrameTimeCodePair = std::tuple<FrameTimeCode, FrameTimeCode>;

class SceneManager {
    public:
        SceneManager(ContentDetector& detector);
        void detect_scenes(VideoStream& video);
        WithError<std::vector<FrameTimeCodePair>> get_scene_list() const;

    private:
        void _process_frame(VideoFrame& next_frame);
        void _decode_thread(VideoStream& video,
                            int32_t downscale_factor,
                            BlockingQueue<VideoFrame>& frame_queue);
        std::vector<FrameTimeCode> _get_cutting_list() const;

        cv::Mat previous_frame_;
        std::vector<int32_t> cutting_list_;
        ContentDetector detector_;
        float framerate_ = 0.0f;
        std::optional<FrameTimeCode> base_timecode_ = std::nullopt;
        std::optional<FrameTimeCode> start_pos_ = std::nullopt;
        std::optional<FrameTimeCode> last_pos_ = std::nullopt;

};

const int32_t compute_downscale_factor(const int32_t frame_width);

#endif