#ifndef ADAPTIVE_DETECTOR_H
#define ADAPTIVE_DETECTOR_H

#include "content_detector.hpp"
#include "fixed_size_deque.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>
#include <vector>
#include <tuple>

struct VideoFrame;

struct FrameNumScore {
    const int32_t frame_num;
    const float frame_score;
};

class AdaptiveDetector : public ContentDetector {
    public:
        AdaptiveDetector(const float adaptive_threshold = 3.0f, const int32_t min_scene_len = 15,
                         const int32_t window_width = 2, const float min_content_val = 15.0f);
        std::optional<int32_t> process_frame(const VideoFrame& next_frame) override;
        static std::shared_ptr<AdaptiveDetector> initialize_detector(float adaptive_threshold = 3.0f,
                                                                     int32_t min_scene_len = 15,
                                                                     int32_t window_width = 2,
                                                                     float min_content_val = 15.0f);
    
    private:
        float _calculate_average_window_score() const;
        
        const float adaptive_threshold_;
        const int32_t min_scene_len_;
        const int32_t window_width_;
        const float min_content_val_;
        const size_t required_frames_;
        FixedSizeDeque<FrameNumScore> buffer_;
        std::optional<int32_t> last_cut_ = std::nullopt;
        std::optional<int32_t> first_frame_num_ = std::nullopt;
};


#endif