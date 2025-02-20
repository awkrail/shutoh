#include "shutoh/detector/histogram_detector.hpp"
#include "shutoh/video_frame.hpp"

HistogramDetector::HistogramDetector(const float threshold, const int32_t min_scene_len, const int32_t bins) 
    : threshold_{std::max(0.0f, std::min(1.0f, 1.0f - threshold))}, min_scene_len_{min_scene_len}, bins_{bins} {}

std::optional<int32_t> HistogramDetector::process_frame(const VideoFrame& next_frame) {
    std::optional<int32_t> cut = std::nullopt;
    const int32_t frame_num = next_frame.frame_num;

    if (!last_scene_cut_.has_value())
        last_scene_cut_ = frame_num;
    
    cv::Mat hist;
    _calculate_histogram(next_frame.frame, hist);

    if (last_hist_.has_value()) {
        const double hist_diff = cv::compareHist(last_hist_.value(), hist, cv::HISTCMP_CORREL);
        
        if (hist_diff <= threshold_ && (frame_num - last_scene_cut_.value()) >= min_scene_len_) {
            cut = frame_num;
            last_scene_cut_ = frame_num;
        }
    }

    last_hist_ = hist;
    return cut;
}

void HistogramDetector::_calculate_histogram(const cv::Mat& frame, cv::Mat& hist) const {
    cv::Mat yuv;
    cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV);

    std::vector<cv::Mat> yuv_splits;
    cv::split(yuv, yuv_splits);

    cv::Mat luma = yuv_splits[0];
    const int32_t hist_size[] = { bins_ };
    const float range[] = { 0, 256 };
    const float* hist_range[] = { range };

    cv::calcHist(&luma, 1, 0, cv::Mat(), hist, 1, hist_size, hist_range, true, false);
    cv::normalize(hist, hist);
}

std::shared_ptr<HistogramDetector> HistogramDetector::initialize_detector(float threshold, int32_t min_scene_len, int32_t bins) {
    if (threshold < 0) {
        std::cout << "Warning: threshold should be positive and is reset to 0.05f." << std::endl;
        threshold = 0.05f;
    }

    if (min_scene_len < 0) {
        std::cout << "Warning: min_scene_len should be positive and is reset to 15." << std::endl;
        min_scene_len = 15;
    }

    if (bins < 0) {
        std::cout << "Warning: bins should be positive and is reset to 256." << std::endl;
        bins = 256;
    }

    return std::make_shared<HistogramDetector>(threshold, min_scene_len, bins);
}