#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "opencv2/opencv.hpp"
#include "content_detector.hpp"
#include "video_frame.hpp"
#include "frame_timecode_pair.hpp"

#include <vector>
#include <cstdint>
#include <optional>

class VideoStream;
template <typename T> class BlockingQueue;
template <typename T> struct WithError;

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

int32_t compute_downscale_factor(const int32_t frame_width);

#endif