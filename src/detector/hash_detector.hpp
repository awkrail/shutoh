#ifndef HASH_DETECTOR_H
#define HASH_DETECTOR_H

#include "base_detector.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

struct VideoFrame;

class HashDetector : public BaseDetector {
    public:
        HashDetector(const float threshold = 0.395, const int32_t min_scene_len = 15,
                     const int32_t size = 16, const int32_t lowpass = 2);
        std::optional<int32_t> process_frame(VideoFrame& next_frame) override;

    private:
        const float threshold_;
        const int32_t min_scene_len_;
        const int32_t size_;
        const int32_t size_sq_;
        const int32_t factor_;
        std::optional<cv::Mat> last_frame_ = std::nullopt;
};

#endif