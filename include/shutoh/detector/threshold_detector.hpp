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
        ThresholdDetector(const float threshold = 95.0f, const int32_t min_scene_len = 15, const float fade_bias = 0.0f);
        std::optional<int32_t> process_frame(const VideoFrame& next_frame) override;
        static std::unique_ptr<ThresholdDetector> initialize_detector(float threshold = 95.0f,
                                                                      int32_t min_scene_len = 15,
                                                                      float fade_bias = 0.0f);

    private:
        float _compute_frame_average(const cv::Mat& frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const float fade_bias_;
        bool process_frame_ = false;
        int32_t last_frame_ = 0;
        std::optional<int32_t> last_scene_cut_ = std::nullopt;
        std::optional<Fade> last_fade_ = std::nullopt;
};

#endif