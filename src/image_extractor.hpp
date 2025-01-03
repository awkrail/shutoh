#ifndef IMAGE_EXTRACTOR_H
#define IMAGE_EXTRACTOR_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <filesystem>

class VideoStream;
template <typename T> struct WithError;

struct StartEndSplitIndex {
    const int32_t start; /* start index of the scene */
    const int32_t end; /* end index of the scene */
};

struct SceneFrameIndex {
    const int32_t scene_ind; /* scene index in the video */
    const int32_t frame_ind_in_scene; /* frame index in the scene (relative) */
    const int32_t frame_ind_in_video; /* frame index in the video (absolute) */
};

class ImageExtractor {
    public:
        ImageExtractor(const std::filesystem::path& output_dir, const int32_t num_images, const int32_t frame_margin);
        WithError<void> save_images(VideoStream& video, const std::filesystem::path& input_path,
                                    const std::vector<FrameTimeCodePair>& scene_list) const;

    private:
        std::vector<int32_t> _get_frame_inds(const FrameTimeCode& start, 
                                             const FrameTimeCode& end) const;
        std::vector<StartEndSplitIndex> _construct_splits(const FrameTimeCode& start,
                                                          const FrameTimeCode& end) const;

        const std::filesystem::path output_dir_;
        const int32_t num_images_;
        const int32_t frame_margin_;
};

#endif