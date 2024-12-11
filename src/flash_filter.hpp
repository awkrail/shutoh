#ifndef FLASH_FILTER_H
#define FLASH_FILTER_H

#include <cstdint>
#include <optional>

namespace flash_filter {

enum class FilterMode {
    MERGE,
    SUPRESS,   
};

class FlashFilter {
    public:
        FlashFilter(FilterMode mode, int32_t filter_length);
        std::optional<int32_t> filter(const int32_t frame_num, const bool is_above_threshold);

    private:
        std::optional<int32_t> _filter_merge(const int32_t frame_num, const bool is_above_threshold);
        FilterMode mode_;
        int32_t filter_length_;
        std::optional<int32_t> last_above_ = std::nullopt;
        std::optional<int32_t> merge_start_ = std::nullopt;
        bool merge_enabled = false;
        bool merge_triggered = false;
};


}

#endif