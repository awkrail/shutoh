#ifndef VIDEO_SPLITTER_H
#define VIDEO_SPLITTER_H

#include "frame_timecode_pair.hpp"

#include <string>
#include <vector>
#include <filesystem>

template <typename T> struct WithError;

class VideoSplitter {
    public:
        VideoSplitter(const std::filesystem::path& output_dir);
        WithError<void> split_video(const std::filesystem::path& input_path,
                                    const int32_t crf,
                                    const std::string& preset,
                                    const std::string& ffmpeg_args,
                                    const std::vector<FrameTimeCodePair>& scene_list) const;

    private:
        const std::filesystem::path output_dir_;
};

void cut_video_with_ffmpeg(const std::string& command);

#endif