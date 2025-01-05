#ifndef CONFIG_H
#define CONFIG_H

#include "frame_timecode.hpp"

#include <opencv2/opencv.hpp>
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

struct ResizedSize {
    const int32_t height;
    const int32_t width;
    const float scale;
    const ResizeMode resize;
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

    float scale; /* scale is ignored if width and height are set. */
    int32_t height;
    int32_t width;
    ResizeMode resize;

    /* time information 
       Time expression is represented as string format,
       i.e., HH:MM:SS[.nnn] or HH:MM:SS. e.g., 00:05:00[.000] or 00:05:00
       These values is set to be std::nullopt as default value, 
       representing start = 0(s), end = video_duration(s).
    */
    const std::optional<std::string> start;
    const std::optional<std::string> end;
    const std::optional<std::string> duration;

    std::optional<FrameTimeCode> start_timecode = std::nullopt;
    std::optional<FrameTimeCode> end_timecode = std::nullopt;

    /* detector parameters */
    const float threshold;
    const int32_t min_scene_len;
};

std::string _interpret_filename(const std::filesystem::path& input_path,
                                const argparse::ArgumentParser& program);
WithError<Config> _construct_config(argparse::ArgumentParser& program);
std::pair<int32_t, int32_t> _calculate_resized_size(const VideoStream& video, 
                                                    std::optional<int32_t> width, 
                                                    std::optional<int32_t> height);
ResizedSize _get_size(const std::string& command, const cv::VideoCapture& cap,
                      const std::optional<int32_t> height, const std::optional<int32_t> width,
                      const std::optional<float> scale);

WithError<Config> parse_args(int argc, char *argv[]);
void update_config_with_video(Config& cfg, const VideoStream& video);

#endif