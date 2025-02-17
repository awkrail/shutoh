#include "shutoh/detector/content_detector.hpp"
#include "shutoh/video_frame.hpp"

ContentDetector::ContentDetector(const float threshold, const int32_t min_scene_len)
    : threshold_{threshold}, min_scene_len_{min_scene_len} {}

std::optional<int32_t> ContentDetector::process_frame(const VideoFrame& next_frame) {
    const float frame_score = _calculate_frame_score(next_frame);
    frame_score_ = frame_score; /* for adaptive detector */
    const bool is_above_threshold = (frame_score > threshold_);
    const int32_t frame_num = next_frame.frame_num;
    std::optional<int32_t> cut = flash_filter_.filter(frame_num, is_above_threshold);
    return cut;
}

float ContentDetector::_calculate_frame_score(const VideoFrame& next_frame) {
    cv::cvtColor(next_frame.frame, next_frame.frame, cv::COLOR_BGR2HSV);
    if(!last_frame_.has_value()) {
        last_frame_ = next_frame.frame;
        return 0.0f;
    }
    const float delta = _mean_pixel_distance(next_frame);
    last_frame_ = next_frame.frame;
    return delta;
}

float ContentDetector::_mean_pixel_distance(const VideoFrame& next_frame) const {
    const int32_t num_pixels = next_frame.frame.rows * next_frame.frame.cols;   
    cv::Mat pixel_diff;
    cv::absdiff(last_frame_.value(), next_frame.frame, pixel_diff);
    cv::Scalar pixel_diff_channel_sum = cv::sum(pixel_diff);
    const float pixel_diff_sum = static_cast<float>((pixel_diff_channel_sum[0] + pixel_diff_channel_sum[1] + pixel_diff_channel_sum[2]) / num_pixels / 3.0);
    return pixel_diff_sum;
}

std::shared_ptr<ContentDetector> ContentDetector::initialize_detector(float threshold, int32_t min_scene_len) {
    if (threshold < 0.0) {
        std::cout << "Warning: threshold should be positive and is reset to be 27.0f" << std::endl;
        threshold = 27.0f;
    }

    if (min_scene_len < 0) {
        std::cout << "Warning: min_scene_len should be positive and is reset to be 15" << std::endl;
        min_scene_len = 15;
    }
    return std::make_shared<ContentDetector>(threshold, min_scene_len);
}