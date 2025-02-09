#ifndef CONTENT_DETECTOR_H
#define CONTENT_DETECTOR_H

#include "flash_filter.hpp"
#include "base_detector.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

struct VideoFrame;

class ContentDetector : public BaseDetector {
    public:
        ContentDetector(const float threshold = 27.0f, const int32_t min_scene_len = 15);
        std::optional<int32_t> process_frame(const VideoFrame& next_frame) override;
        std::optional<float> get_frame_score() const { return frame_score_; }
        static std::unique_ptr<ContentDetector> initialize_detector(float threshold = 27.0f,
                                                                    int32_t min_scene_len = 15);

    private:
        float _calculate_frame_score(const VideoFrame& next_frame);
        float _mean_pixel_distance(const VideoFrame& next_frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const FilterMode filter_mode_ = FilterMode::MERGE;
        std::optional<float> frame_score_ = std::nullopt;
        std::optional<cv::Mat> last_frame_ = std::nullopt;
        FlashFilter flash_filter_ = FlashFilter(filter_mode_, min_scene_len_);
};

#endif