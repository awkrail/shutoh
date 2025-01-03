#ifndef CONTENT_DETECTOR_H
#define CONTENT_DETECTOR_H

#include "flash_filter.hpp"

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <optional>

struct VideoFrame;

struct Components {
    const float delta_hue = 1.0;
    const float delta_sat = 1.0;
    const float delta_lum = 1.0;
    const float delta_edges = 0.0;
};

class ContentDetector {
    public:
        ContentDetector(const float threshold, const int32_t min_scene_len);
        std::optional<int32_t> process_frame(VideoFrame& next_frame);

    private:
        float _calculate_frame_score(VideoFrame& next_frame);
        float _mean_pixel_distance(VideoFrame& next_frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const Components weights_ = Components();
        const FilterMode filter_mode_ = FilterMode::MERGE;
        float frame_score_;
        std::optional<cv::Mat> last_frame_ = std::nullopt;
        FlashFilter flash_filter_ = FlashFilter(filter_mode_, min_scene_len_);
};

#endif