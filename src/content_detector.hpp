#ifndef CONTENT_DETECTOR_H
#define CONTENT_DETECTOR_H

#include "frame_timecode.hpp"
#include "video_frame.hpp"

#include <cstdint>
#include <optional>

namespace content_detector {

struct Components {
    const float delta_hue = 1.0;
    const float delta_sat = 1.0;
    const float delta_lum = 1.0;
    const float delta_edges = 0.0;
};

enum class FilterMode {
    MERGE,
    SUPRESS,   
};

class ContentDetector {
    public:
        ContentDetector(const float threshold = 27.0, 
                        const int32_t min_scene_len = 15,
                        const Components weights = Components(),
                        const FilterMode filter_mode = FilterMode::MERGE)
        : threshold_(threshold), min_scene_len_(min_scene_len), weights_(weights), filter_mode_(filter_mode), last_frame_(std::nullopt) {}
        std::vector<frame_timecode::FrameTimeCode> process_frame(video_frame::VideoFrame& next_frame);

    private:
        float _calculate_frame_score(video_frame::VideoFrame& next_frame);
        float _mean_pixel_distance(video_frame::VideoFrame& next_frame) const;

        const float threshold_;
        const int32_t min_scene_len_;
        const Components weights_;
        const FilterMode filter_mode_;
        float frame_score_;
        std::optional<cv::Mat> last_frame_;
};



}

#endif