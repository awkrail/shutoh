#include <stdexcept>
#include <optional>
#include "flash_filter.hpp"

FlashFilter::FlashFilter(const FilterMode mode, const int32_t length) : mode_{mode}, filter_length_{length} {}

std::optional<int32_t> FlashFilter::filter(const int32_t frame_num, const bool is_above_threshold) {
    if (!last_above_.has_value()) {
        last_above_ = frame_num;
    }
    if (mode_ == FilterMode::MERGE) {
        return _filter_merge(frame_num, is_above_threshold);
    } else {
        throw std::runtime_error("Currently, only mode_ == FilterMode::MERGE is supported.");
    }
}

std::optional<int32_t> FlashFilter::_filter_merge(const int32_t frame_num, const bool is_above_threshold) {
    const bool min_length_met = (frame_num - last_above_.value()) >= filter_length_;
    if (is_above_threshold) {
        last_above_ = frame_num;
    }
    if (merge_triggered) {
        const int32_t num_merged_frames = last_above_.value() - merge_start_.value();
        if (min_length_met && (!is_above_threshold) && num_merged_frames >= filter_length_) {
            merge_triggered = false;
            return last_above_.value();
        }
        return std::nullopt;
    }

    if (!is_above_threshold) {
        return std::nullopt;
    }

    if (min_length_met) {
        merge_enabled = true;
        return frame_num;
    }

    if (merge_enabled) {
        merge_triggered = true;
        merge_start_ = frame_num;
    }

    return std::nullopt;
}