#include "shutoh/detector/threshold_detector.hpp"
#include "shutoh/video_frame.hpp"

ThresholdDetector::ThresholdDetector(const float threshold, const int32_t min_scene_len, const float fade_bias)
    : threshold_{threshold}, min_scene_len_{min_scene_len}, fade_bias_{fade_bias} {}

std::optional<int32_t> ThresholdDetector::process_frame(const VideoFrame& next_frame) {
    std::optional<int32_t> cut = std::nullopt;
    const int32_t frame_num = next_frame.frame_num;

    if (!last_scene_cut_.has_value())
        last_scene_cut_ = frame_num;
    
    const float frame_avg = _compute_frame_average(next_frame.frame);

    if (process_frame_) {
        if (last_fade_.value() == Fade::FADE_IN && frame_avg < threshold_) {
            last_fade_ = Fade::FADE_OUT;
            last_frame_ = frame_num;
        } else if (last_fade_.value() == Fade::FADE_OUT && frame_avg >= threshold_) {
            if ((frame_num - last_scene_cut_.value()) >= min_scene_len_) {
                const int32_t f_out = last_frame_;
                const int32_t f_split = (frame_num + f_out + static_cast<int32_t>(fade_bias_ * (frame_num - f_out))) / 2;
                cut = f_split;
                last_scene_cut_ = frame_num;
            }
            last_fade_ = Fade::FADE_IN;
            last_frame_ = frame_num;
        }
    } else {
        last_frame_ = 0;
        if (frame_avg < threshold_)
            last_fade_ = Fade::FADE_OUT;
        else
            last_fade_ = Fade::FADE_IN;
    }

    process_frame_ = true;
    return cut;
}

float ThresholdDetector::_compute_frame_average(const cv::Mat& frame) const {
    const cv::Scalar sum_value = cv::sum(frame);
    const int32_t total_pixels = frame.rows * frame.cols * frame.channels();
    return static_cast<float>(sum_value[0] + sum_value[1] + sum_value[2]) / total_pixels;
}

std::shared_ptr<ThresholdDetector> ThresholdDetector::initialize_detector(float threshold, int32_t min_scene_len, float fade_bias) {
    if (threshold < 0) {
        std::cout << "Warning: threshold should be positive and is reset to 12.0f." << std::endl;
        threshold = 12.0f;
    }

    if (min_scene_len < 0) {
        std::cout << "Warning: min_scene_len should be positive and is reset to 15." << std::endl;
        min_scene_len = 15;
    }

    if (std::abs(fade_bias) > 1) {
        std::cout << "Warning: fade_bias should be between -1.0f and +1.0f. Reset to 0.0f" << std::endl;
        fade_bias = 0.0f;
    }

    return std::make_shared<ThresholdDetector>(threshold, min_scene_len, fade_bias);
}