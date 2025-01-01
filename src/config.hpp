#ifndef CONFIG_H
#define CONFIG_H

#include "frame_timecode.hpp"

#include <filesystem>
#include <optional>

struct CommonConfig {
    /* common */
    const std::filesystem::path input_path;
    const std::filesystem::path output_dir;
    const std::string command;
    const std::string filename;
};

struct ListSceneConfig {
    const CommonConfig common_cfg;
    const bool no_output_file;
};

struct SplitVideoConfig {
    const CommonConfig common_cfg;
    const bool copy;
    const int32_t crf;
    const std::string preset;
    const std::string ffmpeg_args;
};

struct SaveImageConfig {
    const CommonConfig common_cfg;
    const int32_t num_images;
    const std::string format;
    const int32_t quality;
    const int32_t frame_margin;
    const float scale; /* scale is ignored if width and height are set. */
    const int32_t height;
    const int32_t width;
};

struct TimeConfig {
    const FrameTimeCode start;
    const FrameTimeCode end;
    const FrameTimeCode duration;
};

std::optional<CommonConfig> parse_args(int argc, char *argv[]);

#endif