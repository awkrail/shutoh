#include "adaptive_detector.hpp"
#include "../video_frame.hpp"

AdaptiveDetector::AdaptiveDetector(const float adaptive_threshold, const int32_t min_scene_len,
                                   const int32_t window_width, const float min_content_val)
    : ContentDetector(255.0, 0), adaptive_threshold_{adaptive_threshold}, min_scene_len_{min_scene_len}, 
      window_width_{window_width}, min_content_val_{min_content_val}, required_frames_{1 + (2 * static_cast<size_t>(window_width_))},
      buffer_{required_frames_} {}

std::optional<int32_t> AdaptiveDetector::process_frame(const VideoFrame& next_frame) {
    std::optional<int32_t> cut = std::nullopt;
    ContentDetector::process_frame(next_frame);
    
    const int32_t frame_num = next_frame.frame_num;
    if (!last_cut_.has_value())
        last_cut_ = frame_num;
    
    const float frame_score = ContentDetector::get_frame_score().value();

    buffer_.push(FrameNumScore { frame_num, frame_score });
    if (buffer_.size() < required_frames_)
        return std::nullopt;

    //const FrameNumScore frame_num_score = buffer_[window_width_];
    const float average_window_score = _calculate_average_window_score();
    std::cout << average_window_score << std::endl;

    return cut;   
}

float AdaptiveDetector::_calculate_average_window_score() const {
    float average_window_score = 0.0f;
    for (size_t i = 0; i < buffer_.size(); i++) {
        if (i != static_cast<size_t>(window_width_))
            average_window_score += buffer_[i].frame_score;
    }
    return average_window_score / (2.0f * window_width_);
}