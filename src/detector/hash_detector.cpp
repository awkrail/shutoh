#include "shutoh/detector/hash_detector.hpp"
#include "shutoh/video_frame.hpp"

HashDetector::HashDetector(const float threshold, const int32_t min_scene_len,
                           const int32_t size, const int32_t lowpass) : threshold_{threshold}, min_scene_len_{min_scene_len}, 
                           size_{size}, size_sq_{size * size}, factor_{lowpass}, imsize_{cv::Size(size * lowpass, size * lowpass)} {}

std::optional<int32_t> HashDetector::process_frame(const VideoFrame& next_frame) {
    std::optional<int32_t> cut = std::nullopt;
    const int32_t frame_num = next_frame.frame_num;

    if (!last_scene_cut_.has_value())
        last_scene_cut_ = frame_num;
    
    if (last_frame_.has_value()) {
        cv::Mat curr_hash;
        _hash_frame(next_frame.frame, curr_hash);

        if (!last_hash_.has_value()) {
            cv::Mat last_hash;
            _hash_frame(last_frame_.value(), last_hash);
            last_hash_ = last_hash;
        }
        
        const int32_t hash_dist = _calculate_hamming_distance(curr_hash, last_hash_.value());
        const float hash_dist_norm = static_cast<float>(hash_dist) / size_sq_;

        last_hash_ = curr_hash;

        if (hash_dist_norm >= threshold_ && (frame_num - last_scene_cut_.value() >= min_scene_len_)) {
            cut = frame_num;
            last_scene_cut_ = frame_num;
        }
    }

    last_frame_ = next_frame.frame;
    return cut;
}

void HashDetector::_hash_frame(const cv::Mat& frame, cv::Mat& hash) const {
    /* Convert frames to grayscale */
    cv::Mat gray_img;
    cv::cvtColor(frame, gray_img, cv::COLOR_BGR2GRAY);

    /* Resize image to square to apply DCT */
    cv::resize(gray_img, gray_img, imsize_, 0, 0, cv::INTER_AREA);

    /* Check to avoid dividing by zero */
    double max_val;
    cv::minMaxLoc(gray_img, nullptr, &max_val, nullptr, nullptr);
    if (max_val == 0.0)
        max_val = 1.0;

    /* Calculate discrete cosine transformation of the image */
    gray_img.convertTo(gray_img, CV_32F);
    gray_img = gray_img / max_val;
    cv::Mat dct_complete;
    cv::dct(gray_img, dct_complete);

    /* Keep low frequency */
    cv::Mat dct_sliced;
    dct_sliced = dct_complete(cv::Range(0, size_), cv::Range(0, size_));
    dct_sliced.copyTo(dct_complete);
    const float median = _calculate_median_in_DCT(dct_complete);

    /* Calculate hash: note that the hash does not return 0/1 but 0/255, yet ok for countNonZero function */
    hash = (dct_complete > median);
}

int32_t HashDetector::_calculate_hamming_distance(const cv::Mat& curr, const cv::Mat& last) const {
    cv::Mat diff = (curr != last);
    return cv::countNonZero(diff);
}

float HashDetector::_calculate_median_in_DCT(const cv::Mat& dct) const {
    /* WATCH: median calculation is based on numpy, where it is computed based on two center elements
       if the number of elements in array is even. This implementation assumes that the size of DCT matrix is even.
       If the users want to use HashDetector with odd size DCT matrix, I will fix it in the future.
     */
    std::vector<float> dct_values;
    dct.reshape(1, 1).copyTo(dct_values);
    
    const size_t median_ind = dct_values.size() / 2;
    std::nth_element(dct_values.begin(), dct_values.begin() + median_ind, dct_values.end());
    const float median_high = dct_values[median_ind];
    std::nth_element(dct_values.begin(), dct_values.begin() + median_ind - 1, dct_values.end());
    const float median_low = dct_values[median_ind-1];
    const float median = (median_high + median_low) / 2.0f;
    
    return median;
}

std::unique_ptr<HashDetector> HashDetector::initialize_detector(float threshold, int32_t min_scene_len,
                                                                int32_t dct_size, int32_t lowpass) {
    if (threshold < 0.0f) {
        std::cout << "Warning: threshold should be positive and is reset to 27.0f." << std::endl;
        threshold = 0.395f;
    }

    if (min_scene_len < 0) {
        std::cout << "Warning: min_scene_len should be positive and is reset to 15." << std::endl;
        min_scene_len = 15;
    }

    if (dct_size < 0) {
        std::cout << "Warning: dct_size should be positive and is reset to 16." << std::endl;
        dct_size = 16;
    }

    if (lowpass < 0) {
        std::cout << "Warning: low_pass should be positive and is reset to 2." << std::endl;
    }

    return std::make_unique<HashDetector>(threshold, min_scene_len, dct_size, lowpass);
}