#ifndef CONTENT_DETECTOR_H
#define CONTENT_DETECTOR_H

#include "frame_timecode.hpp"
#include "video_frame.hpp"
#include "flash_filter.hpp"

#include <cstdint>
#include <optional>

namespace content_detector {

struct Components {
    const float delta_hue = 1.0;
    const float delta_sat = 1.0;
    const float delta_lum = 1.0;
    const float delta_edges = 0.0;
};

class ContentDetector {
    public:
        ContentDetector(const float threshold = 27.0, const int32_t min_scene_len = 15) : threshold_{threshold}, min_scene_len_{min_scene_len} {}
        std::optional<int32_t> process_frame(video_frame::VideoFrame& next_frame);

    private:
        float _calculate_frame_score(video_frame::VideoFrame& next_frame);
        float _mean_pixel_distance(video_frame::VideoFrame& next_frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const Components weights_ = Components();
        const flash_filter::FilterMode filter_mode_ = flash_filter::FilterMode::MERGE;
        float frame_score_;
        std::optional<cv::Mat> last_frame_ = std::nullopt;
        flash_filter::FlashFilter flash_filter_ = flash_filter::FlashFilter(filter_mode_, min_scene_len_);
};



}

#endif