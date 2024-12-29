#ifndef IMAGE_EXTRACTOR_H
#define IMAGE_EXTRACTOR_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <filesystem>

template <typename T> struct WithError;

class ImageExtractor {
    public:
        ImageExtractor(const std::filesystem::path& output_dir);
        WithError<void> save_images(const std::filesystem::path& input_path,
                                    const std::vector<FrameTimeCodePair>& scene_list) const;

    private:
        std::vector<int32_t> _get_frame_range(const FrameTimeCode& start, 
                                              const FrameTimeCode& end) const;
        std::vector<int32_t> _get_frame_inds(const std::vector<int32_t>& frame_range) const;
        std::vector<std::vector<int32_t>> _construct_splits(const std::vector<int32_t>& frame_range) const;

        const int32_t image_num_ = 3;
        const int32_t frame_margin_ = 1;
        const std::filesystem::path output_dir_;
};

#endif