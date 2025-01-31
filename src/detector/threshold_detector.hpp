#ifndef THRESHOLD_DETECTOR_H
#define THRESHOLD_DETECTOR_H

#include "base_detector.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

enum class Fade {
    FADE_IN,
    FADE_OUT
};

class ThresholdDetector : public BaseDetector {
    public:
        ThresholdDetector(const int32_t threshold = 95, const int32_t min_scene_len = 15,
                          const float fade_bias = 0.0f, const bool add_final_scene = false);
        std::optional<int32_t> process_frame(const VideoFrame& next_frame) override;
    
    private:
        float _compute_frame_average(const cv::Mat& frame) const;

        const int32_t threshold_;
        const int32_t min_scene_len_;
        const float fade_bias_;
        const bool add_final_scene_;
        bool process_frame_ = false;
        int32_t last_frame_ = 0;
        std::optional<int32_t> last_scene_cut_ = std::nullopt;
        std::optional<Fade> last_fade_ = std::nullopt;
};

#endif