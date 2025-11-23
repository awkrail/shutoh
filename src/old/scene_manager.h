#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "detector/base_detector.hpp"
#include "video_frame.hpp"
#include "frame_timecode.hpp"
#include "frame_timecode_pair.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include <memory>

class VideoStream;
template <typename T> class BlockingQueue;
template <typename T> struct WithError;

class SceneManager {
    public:
        explicit SceneManager(std::shared_ptr<BaseDetector> detector);
        void detect_scenes(VideoStream& video);
        WithError<std::vector<FrameTimeCodePair>> get_scene_list() const;

    private:
        void _process_frame(VideoFrame& next_frame);
        void _decode_thread(VideoStream& video,
                            const float downscale_factor,
                            BlockingQueue<VideoFrame>& frame_queue);
        std::vector<FrameTimeCode> _get_cutting_list() const;

        cv::Mat previous_frame_;
        std::vector<int32_t> cutting_list_;
        std::shared_ptr<BaseDetector> detector_;
        float framerate_ = 0.0f;
        std::optional<FrameTimeCode> start_ = std::nullopt;
        std::optional<FrameTimeCode> end_ = std::nullopt;
};

float compute_downscale_factor(const int32_t frame_width);

#endif
