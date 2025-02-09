#include "shutoh/detector/adaptive_detector.hpp"
#include "shutoh/video_frame.hpp"

#include <algorithm>
#include <cmath>

AdaptiveDetector::AdaptiveDetector(const float adaptive_threshold, const int32_t min_scene_len,
                                   const int32_t window_width, const float min_content_val)
    : ContentDetector(255.0, 0), adaptive_threshold_{adaptive_threshold}, min_scene_len_{min_scene_len}, 
      window_width_{window_width}, min_content_val_{min_content_val}, required_frames_{1 + (2 * static_cast<size_t>(window_width_))},
      buffer_{required_frames_} {}

std::optional<int32_t> AdaptiveDetector::process_frame(const VideoFrame& next_frame) {
    ContentDetector::process_frame(next_frame);
    
    const int32_t frame_num = next_frame.frame_num;
    if (!last_cut_.has_value())
        last_cut_ = frame_num;
    
    const float frame_score = ContentDetector::get_frame_score().value();

    buffer_.push(FrameNumScore { frame_num, frame_score });
    if (buffer_.size() < required_frames_)
        return std::nullopt;

    const int32_t target_frame = buffer_[window_width_].frame_num;
    const float target_score = buffer_[window_width_].frame_score;

    const float average_window_score = _calculate_average_window_score();
    const bool is_average_zero = std::abs(average_window_score) < 0.00001f;

    float adaptive_ratio = 0.0f;
    if (!is_average_zero)
        adaptive_ratio = std::min(target_score / average_window_score, 255.0f);
    else if (is_average_zero && target_score >= min_content_val_)
        adaptive_ratio = 255.0f;

    const bool threshold_met = (adaptive_ratio >= adaptive_threshold_ && target_score >= min_content_val_);
    const bool min_length_met = (frame_num - last_cut_.value()) >= min_scene_len_;

    if (threshold_met && min_length_met) {
        last_cut_ = target_frame;
        return target_frame;
    }

    return std::nullopt;
}

float AdaptiveDetector::_calculate_average_window_score() const {
    float average_window_score = 0.0f;
    for (size_t i = 0; i < buffer_.size(); i++) {
        if (i != static_cast<size_t>(window_width_))
            average_window_score += buffer_[i].frame_score;
    }
    return average_window_score / (2.0f * window_width_);
}

std::unique_ptr<AdaptiveDetector> AdaptiveDetector::initialize_detector(float adaptive_threshold, int32_t min_scene_len,
                                                                        int32_t window_width, float min_content_val) {
    if (adaptive_threshold < 0.0) {
        std::cout << "Warning: threshold should be positive and is reset to be 3.0" << std::endl;
        adaptive_threshold = 3.0f;
    }
    if (min_scene_len < 0) {
        std::cout << "Warning: min_scene_len should be positive and is reset to be 15" << std::endl;
        min_scene_len = 15;
    }
    if (window_width < 0) {
        std::cout << "Warning: window_width should be positive and is reset to be 2" << std::endl;
        window_width = 2;
    }
    if (min_content_val < 0) {
        std::cout << "Warning: min_content_val should be positive and is reset to be 15.0" << std::endl;
        min_content_val = 15.0; 
    }
    return std::make_unique<AdaptiveDetector>(adaptive_threshold, min_scene_len, window_width, min_content_val);
}