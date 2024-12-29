#include "image_extractor.hpp"
#include "error.hpp"
#include "frame_timecode.hpp"

#include <iostream>
#include <numeric>

ImageExtractor::ImageExtractor(const std::filesystem::path& output_dir) : output_dir_{output_dir} {}

WithError<void> ImageExtractor::save_images(const std::filesystem::path& input_path,
                                            const std::vector<FrameTimeCodePair>& scene_list) const {
    std::vector<std::vector<int32_t>> frame_inds_list;
    for (auto& scene : scene_list) {
        const FrameTimeCode start = std::get<0>(scene);
        const FrameTimeCode end = std::get<1>(scene);
        const std::vector<int32_t> frame_range = _get_frame_range(start, end);
        const std::vector<int32_t> frame_inds = _get_frame_inds(frame_range);
        frame_inds_list.push_back(frame_inds);
    }

    for (auto& frame_inds : frame_inds_list) {
        for (auto& frame_ind : frame_inds) {
            std::cout << frame_ind << " ";
        }
        std::cout << std::endl;
    }

    return WithError<void> { Error(ErrorCode::Success, "") };
}

std::vector<int32_t> ImageExtractor::_get_frame_range(const FrameTimeCode& start, 
                                                      const FrameTimeCode& end) const {
    const int32_t start_frame = start.get_frame_num();
    const int32_t end_frame = end.get_frame_num();    
    const size_t range_length = std::max(1, end_frame - start_frame);
    std::vector<int32_t> frame_range(range_length);
    std::iota(frame_range.begin(), frame_range.end(), start_frame);
    return frame_range;
}

std::vector<int32_t> ImageExtractor::_get_frame_inds(const std::vector<int32_t>& frame_range) const {
    std::vector<std::vector<int32_t>> splits = _construct_splits(frame_range);
    std::vector<int32_t> frame_inds;
    
    const int32_t split_size = splits.size();
    for (int j = 0; j < split_size; j++) {
        const size_t last_ind = splits[j].size();
        int32_t frame_ind;
        if (j == 0) {
            /* First split */
            const size_t last_ind = splits[j].size() - 1;
            frame_ind = std::min(splits[j][0] + frame_margin_, splits[j][last_ind-1]);
        } else if ((j > 0 && j < image_num_ - 1) || image_num_ == 1) {
            /* between first and last splits (1..n-2) */
            frame_ind = splits[j][last_ind / 2];
        } else {
            /* Last split */
            frame_ind = std::max(splits[j][last_ind-1] - frame_margin_, splits[j][0]);
        }
        frame_inds.push_back(frame_ind);
    }
    return frame_inds;
}

std::vector<std::vector<int32_t>> ImageExtractor::_construct_splits(const std::vector<int32_t>& frame_range) const {
    std::vector<std::vector<int32_t>> result;
    const int32_t total_size = frame_range.size();
    const int32_t base_size = total_size / image_num_;
    const int32_t remainder = total_size % image_num_;
    
    size_t start = 0;
    for (int32_t i = 0; i < image_num_; i++) {
        const size_t split_size = base_size + (i < remainder ? 1 : 0);
        result.emplace_back(frame_range.begin() + start, frame_range.begin() + start + split_size);
        start += split_size;
    }
    return result;
}