#ifndef CONFIG_H
#define CONFIG_H

#include "frame_timecode.hpp"

#include <filesystem>
#include <optional>
#include <argparse/argparse.hpp>

class VideoStream;
template <typename T> struct WithError;

enum class ResizeMode {
    ORIGINAL,
    RESIZE_TARGET,
    RESIZE_SCALE,
};

struct Config {
    /* common */
    const std::filesystem::path input_path;
    const std::filesystem::path output_dir;
    const std::string command;
    const std::string filename;
    const bool verbose;

    /* list-scene */
    const bool no_output_file;

    /* split-video */
    const bool copy;
    const int32_t crf;
    const std::string preset;
    const std::string ffmpeg_args;

    /* save-images */
    const int32_t num_images;
    const std::string format;
    const int32_t quality;
    const int32_t frame_margin;

    std::optional<float> scale; /* scale is ignored if width and height are set. */
    std::optional<int32_t> height;
    std::optional<int32_t> width;
    ResizeMode resize = ResizeMode::ORIGINAL;

    /* time information 
       Time expression is represented as string format,
       i.e., HH:MM:SS[.nnn] or HH:MM:SS. e.g., 00:05:00[.000] or 00:05:00
       These values is set to be std::nullopt as default value, 
       representing start = 0(s), end = video_duration(s).
    */
    const std::optional<std::string> start;
    const std::optional<std::string> end;
    const std::optional<std::string> duration;

    /* detector parameters */
    const float threshold;
    const int32_t min_scene_len;
};

WithError<Config> _construct_config(argparse::ArgumentParser& program);
WithError<Config> parse_args(int argc, char *argv[]);
void update_config_with_video(Config& cfg, const VideoStream& video);
std::pair<int32_t, int32_t> _calculate_resized_size(const VideoStream& video, 
                                                    std::optional<int32_t> width, 
                                                    std::optional<int32_t> height);

#endif