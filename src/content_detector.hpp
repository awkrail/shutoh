#ifndef CONTENT_DETECTOR_H
#define CONTENT_DETECTOR_H

#include "flash_filter.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

struct VideoFrame;

class ContentDetector {
    public:
        ContentDetector(const float threshold, const int32_t min_scene_len);
        std::optional<int32_t> process_frame(VideoFrame& next_frame);

    private:
        float _calculate_frame_score(VideoFrame& next_frame);
        float _mean_pixel_distance(VideoFrame& next_frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const FilterMode filter_mode_ = FilterMode::MERGE;
        std::optional<cv::Mat> last_frame_ = std::nullopt;
        FlashFilter flash_filter_ = FlashFilter(filter_mode_, min_scene_len_);
};

#endif