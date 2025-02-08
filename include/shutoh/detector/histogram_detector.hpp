#ifndef HISTOGRAM_DETECTOR_H
#define HISTOGRAM_DETECTOR_H

#include "base_detector.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

struct VideoFrame;

class HistogramDetector : public BaseDetector {
    public:
        HistogramDetector(const float threshold = 0.05, const int32_t min_scene_len = 15,
                          const int32_t bins = 256);
        std::optional<int32_t> process_frame(const VideoFrame& next_frame) override;
    
    private:
        void _calculate_histogram(const cv::Mat& frame, cv::Mat& hist) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const int32_t bins_;
        std::optional<int32_t> last_scene_cut_ = std::nullopt;
        std::optional<cv::Mat> last_hist_ = std::nullopt;
};

#endif